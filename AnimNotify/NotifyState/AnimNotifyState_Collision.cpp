// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Collision.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"


void UAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	// ���� �����ϴ� ���.
	// Ȥ�� �ƿ� ó������ ���̽� ĳ���Ͱ� �浹ü�� ������ �ִ� �����.. Ư�� �±� ������Ʈ ã�Ƽ� ������Ʈ.
	AActor* tmpActor = MeshComp->GetOwner();
	
	if (targetComp == nullptr && tmpActor)
	{
		targetComp = NewObject<USphereComponent>(tmpActor);

		if (targetComp)
		{
			targetComp->SetSphereRadius(Radius);
			targetComp->AttachToComponent(MeshComp->GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);

			targetComp->SetGenerateOverlapEvents(true);

			targetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			targetComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

			targetComp->RegisterComponent();
		}
	}
		
}

void UAnimNotifyState_Collision::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
}

void UAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	/*if (targetComp)
	{
		targetComp->DestroyComponent();
		targetComp = nullptr;
	}*/
}
