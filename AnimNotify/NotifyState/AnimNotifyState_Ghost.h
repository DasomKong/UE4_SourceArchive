// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Ghost.generated.h"

class AActor;
class UMaterialInstance;
/**
 *
 */
UCLASS()
class PROJECTA_API UAnimNotifyState_Ghost : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_Ghost();

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	// for get notified property changed
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	/*
	spawn ghost actor with input skeletalmesh
	*/
	void SpawnGhost(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation);
	void SetNatrualSpawnCountAndTimeRate(float TotalDuration);
public:
	// spawn rate in 1 sec
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int spawnCount;
	// object remaining time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LifeTime;


	// for ghost material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMaterialInstance* _material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float paramOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float paramExponentIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor colorParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool doFade;

private:
	float passedTime;
	float SpawnSec;
	int RealCount;
};