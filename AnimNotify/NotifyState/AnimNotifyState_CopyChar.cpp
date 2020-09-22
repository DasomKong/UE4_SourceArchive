// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_CopyChar.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


UAnimNotifyState_CopyChar::UAnimNotifyState_CopyChar()
	:TagName(TEXT("copyComp"))
{}

void UAnimNotifyState_CopyChar::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Received_NotifyBegin(MeshComp, Animation, TotalDuration);

	SpawnCopy(MeshComp);
}

void UAnimNotifyState_CopyChar::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Received_NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_CopyChar::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Received_NotifyEnd(MeshComp, Animation);

	DeleteCopy(MeshComp);
}

void UAnimNotifyState_CopyChar::SpawnCopy(USkeletalMeshComponent * MeshComp)
{
	if (material == nullptr)
		return;

	AActor* tmpOwner = MeshComp->GetOwner();

	if (MeshComp->GetOwner())
	{
		USkeletalMeshComponent* copyComp = NewObject<USkeletalMeshComponent>(tmpOwner, USkeletalMeshComponent::StaticClass());

		copyComp->RegisterComponent();
		copyComp->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform);

		copyComp->SetSkeletalMesh(MeshComp->SkeletalMesh);

		UMaterialInterface* tmpMaterial = Cast<UMaterialInterface>(material);

		for (int i = 0; i < MeshComp->GetNumMaterials(); ++i)
		{
			copyComp->SetMaterial(i, tmpMaterial);
		}

		copyComp->SetMasterPoseComponent(MeshComp);

		copyComp->ComponentTags.Add(TagName);
	}

	
}

void UAnimNotifyState_CopyChar::DeleteCopy(USkeletalMeshComponent * MeshComp)
{
	if (MeshComp->GetOwner())
	{
		TArray<UActorComponent*> _array;

		_array = MeshComp->GetOwner()->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), TagName);

		if (_array.Num() > 0)
		{
			for (auto& comp : _array)
			{
				comp->DestroyComponent();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Can't find CopyComp."));
		}
	}
}
