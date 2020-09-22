// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "../../JHPTypes.h"

#include "AnimNotify_BaseDmgCol.generated.h"

/**
 *
 */
UCLASS()
class PROJECTA_API UAnimNotify_BaseDmgCol : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_BaseDmgCol()
		:remainTime(.033f)
	{
#if WITH_EDITORONLY_DATA
		NotifyColor = FColor(255, 0, 255, 255);
#endif // WITH_EDITORONLY_DATA
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_DmgCol)
		FTransform transform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_DmgCol)
		float remainTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_DmgCol)
	FBattleData battleData;
};

UCLASS()
class PROJECTA_API UAnimNotify_SimpleCol : public UAnimNotify_BaseDmgCol
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);


	UAnimNotify_SimpleCol()
		:isAttachedToOwner(false), attachName(NAME_None), flickeringTime(0.f)
	{}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = AnimNoti_Melee)
		uint8 isAttachedToOwner : 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = AnimNoti_Melee)
		FName attachName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = AnimNoti_Melee)
		float flickeringTime;
};

UCLASS()
class PROJECTA_API UAnimNotify_TraceCol : public UAnimNotify_BaseDmgCol
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Trace)
		float Range;
};

UCLASS()
class PROJECTA_API UAnimNotify_ProjCol : public UAnimNotify_BaseDmgCol
{
	GENERATED_BODY()

		UAnimNotify_ProjCol()
		:SM(nullptr), isHomming(false), initSpd(100.f), maxSpd(3000.f), HomingAcc(1000.f), gravityScale(1.f), range(1000.f), trail(nullptr)
	{}

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		class UStaticMesh* SM;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		TArray<FName> SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		bool isHomming;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		float initSpd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		float maxSpd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		float HomingAcc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		float gravityScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		float range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimNoti_Proj)
		class UParticleSystem* trail;
};