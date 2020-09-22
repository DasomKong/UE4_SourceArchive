// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackoutActor.generated.h"

UCLASS()
class PROJECTA_API ABackoutActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackoutActor();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
private:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ScreenEffect, meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* skySphereComp;

};
