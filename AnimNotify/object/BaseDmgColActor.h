// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../../JHPTypes.h"

#include "BaseDmgColActor.generated.h"

UCLASS()
class PROJECTA_API ABaseDmgColActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseDmgColActor();

	virtual void BeginPlay() override;

	void whenOverlapped(AActor* otherActor, UPrimitiveComponent* otherComp);

	virtual void doWhenSpawn() {
		SetLifeSpan(remainTime);
	}

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	const bool giveDmg(const AActor* target, const float dmg);
	virtual void lastProcess(){}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DmgCol)
	float remainTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DmgCol)
	FBattleData battleData;
};

UCLASS()
class PROJECTA_API ASimpleCol: public ABaseDmgColActor
{
	GENERATED_BODY()

public:
	ASimpleCol();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void doWhenSpawn() override;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = SimpleCol)
	class USphereComponent* sphereCol;

	UPROPERTY(BlueprintReadOnly, Category = SimpleCol)
	float flickeringTime;

	UPROPERTY(BlueprintReadOnly, Category = SimpleCol)
	bool isAttachedToOwner;

	UPROPERTY(BlueprintReadOnly, Category = SimpleCol)
	FName attachName;

private:
	FTimerHandle flickeringTimer;

};

UCLASS()
class PROJECTA_API ALineTraceCol : public ABaseDmgColActor
{
	GENERATED_BODY()

public:
	ALineTraceCol();

	virtual void doWhenSpawn() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = LineTraceCol)
	float Range;
};

UCLASS()
class PROJECTA_API AProjCol : public ABaseDmgColActor
{
	GENERATED_BODY()

public:
	AProjCol();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void lastProcess() override;

	void setProjectileProp(const bool _isHoming, const float _initSpd, const float _maxSpd, const float _hommingAccel, const float _gravityScale, const AActor* target);

private:
	void emptyPathsExcludeTarget(void)
	{
		if (pathArray.Num() > 0)
		{
			for (int32 i = 0; i < pathArray.Num() - 1; ++i)
			{
				pathArray[i]->DestroyComponent();
			}
			pathArray.Empty();
		}
	}

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ProjCol)
	class USphereComponent* sphereCol;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ProjCol)
	class UStaticMeshComponent* SM;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ProjCol)
	class UProjectileMovementComponent* movement;

	UPROPERTY()
	TArray<class USceneComponent*> pathArray;

	int32 cachedNum;
	int32 idx;
};

UCLASS()
class PROJECTA_API ATrailActor : public AActor
{
	GENERATED_BODY()

public:
	ATrailActor();

	void setTrailActor(class UParticleSystem* PSC_templete);

	UFUNCTION()
	void whenOwnerDestroyed(AActor* destroyedActor);

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = TrailActor)
	class UParticleSystemComponent* trail;

};