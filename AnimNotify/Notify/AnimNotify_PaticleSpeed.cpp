// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PaticleSpeed.h"

#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Particles/ParticleSystem.h"
#include "Animation/AnimMontage.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UAnimNotify_PaticleSpeed::UAnimNotify_PaticleSpeed()
	:particle(nullptr), spawnTransform(FTransform::Identity), timeRate(1.f), isAttached(false), attachName(NAME_None)
{}

void UAnimNotify_PaticleSpeed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (particle != nullptr && MeshComp->GetOwner())
	{
		UParticleSystemComponent* result;

		if (isAttached)
		{
			result = UGameplayStatics::SpawnEmitterAttached(particle, MeshComp, attachName, spawnTransform.GetLocation(), spawnTransform.GetRotation().Rotator(), spawnTransform.GetScale3D());
		}
		else
		{
			FTransform outTransform;

			{
				const FTransform transformOffset = MeshComp->GetSocketTransform(FName(TEXT("None")));
				const FTransform locTransform = FTransform(spawnTransform.GetLocation());
				FTransform::Multiply(&outTransform, &locTransform, &transformOffset);
			}

			result = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), particle, outTransform.GetLocation(), spawnTransform.GetRotation().Rotator(), spawnTransform.GetScale3D());
		}

		if (result->IsValidLowLevel())
		{
			// timer to tick
			FTimerHandle TH_ParticleTime;
			FTimerDelegate TD;

			TD.BindLambda([&](UParticleSystemComponent* psc, USkeletalMeshComponent* skComp, UAnimSequenceBase* animSeq)
			{
				if (!psc->IsValidLowLevel())
				{
					if (skComp && skComp->GetOwner() && skComp->GetOwner()->GetWorld())
					{
						skComp->GetOwner()->GetWorldTimerManager().ClearTimer(TH_ParticleTime);
					}
				}
				else
				{
					FSmartName CurveName;

					UAnimMontage* montage = Cast<UAnimMontage>(animSeq);

					if (MeshComp && montage && montage->GetSkeleton()->GetSmartNameByName(USkeleton::AnimCurveMappingName, FName(TEXT("RateScale")), CurveName))
					{
						const FRawCurveTracks& Curves = montage->GetCurveData();
						const FFloatCurve* ScaleCurve = static_cast<const FFloatCurve*>(Curves.GetCurveData(CurveName.UID));

						if (ScaleCurve != nullptr)
						{
							const float pos = skComp->GetAnimInstance()->Montage_GetPosition(montage);
							float newRate = ScaleCurve->Evaluate(pos);

							if (newRate <= 0)
								newRate = 0.1f;

							psc->CustomTimeDilation = newRate;
						}
					}
				}

			}, result, MeshComp, Animation);

			MeshComp->GetWorld()->GetTimerManager().SetTimer(TH_ParticleTime, TD, MeshComp->GetWorld()->GetDeltaSeconds(), true);
		}
	}
}