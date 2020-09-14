// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "../JHPTypes.h"

#include "CommandHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_CommandMatched, const int32, cmdID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_ActionKeyInput, EJHPInputKey, Key);

class UInputSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UCommandHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCommandHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = Command)
	void mapInputSettings(UInputSettings* inputSettings);

	void setUsableCommandKeys();

private:
	void updateKeyArray(const float deltaTime);
	bool checkMatchingCommand(int32& matchedKeyID, bool isForward);
	void parseCurrentFrameKey(void);

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Command)
	FDele_CommandMatched CommandMatched;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Command)
	FDele_ActionKeyInput ActionKeyInput;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Command, meta =(AllowPrivateAccess="true"))
	TArray<EJHPInputKey> storeKeyArray;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Command, meta = (AllowPrivateAccess = "true"))
	float maxWaitTime;
	
	UPROPERTY(Transient)
	TMap<FKey, EJHPInputKey> reMappedAxisKey;

	UPROPERTY(Transient)
	TMap<FKey, EJHPInputKey> reMappedActionKey;

	UPROPERTY(Transient)
	FCommandMap commandMap;
	
	// usable cmd keys are static and common
	TMap<int32, FCommandKeyArray> usableCommandMap;
	int32 maxInputCount;

	APlayerController* controller;

	bool cmdStarted;

	float passedTime;
};
