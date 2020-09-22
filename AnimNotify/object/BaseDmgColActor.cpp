// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDmgColActor.h"

#include "../../Character/JHPCharacter.h"
#include "../../Base/JHPPlayerController.h"
#include "../../Base/APStageGameState.h"

#include "Engine/World.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

namespace
{
	const int32 segment = 3;
	const float turbuleDist = 300.f;
	const float minDist = 200.f;
}

// Sets default values
ABaseDmgColActor::ABaseDmgColActor()
	:remainTime(1.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseDmgColActor::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseDmgColActor::whenOverlapped(AActor * otherActor, UPrimitiveComponent * otherComp)
{
	AJHPCharacter* ownerPtr = Cast<AJHPCharacter>(GetOwner());

	if (ownerPtr != nullptr)
	{
		AJHPCharacter* _target = Cast<AJHPCharacter>(const_cast<AActor*>(otherActor));

		if (ownerPtr->charType != _target->charType)
		{
			AAPStageGameState* curGS = Cast<AAPStageGameState>(ownerPtr->GetWorld()->GetGameState());


			lastProcess();
		}
	}
}

void ABaseDmgColActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	whenOverlapped(OtherActor, OtherComp);
}

const bool ABaseDmgColActor::giveDmg(const AActor * target, const float dmg)
{
	AJHPCharacter* _target = Cast<AJHPCharacter>(const_cast<AActor*>(target));
	AJHPCharacter* ownerPtr = Cast<AJHPCharacter>(GetOwner());

	if (_target != nullptr)
	{
		if (ownerPtr->charType != _target->charType)
		{
			//hitData.causer = ownerPtr;

			//_target->getDamaged(hitData, dmg, isHitBack, hitBackTime);
			
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		// temp method for fake destructable object
		UGameplayStatics::ApplyDamage(const_cast<AActor*>(target), dmg, nullptr, GetOwner(), nullptr);
		return true;
	}
}

ASimpleCol::ASimpleCol()
{
	PrimaryActorTick.bCanEverTick = false;

	sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("boxCol"));
	SetRootComponent(sphereCol);

	sphereCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereCol->SetCollisionObjectType(ECC_Pawn);
	sphereCol->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
	sphereCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Overlap);

	sphereCol->SetHiddenInGame(true);

	sphereCol->OnComponentBeginOverlap.AddDynamic(this, &ABaseDmgColActor::OnOverlapBegin);
}

void ASimpleCol::BeginPlay()
{
	Super::BeginPlay();

}

void ASimpleCol::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (flickeringTimer.IsValid())
	{
		UWorld* curWorld = GetWorld();

		if (curWorld)
		{
			curWorld->GetTimerManager().ClearTimer(flickeringTimer);
		}
	}
}

void ASimpleCol::doWhenSpawn()
{
	ABaseDmgColActor::doWhenSpawn();

	UWorld* curWorld = GetWorld();

	if (curWorld && flickeringTime > 0.f)
	{
		FTimerDelegate flicker;
		flicker.BindLambda([&]()
		{
			//UE_LOG(LogTemp, Log, TEXT("%s"), boxCol->IsCollisionEnabled() ? TEXT("true") : TEXT("false"));
			
			if (sphereCol != nullptr)
			{
				if (sphereCol->IsCollisionEnabled())
				{
					sphereCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
				else
				{
					sphereCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				}
			}
		});

		curWorld->GetTimerManager().SetTimer(flickeringTimer, flicker, flickeringTime, true);
	}
}

ALineTraceCol::ALineTraceCol()
	:Range(600.f)
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Default")));
}

void ALineTraceCol::doWhenSpawn()
{
	ABaseDmgColActor::doWhenSpawn();

	AJHPCharacter* ownerPtr = Cast<AJHPCharacter>(GetOwner());

	if (ownerPtr != nullptr)
	{
		const FVector startPoint = ownerPtr->GetActorLocation();
		const FVector endPoint = startPoint + (GetActorForwardVector() * Range);

		TArray<TEnumAsByte<EObjectTypeQuery>> objTypeQueryArray;
		objTypeQueryArray.Add(ownerPtr->getOppositeQueryType());

		FHitResult outHit;

		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetOwner(), startPoint, endPoint, objTypeQueryArray,
			false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, outHit, true, FColor::Red, FColor::Green, 1.f))
		{
			whenOverlapped(outHit.GetActor(), outHit.GetComponent());
		}
	}
	else
	{
		AActor* tmpOwner = GetOwner();

		if (tmpOwner != nullptr)
		{
			const FVector startPoint = tmpOwner->GetActorLocation();
			const FVector endPoint = startPoint + (GetActorForwardVector() * Range);

			TArray<TEnumAsByte<EObjectTypeQuery>> objTypeQueryArray;
			objTypeQueryArray.Add(EObjectTypeQuery::ObjectTypeQuery1);

			FHitResult outHit;

			UKismetSystemLibrary::LineTraceSingleForObjects(GetOwner(), startPoint, endPoint, objTypeQueryArray,
				false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, outHit, true, FColor::Red, FColor::Green, 1.f);
		}
	}
}

AProjCol::AProjCol()
	:idx(0)
{
	PrimaryActorTick.bCanEverTick = true;

	sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCol"));
	SetRootComponent(sphereCol);

	sphereCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereCol->SetCollisionObjectType(ECC_Pawn);
	sphereCol->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
	sphereCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Overlap);

	sphereCol->SetHiddenInGame(true);

	sphereCol->OnComponentBeginOverlap.AddDynamic(this, &ABaseDmgColActor::OnOverlapBegin);

	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM"));
	SM->SetupAttachment(GetRootComponent());
	SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movement"));
}

void AProjCol::BeginPlay()
{
	Super::BeginPlay();

}

void AProjCol::Tick(float DeltaTime)
{
	// has path to performant turbulence
	if (cachedNum > 0 && movement->HomingTargetComponent != nullptr)
	{
		if (FVector::DistSquared(GetActorLocation(), movement->HomingTargetComponent->GetComponentLocation()) <= minDist)
		{
			if (++idx >= cachedNum)
			{
				emptyPathsExcludeTarget();
				movement->HomingAccelerationMagnitude *= .8f;
			}
			else
			{
				movement->HomingTargetComponent = pathArray[idx];
			}
		}
	}
}

void AProjCol::lastProcess()
{
	emptyPathsExcludeTarget();

	Destroy();
}

void AProjCol::setProjectileProp(const bool _isHoming, const float _initSpd, const float _maxSpd, const float _hommingAccel, const float _gravityScale, const AActor* target)
{
	movement->bIsHomingProjectile = _isHoming;

	movement->InitialSpeed = _maxSpd;
	movement->MaxSpeed = _maxSpd;
	movement->HomingAccelerationMagnitude = _hommingAccel;
	movement->ProjectileGravityScale = _gravityScale;

	if (_isHoming && target)
	{
		movement->InitialSpeed = _initSpd;

		pathArray.Empty();

		const FVector curLoc = GetActorLocation();
		const FVector forwardVector = GetActorForwardVector();
		const float segDist = turbuleDist / segment;

		for (int32 i = 1; i <= segment; ++i)
		{
			USceneComponent* pathComp = NewObject<USceneComponent>(this, USceneComponent::StaticClass());
			pathComp->RegisterComponent();

			const FVector newLoc = UKismetMathLibrary::Quat_RotateVector(
				FRotator(UKismetMathLibrary::RandomFloatInRange(-10.f, 10.f),/*roll*/
					UKismetMathLibrary::RandomFloatInRange(-10.f, 10.f)/*yaw*/,
					0.f/*pitch*/).Quaternion()
				, forwardVector) 
				* (segDist * i);

			pathComp->SetWorldLocation(curLoc + newLoc);
			
			if (GetWorld())
				UKismetSystemLibrary::DrawDebugPoint(GetWorld(), curLoc + newLoc, 5.f, FColor::Blue, 1.f);

			pathArray.Add(pathComp);
		}

		pathArray.Add(target->GetRootComponent());

		cachedNum = pathArray.Num();

		movement->HomingTargetComponent = pathArray[idx];
	}
}

ATrailActor::ATrailActor()
{
	PrimaryActorTick.bCanEverTick = false;

	trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	SetRootComponent(trail);
}

void ATrailActor::setTrailActor(UParticleSystem * PSC_templete)
{
	trail->SetTemplate(PSC_templete);
	
	if (GetOwner())
	{
		AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);

		GetOwner()->OnDestroyed.AddDynamic(this, &ATrailActor::whenOwnerDestroyed);
	}
}

void ATrailActor::whenOwnerDestroyed(AActor* destroyedActor)
{
	SetLifeSpan(.5f);
}
