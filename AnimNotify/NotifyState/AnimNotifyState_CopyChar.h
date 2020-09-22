// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CopyChar.generated.h"

class UMaterialInstance;

/**
 * 
 */
UCLASS()
class PROJECTA_API UAnimNotifyState_CopyChar : public UAnimNotifyState
{
	GENERATED_BODY()

	UAnimNotifyState_CopyChar();

public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	void SpawnCopy(USkeletalMeshComponent * MeshComp);
	void DeleteCopy(USkeletalMeshComponent * MeshComp);

public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category="JHP")
		UMaterialInstance* material;

private:
	const FName TagName;
};
