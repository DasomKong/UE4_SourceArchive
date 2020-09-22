// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Ghost.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "../object/ghostDummy.h"

UAnimNotifyState_Ghost::UAnimNotifyState_Ghost()
	:spawnCount(5), LifeTime(2.f), _material(nullptr), passedTime(0.f), SpawnSec(0.f), RealCount(5)
{
	_material = LoadObject<UMaterialInstance>(NULL, TEXT("MaterialInstanceConstant'/Game/art/Material/ghostMat_Inst.ghostMat_Inst'"));

	// ���� ��Į�� 3���� ���� 1�� �Ķ���͵��� ��� ��.
	//resetMaterialParameters();
}

void UAnimNotifyState_Ghost::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Received_NotifyBegin(MeshComp, Animation, TotalDuration);

	SetNatrualSpawnCountAndTimeRate(TotalDuration);

	passedTime = 0.f;

	SpawnGhost(MeshComp, Animation);
}


void UAnimNotifyState_Ghost::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Received_NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (passedTime >= SpawnSec)
	{
		passedTime = 0.f;

		SpawnGhost(MeshComp, Animation);
	}
	passedTime += FrameDeltaTime;
}

void UAnimNotifyState_Ghost::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Received_NotifyEnd(MeshComp, Animation);

	SpawnGhost(MeshComp, Animation);
}

void UAnimNotifyState_Ghost::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	// �����ϰ� ������ �����Ǿ����� ����ϴ� �뵵.
	//TArray<FName> tmpKey;
	//scalarParam.GetKeys(tmpKey);

	//for (FName _name : tmpKey)
	//	MID->SetScalarParameterValue(_name, *scalarParam.Find(_name));

	//tmpKey.Empty();
	//colorParam.GetKeys(tmpKey);

	//for (FName _name : tmpKey)
	//	MID->SetVectorParameterValue(_name, *colorParam.Find(_name));
}

void UAnimNotifyState_Ghost::SpawnGhost(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (RealCount < 1)
		return;

	UWorld* world = MeshComp->GetWorld();

	if (world && MeshComp->GetOwner())
	{
		AghostDummy* newGhost = world->SpawnActor<class AghostDummy>();

		newGhost->SetActorLocationAndRotation(MeshComp->GetOwner()->GetActorLocation() + MeshComp->GetRelativeLocation(),
			MeshComp->GetOwner()->GetActorRotation() + MeshComp->GetRelativeRotation());

		newGhost->SetParams(FGhostData(_material, MeshComp, Animation, LifeTime, doFade), FGhostMatData(paramOpacity, paramExponentIn, colorParam));

		--RealCount;
	}
}

void UAnimNotifyState_Ghost::SetNatrualSpawnCountAndTimeRate(float TotalDuration)
{
	// ������ 0�� �� �ȴ�. ������ 1�� fix.
	if (spawnCount <= 0)
		RealCount = 1;
	else
		RealCount = spawnCount;

	if (spawnCount >= 3)
	{
		// 3�� �̻��� ���, ���۰� �� �ϳ��� ����ϵ����ϰ� ���� ���� + 1 �� �ð��� ������.
		int tmpCount = spawnCount - 2;

		SpawnSec = TotalDuration / (tmpCount + 1);

	}
	else
	{
		// 2�� ������ ���, ���۰� ������ ��ġ�ϵ��� �ϹǷ�, ���� ƽ �߿� ������ �� �ϵ���.
		SpawnSec = TotalDuration + 0.3f;
	}
}
