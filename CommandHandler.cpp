// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandHandler.h"

#include "GameFramework/InputSettings.h"
#include "GameFrameWork/PlayerController.h"

//#include "InputCoreTypes.h"

// Sets default values for this component's properties
UCommandHandler::UCommandHandler()
	:maxWaitTime(0.15f), maxInputCount(10), passedTime(0.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	storeKeyArray.Reserve(maxInputCount);
}


// Called when the game starts
void UCommandHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	controller = Cast<APlayerController>(GetOwner());

	setUsableCommandKeys();
}

// Called every frame
void UCommandHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	// test
	/*{
		FString debugMessage;

		for (EJHPInputKey& key : storeKeyArray)
		{
			debugMessage.Append(FString::Printf(TEXT("%d,"), static_cast<int8>(key)));
		}

		UE_LOG(LogTemp, Warning, TEXT("current input keys : %s"), *debugMessage);
	}*/

	parseCurrentFrameKey();

	if (storeKeyArray.Num() > 0)
		updateKeyArray(DeltaTime);
}

void UCommandHandler::parseCurrentFrameKey(void)
{
	TArray<EJHPInputKey> currentPressedKey;

	// ��ϵ� Ű �� ���� Ű ���� üũ

	// ���� Ű üũ
	for (auto& val : reMappedAxisKey)
	{
		if (controller->GetInputAnalogKeyState(val.Key))
		{
			currentPressedKey.Add(val.Value);
		}
	}
	
	/*
	���� �밢�� ����Ű�� ����ϰ� ���� �����Ƿ�, ���ÿ� ���� ����Ű�� �ܼ��� ���������� ������� ó��,
	���� �밢�� Ŀ�ǵ带 ����ϰ� �ȴٸ� Ŀ�ǵ���� �밢���� ������ Ŀ�ǵ�� �����ϰ�, 
	"�Ʒ� + ��" �� ���� Ŀ�ǵ带 �����ؼ� "�Ʒ� + ���� '�밢�� �Ʒ���' + ��" �� �������� ������ �� �ֵ��� �۾��� ��.
	-> ���� Ű�� Ȯ���ؼ� ���ǹ��� Ű ������ �ִٸ� �߰��� Ű�� ������Ű�� ��.
	*/ 
	
	/*
	if(currentPressedKey.Num() > 1)
	{
		int32 dirVal = 0;

		// ����Ű���� Ȯ��. -> �밢���� �ʿ����� ���.
		for (auto& val : currentPressedKey)
		{
			dirVal += static_cast<int32>(val);
		}

		currentPressedKey.Empty();

		EJHPInputKey newKey = static_cast<EJHPInputKey>(dirVal);

		switch (newKey)
		{
		case EJHPInputKey::R_DOWN:
		case EJHPInputKey::L_DOWN:
		case EJHPInputKey::R_UP:
		case EJHPInputKey::L_UP:
			currentPressedKey.Add(newKey);
			break;
		default:
			break;
		}
	}
	*/

	if (currentPressedKey.Num() > 1)
		currentPressedKey.Empty();

	// �׼� Ű üũ
	for (auto& val : reMappedActionKey)
	{
		if (controller->WasInputKeyJustPressed(val.Key))
		{
			currentPressedKey.Add(val.Value);
		}
	}

	// ����Ű�� ������ �߸�
	if (currentPressedKey.Num() < 1)
	{
		currentPressedKey.Add(EJHPInputKey::NEUTRAL);
	}
	else
	{
		passedTime = 0.f;
	}

	// �ߺ�Ű Ȯ��
	if (storeKeyArray.Num() > 0)
	{
		if (storeKeyArray.Top() == currentPressedKey[0])
		{
			currentPressedKey.RemoveAt(0);
		}
		else if (storeKeyArray.Top() == EJHPInputKey::NEUTRAL)
		{
			storeKeyArray.Pop();
		}
	}
	
	const int32 storedKeyCount = storeKeyArray.Num();
	const int32 curKeyCount = currentPressedKey.Num();

	// ���� Ű�� �����Ѵٸ� ���
	if (storedKeyCount + curKeyCount > maxInputCount)
	{
		storeKeyArray.RemoveAt(0, storedKeyCount > curKeyCount ? curKeyCount : storedKeyCount);
	}
	
	storeKeyArray.Append(currentPressedKey);

}

void UCommandHandler::mapInputSettings(UInputSettings * inputSettings)
{
	TArray<FInputActionKeyMapping> outActionMappings;

	inputSettings->GetActionMappingByName(FName(TEXT("Atk")), outActionMappings);

	for (auto& key : outActionMappings)
	{
		reMappedActionKey.Add(key.Key, EJHPInputKey::KEY_A);
	}

	outActionMappings.Empty();

	inputSettings->GetActionMappingByName(FName(TEXT("Jump")), outActionMappings);

	for (auto& key : outActionMappings)
	{
		reMappedActionKey.Add(key.Key, EJHPInputKey::KEY_B);
	}

	TArray<FInputAxisKeyMapping> outAxisMappings;

	inputSettings->GetAxisMappingByName(FName(TEXT("MoveForward")), outAxisMappings);

	for (auto& key : outAxisMappings)
	{
		key.Scale > 0 ? reMappedAxisKey.Add(key.Key, EJHPInputKey::RIGHT) : reMappedAxisKey.Add(key.Key, EJHPInputKey::LEFT);
	}

	outAxisMappings.Empty();

	inputSettings->GetAxisMappingByName(FName(TEXT("UpDown")), outAxisMappings);

	for (auto& key : outAxisMappings)
	{
		key.Scale > 0 ? reMappedAxisKey.Add(key.Key, EJHPInputKey::UP) : reMappedAxisKey.Add(key.Key, EJHPInputKey::DOWN);
	}
}

void UCommandHandler::setUsableCommandKeys()
{
	// ���� �� �ѹ��� �۵��ϵ��� ����.
	{
		usableCommandMap.Reserve(7);

		FCommandMap temp;

		for (FCommandArray arr : temp.cmdArray)
		{
			usableCommandMap.Add(arr.cmdID ,arr.commandArray);
		}
	}

	// Ŀ�ǵ尡 �� ����� ������ ��ġ.
	usableCommandMap.ValueSort([](const FCommandKeyArray& a, const FCommandKeyArray& b) {
		if (a.Num() > b.Num())
			return true;
		else
			return false;
	});
}

void UCommandHandler::updateKeyArray(const float deltaTime)
{
	passedTime += deltaTime;
	int32 matchedID = -1;
	
	int methodIdx = 0;

	// jump
	if (storeKeyArray.Last() == EJHPInputKey::KEY_B)
	{
		methodIdx = 1;
	}
	else if (storeKeyArray.Last() == EJHPInputKey::KEY_A)
	{
		methodIdx = 2;

		matchedID = static_cast<int32>(EJHPInputKey::KEY_A);
	}

	// check command
	if (storeKeyArray.Num() > 1)
	{
		const AActor* ctrlPawn = controller->GetPawn();

		if (ctrlPawn && ctrlPawn->IsValidLowLevel())
		{
			const FVector dir = ctrlPawn->GetActorForwardVector();

			bool isForward = dir.X > 0 ? true : false;

			if (checkMatchingCommand(matchedID, isForward))
			{
				methodIdx = 2;
			}
		}
	}

	switch (methodIdx)
	{
	case 1:
	{
		ActionKeyInput.Broadcast(storeKeyArray.Last());

		passedTime += maxWaitTime + 0.1f;
	}
	break;
	case 2:
	{
		CommandMatched.Broadcast(matchedID);

		passedTime += maxWaitTime + 0.1f;
	}
		break;
	}

	if (passedTime >= maxWaitTime)
	{
		passedTime = 0.f;
		storeKeyArray.Empty();
	}
}

bool UCommandHandler::checkMatchingCommand(int32& matchedKeyID, bool isForward)
{
	TArray<EJHPInputKey> tmpInput;

	// ����
	for (int i = 0; i < storeKeyArray.Num(); ++i)
		tmpInput.Add(storeKeyArray[i]);

	// ���� ����
	if (!isForward)
	{
		for (int i = 0; i < tmpInput.Num(); ++i)
		{
			if (tmpInput[i] == EJHPInputKey::LEFT)
			{
				tmpInput[i] = EJHPInputKey::RIGHT;
			}
			else if (tmpInput[i] == EJHPInputKey::RIGHT)
			{
				tmpInput[i] = EJHPInputKey::LEFT;
			}
		}
	}

	const int inputKeyCnt = tmpInput.Num();

	for (auto& cmdMap : usableCommandMap)
	{
		const int cnt = cmdMap.Value.Num();

		if (cnt > inputKeyCnt)
			continue;

		bool isMatched = [cnt, inputKeyCnt](const FCommandKeyArray& src, const FCommandKeyArray& dst)->bool
		{
			for (int i = 0; i < cnt; ++i)
			{
				if (src[cnt - 1 - i] != dst[inputKeyCnt - 1 - i])
				{
					return false;
				}
			}

			return true;
		}(cmdMap.Value, tmpInput);

		if (isMatched)
		{
			matchedKeyID = cmdMap.Key;
			return true;
		}
	}

	return false;
}
