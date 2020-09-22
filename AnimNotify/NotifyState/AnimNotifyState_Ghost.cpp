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

	// 현재 스칼라 3개와 벡터 1개 파라미터들을 사용 중.
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
	// 순수하게 변경이 감지되었을때 사용하는 용도.
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
	// 음수나 0은 안 된다. 강제로 1로 fix.
	if (spawnCount <= 0)
		RealCount = 1;
	else
		RealCount = spawnCount;

	if (spawnCount >= 3)
	{
		// 3개 이상일 경우, 시작과 끝 하나씩 출력하도록하고 남은 갯수 + 1 로 시간을 나눈다.
		int tmpCount = spawnCount - 2;

		SpawnSec = TotalDuration / (tmpCount + 1);

	}
	else
	{
		// 2개 이하일 경우, 시작과 끝에만 배치하도록 하므로, 절대 틱 중에 만나지 못 하도록.
		SpawnSec = TotalDuration + 0.3f;
	}
}
