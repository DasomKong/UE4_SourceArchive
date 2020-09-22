// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_PlaySpeed.h"

#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

namespace JHPUtils
{
	const FName propName = "RateScale";
}

void UAnimNotifyState_PlaySpeed::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	FSmartName CurveName;
	
	UAnimMontage* montage = Cast<UAnimMontage>(Animation);
	
	if (montage && montage->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, JHPUtils::propName, CurveName))
	{
		const FRawCurveTracks& Curves = montage->GetCurveData();
		const FFloatCurve* ScaleCurve = static_cast<const FFloatCurve*>(Curves.GetCurveData(CurveName.UID));

		if (ScaleCurve != nullptr)
		{
			const float pos = MeshComp->GetAnimInstance()->Montage_GetPosition(montage);
			float newRate = ScaleCurve->Evaluate(pos);

			if (newRate <= 0.005f)
				newRate = 0.005f;

			MeshComp->GetAnimInstance()->Montage_SetPlayRate(montage, newRate);
		}
	}
}

void UAnimNotifyState_PlaySpeed::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// refresh to default
	//Animation->RateScale = 1.f;
}
