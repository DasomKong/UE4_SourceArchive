// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SpawnDummy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UAnimNotifyState_SpawnDummy : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform actorTransform;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> dummyActorClass;

	UPROPERTY()
	AActor* dummy;
};
