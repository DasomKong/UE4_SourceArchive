// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SpawnDummy.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_SpawnDummy::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#ifdef WITH_EDITOR
	if (dummy == nullptr)
	{
		UWorld* world = MeshComp->GetWorld();

		if (world && MeshComp->GetOwner())
		{
			dummy = world->SpawnActor<AActor>(dummyActorClass, actorTransform);
		}
	}
#endif // WITH_EDITOR
}

void UAnimNotifyState_SpawnDummy::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
#ifdef WITH_EDITOR
	if (dummy)
	{
		dummy->SetActorTransform(actorTransform);
	}
#endif // WITH_EDITOR
}

void UAnimNotifyState_SpawnDummy::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#ifdef WITH_EDITOR
	if (dummy != nullptr)
	{
		dummy->Destroy();
		dummy = nullptr;
	}
#endif // WITH_EDITOR
}
