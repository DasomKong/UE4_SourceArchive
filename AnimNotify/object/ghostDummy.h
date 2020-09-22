// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "ghostDummy.generated.h"

class UPoseableMeshComponent;
class UMaterialInstanceDynamic;
class USkeletalMeshComponent;
class UAnimSequenceBase;
class UMaterialInstance;

struct FGhostData
{
	FGhostData(){}
	FGhostData(UMaterialInstance* _material, USkeletalMeshComponent* _targetMesh, UAnimSequenceBase* _animSeq, float _lifeTime, bool _doFade)
		:material(_material), targetMesh(_targetMesh), animSeq(_animSeq), lifeTime(_lifeTime), doFade(_doFade)
	{}

	class UMaterialInstance* material;
	class USkeletalMeshComponent* targetMesh;
	class UAnimSequenceBase* animSeq;
	float lifeTime;
	bool doFade;
};

struct FGhostMatData
{
	FGhostMatData(const float& _opa, const float& _exp, const FLinearColor& _color)
		:opa(_opa), exp(_exp), color(_color)
	{}

	float opa;
	float exp;
	FLinearColor color;
};

UCLASS()
class PROJECTA_API AghostDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AghostDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetParams(const FGhostData& _data, const FGhostMatData& _matData);

	void onFadeOut();

	// for curves timeline callback
	/*UFUNCTION()
		void onTimeCallbackFloat(float value);

	UFUNCTION()
		void onTimeCallbackFinished();*/

private:
	FTimeline timeLine;
	class UPoseableMeshComponent* poseableMesh;
	class UMaterialInstanceDynamic* MID;

	FTimerHandle _fadeTimer;

	float timePassed;
	float lifeTime;

	bool doFadeOut;
	float opacity;
};
