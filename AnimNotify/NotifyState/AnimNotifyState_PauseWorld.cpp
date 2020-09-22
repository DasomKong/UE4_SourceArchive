// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_PauseWorld.h"

#include "Kismet/GameplayStatics.h"

#include "../../Character/JHPCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_PauseWorld::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AActor* owner = MeshComp->GetOwner();

	proxy_TogglePause(owner, true);
}

void UAnimNotifyState_PauseWorld::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AActor* owner = MeshComp->GetOwner();

	proxy_TogglePause(owner, false);
}

void UAnimNotifyState_PauseWorld::proxy_TogglePause(AActor* owner, bool isOn)
{
	if (owner)
	{
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(owner->GetWorld(), AJHPCharacter::StaticClass(), outActors);

		outActors.Remove(owner);

		for (auto& actor : outActors)
		{
			Cast<AJHPCharacter>(actor)->togglePauseCharacter(isOn);
		}
	}
}