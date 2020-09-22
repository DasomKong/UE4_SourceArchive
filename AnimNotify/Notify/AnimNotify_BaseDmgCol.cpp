// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_BaseDmgCol.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

#include "../object/BaseDmgColActor.h"

#include "../../Character/JHPCharacter.h"

void UAnimNotify_SimpleCol::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const FTransform spawnTransform = transform * MeshComp->GetSocketTransform(FName(TEXT("None")));

	auto spawnActor = Cast<ALineTraceCol>(UGameplayStatics::BeginDeferredActorSpawnFromClass(MeshComp->GetOwner(), ALineTraceCol::StaticClass(), spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, MeshComp->GetOwner()));

	if (spawnActor != nullptr)
	{
		if (isAttachedToOwner)
		{
			spawnActor->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, attachName);
		}

		spawnActor->remainTime = remainTime;

		spawnActor->battleData = battleData;

		UGameplayStatics::FinishSpawningActor(spawnActor, spawnTransform);

		spawnActor->doWhenSpawn();
	}
}

void UAnimNotify_TraceCol::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const FTransform spawnTransform = transform * MeshComp->GetSocketTransform(FName(TEXT("None")));

	auto spawnActor = Cast<ALineTraceCol>(UGameplayStatics::BeginDeferredActorSpawnFromClass(MeshComp->GetOwner(), ALineTraceCol::StaticClass(), spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, MeshComp->GetOwner()));

	if (spawnActor != nullptr)
	{
		spawnActor->Range = Range;
		spawnActor->remainTime = remainTime;
		
		spawnActor->battleData = battleData;

		UGameplayStatics::FinishSpawningActor(spawnActor, spawnTransform);

		spawnActor->doWhenSpawn();
	}
}

void UAnimNotify_ProjCol::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (SocketName.Num() < 1)
		return;

	AActor* tmpOwner = MeshComp->GetOwner();

	AActor* tmpHomming = nullptr;

	if (isHomming)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> objTypeQueryArray;

		{
			AJHPCharacter* ownerPtr = Cast<AJHPCharacter>(tmpOwner);
			
			if (ownerPtr)
			{
				objTypeQueryArray.Add(ownerPtr->getOppositeQueryType());
			}
			else
			{
				objTypeQueryArray.Add(EObjectTypeQuery::ObjectTypeQuery2);
			}
		}

		const FVector startPoint = tmpOwner->GetActorLocation();

		const FVector endPoint = startPoint + ((MeshComp->GetComponentTransform() * transform).GetRotation().GetForwardVector() * range);

		FHitResult outHit;

		if (UKismetSystemLibrary::LineTraceSingleForObjects(tmpOwner, startPoint, endPoint, objTypeQueryArray,
			false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, outHit, true, FColor::Red, FColor::Green, 1.f))
		{
			tmpHomming = outHit.GetActor();
		}
	}

	for (auto& names : SocketName)
	{
		const FTransform spawnTransform = MeshComp->GetSocketTransform(names);

		auto spawnActor = Cast<AProjCol>(UGameplayStatics::BeginDeferredActorSpawnFromClass(MeshComp->GetOwner(), AProjCol::StaticClass(), spawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, MeshComp->GetOwner()));

		if (spawnActor != nullptr)
		{
			spawnActor->remainTime = remainTime;
			spawnActor->battleData = battleData;


			spawnActor->SM->SetStaticMesh(SM);

			spawnActor->setProjectileProp(isHomming, initSpd, maxSpd, HomingAcc, gravityScale, tmpHomming);

			UGameplayStatics::FinishSpawningActor(spawnActor, spawnTransform);

			spawnActor->doWhenSpawn();
		}

		auto trailActor = Cast<ATrailActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(MeshComp->GetOwner(), ATrailActor::StaticClass(), FTransform::Identity, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, spawnActor));

		if (trailActor != nullptr)
		{
			trailActor->setTrailActor(trail);

			UGameplayStatics::FinishSpawningActor(trailActor, FTransform::Identity);
		}

	}
}
