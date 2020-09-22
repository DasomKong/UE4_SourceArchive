// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNofityState_ToggleWpCol.h"

#include "../../Character/JHPCharacter.h"

void UAnimNofityState_ToggleWpCol::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	 TWeakObjectPtr<AJHPCharacter> target = Cast<AJHPCharacter>(MeshComp->GetOwner());

	 if (target.IsValid())
	 {
		 target.Get()->setAtkCol(true, true);
	 }
}

void UAnimNofityState_ToggleWpCol::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (flickeringTime > 0.f)
	{
		if (curTime > flickeringTime)
		{
			// toggle state
			TWeakObjectPtr<AJHPCharacter> target = Cast<AJHPCharacter>(MeshComp->GetOwner());

			if (target.IsValid())
			{
				// just toggle
				target.Get()->setAtkCol();
			}

			curTime = 0.f;
		}
		else
		{
			curTime += FrameDeltaTime;
		}
	}
}

void UAnimNofityState_ToggleWpCol::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	TWeakObjectPtr<AJHPCharacter> target = Cast<AJHPCharacter>(MeshComp->GetOwner());

	if (target.IsValid())
	{
		target.Get()->setAtkCol(true, false);
	}
}
