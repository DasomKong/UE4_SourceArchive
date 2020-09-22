// Fill out your copyright notice in the Description page of Project Settings.


#include "ghostDummy.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/AnimSequenceBase.h"

#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AghostDummy::AghostDummy()
	:timePassed(0.f), lifeTime(2.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	poseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	RootComponent = poseableMesh;
}

// Called when the game starts or when spawned
void AghostDummy::BeginPlay()
{
	Super::BeginPlay();

}

void AghostDummy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(_fadeTimer);
}

// Called every frame
void AghostDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (timePassed >= lifeTime)
	{
		if (doFadeOut)
		{
			if (!_fadeTimer.IsValid())
				GetWorldTimerManager().SetTimer(_fadeTimer, this, &AghostDummy::onFadeOut, DeltaTime, true);
		}
		else
			Destroy();
	}

	timePassed += DeltaTime;
}

void AghostDummy::SetParams(const FGhostData& _data, const FGhostMatData& _matData)
{
	lifeTime = _data.lifeTime;

	// set the skeletalmesh to pose
	poseableMesh->SetSkeletalMesh(_data.targetMesh->SkeletalMesh);
	poseableMesh->CopyPoseFromSkeletalComponent(_data.targetMesh);

	// set material for ghost
	MID = UMaterialInstanceDynamic::Create(_data.material, this);

	opacity = _matData.opa;

	MID->SetScalarParameterValue(FName(TEXT("opacity")),_matData.opa);
	MID->SetScalarParameterValue(FName(TEXT("ExponentIn")), _matData.exp);
	MID->SetVectorParameterValue(FName(TEXT("Emissive_Color")), _matData.color);

	UMaterialInterface* itfMaterial = Cast<UMaterialInterface>(MID);

	for (int i = 0; i < poseableMesh->GetMaterials().Num(); ++i)
		poseableMesh->SetMaterial(i, itfMaterial);

	doFadeOut = _data.doFade;

	// set timeline data has curve.
	if (_data.animSeq->RawCurveData.FloatCurves.Num() > 0)
	{
		// not used now
	}
}

void AghostDummy::onFadeOut()
{
	opacity -= this->GetWorld()->DeltaTimeSeconds;

	MID->SetScalarParameterValue(FName(TEXT("opacity")), opacity);

	if(opacity <= 0.f)
		Destroy();
}

