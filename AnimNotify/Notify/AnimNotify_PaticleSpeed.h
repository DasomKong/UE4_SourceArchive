// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PaticleSpeed.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTA_API UAnimNotify_PaticleSpeed : public UAnimNotify
{
	GENERATED_BODY()

	UAnimNotify_PaticleSpeed();
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNofity)
	UParticleSystem* particle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNofity)
	FTransform spawnTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNofity)
	float timeRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNofity)
	bool isAttached;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNofity)
	FName attachName;

};
