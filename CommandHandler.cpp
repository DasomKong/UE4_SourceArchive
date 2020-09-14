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

	// 등록된 키 중 눌린 키 전부 체크

	// 방향 키 체크
	for (auto& val : reMappedAxisKey)
	{
		if (controller->GetInputAnalogKeyState(val.Key))
		{
			currentPressedKey.Add(val.Value);
		}
	}
	
	/*
	현재 대각선 방향키를 사용하고 있지 않으므로, 동시에 눌린 방향키는 단순히 지워버리는 방식으로 처리,
	이후 대각선 커맨드를 사용하게 된다면 커맨드맵을 대각선을 포함한 커맨드로 변경하고, 
	"아래 + 앞" 과 같은 커맨드를 보정해서 "아래 + 생성 '대각선 아래앞' + 앞" 의 형식으로 변경할 수 있도록 작업할 것.
	-> 이전 키를 확인해서 유의미한 키 변경이 있다면 중간에 키를 생성시키는 것.
	*/ 
	
	/*
	if(currentPressedKey.Num() > 1)
	{
		int32 dirVal = 0;

		// 방향키부터 확인. -> 대각선이 필요해질 경우.
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

	// 액션 키 체크
	for (auto& val : reMappedActionKey)
	{
		if (controller->WasInputKeyJustPressed(val.Key))
		{
			currentPressedKey.Add(val.Value);
		}
	}

	// 누른키가 없으면 중립
	if (currentPressedKey.Num() < 1)
	{
		currentPressedKey.Add(EJHPInputKey::NEUTRAL);
	}
	else
	{
		passedTime = 0.f;
	}

	// 중복키 확인
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

	// 눌린 키가 존재한다면 등록
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
	// 추후 단 한번만 작동하도록 세팅.
	{
		usableCommandMap.Reserve(7);

		FCommandMap temp;

		for (FCommandArray arr : temp.cmdArray)
		{
			usableCommandMap.Add(arr.cmdID ,arr.commandArray);
		}
	}

	// 커맨드가 긴 기술을 앞으로 배치.
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

	// 복사
	for (int i = 0; i < storeKeyArray.Num(); ++i)
		tmpInput.Add(storeKeyArray[i]);

	// 방향 교정
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
