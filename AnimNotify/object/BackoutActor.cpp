// Fill out your copyright notice in the Description page of Project Settings.


#include "BackoutActor.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ABackoutActor::ABackoutActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	skySphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("skySphere"));
	RootComponent = skySphereComp;
	
	UMaterialInstance* _material = LoadObject<UMaterialInstance>(NULL, TEXT("MaterialInstanceConstant'/Game/art/Material/M_Backout_Inst.M_Backout_Inst'"));
	UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"));

	UMaterialInterface* itfMaterial = Cast<UMaterialInterface>(_material);

	skySphereComp->SetStaticMesh(mesh);
	skySphereComp->SetMaterial(0, itfMaterial);
}



//
//// Called when the game starts or when spawned
//void ABackoutActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ABackoutActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
