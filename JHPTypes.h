#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "Engine/DataTable.h"

#include "JHPTypes.generated.h"

class AJHPCharacter;

/*
type defines
*/

typedef TArray<EJHPInputKey> FCommandKeyArray;

/*
	Char Type
*/

UENUM(BlueprintType)
enum class ECharType : uint8
{
	PLAYER,
	ENEMY,
	END
};

/*
	Weapon
*/

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NONE,
	SCYTHESWORD,
};

/*
	Command
*/

UENUM(BlueprintType)
// look at right default. R == FORWARD
enum class EJHPInputKey : uint8
{
	NEUTRAL = 0 UMETA(DisplayName = "N"),
	LEFT = 1 UMETA(DisplayName = "L"),
	RIGHT = 5 UMETA(DisplayName = "R"),
	UP = 7 UMETA(DisplayName = "U"),
	DOWN = 13 UMETA(DisplayName = "D"),

	L_UP = 8 UMETA(DisplayName = "LU"),
	R_UP = 12 UMETA(DisplayName = "RU"),
	L_DOWN = 14 UMETA(DisplayName = "LD"),
	R_DOWN = 18 UMETA(DisplayName = "RD"),

	KEY_A = 128 UMETA(DisplayName = "Atk"),
	KEY_B = 255 UMETA(DisplayName = "Jump")
};

UENUM(BlueprintType)
enum class EDmgType : uint8
{
	NORMAL,
	HUGE,
	DOWN,
	AIRBORNE,
	GRAB,
	RELEASE
};

UENUM(BlueprintType)
enum class EHittedState : uint8
{
	NORMAL,
	GROGGY,
	KNOCKBACK,

	// for air
	AIR_START,
	AIR_FALL,
	HITONAIR,
	DOWN,

	// for grab
	GRABBED,
	GRAB_END,

	END
};


/*
	Character Enums
*/
UENUM(BlueprintType)
enum class ECharState : uint8
{
	IDLE,
	JUMP,
	HITTED,
	DEAD,
	ACTING,
	END
};

/*
if you want to add new skill commands pool, add cmdMap
*/

USTRUCT(Atomic, BlueprintType)
struct FActingData
{
	GENERATED_BODY()

public:
	FActingData()
		:animToPlay(nullptr), needAnimRefresh(true), requiredManaRate(0.f), afterDelay(0.f), finishedState(ECharState::IDLE), actLevel(0),
		canRotate(false), canJump(false), canCancle(false)
	{
		conditionToExecute.Add(ECharState::IDLE);
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Acting)
	TArray<FName> ChildrenID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Acting)
		class UAnimMontage* animToPlay;

	UPROPERTY(EditDefaultsOnly, Category = Acting)
		bool needAnimRefresh;
	
	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = Acting)
		float requiredManaRate;

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = Acting)
	float afterDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Acting)
		TArray<ECharState> conditionToExecute;

	// this can be changed by components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Acting)
	ECharState finishedState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Acting)
		int32 actLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Acting)
		TArray<TSubclassOf<class UActingComp>> actingCompClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Acting)
		uint8 canRotate : 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Acting)
		uint8 canJump : 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Acting)
		uint8 canCancle : 1;

	UPROPERTY()
	TArray<class UActingComp*> actingComps;
};

USTRUCT(BlueprintType, Atomic)
struct FCommandArray
{
	GENERATED_BODY()

		FCommandArray() {}
	FCommandArray(EJHPInputKey inInputArray[], const int _size)
		:cmdID(0)
	{
		for (int i = 0; i < _size; ++i)
		{
			commandArray.Add(inInputArray[i]);
			cmdID += static_cast<int32>(inInputArray[i]);
		}
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Command)
		TMap<FName, FActingData> skills;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Command)
		TArray<EJHPInputKey> commandArray;

	UPROPERTY(BlueprintReadOnly, Category = Command)
	int32 cmdID;
};

USTRUCT(BlueprintType, Atomic)
struct FCommandMap : public FTableRowBase
{
	GENERATED_BODY()

		FCommandMap() {

		// ATK
		EJHPInputKey a[]{ EJHPInputKey::KEY_A };
		cmdArray.Add(FCommandArray(a, 1));

		// FORWARD DASH
		EJHPInputKey fd[]{ EJHPInputKey::RIGHT, EJHPInputKey::RIGHT };
		cmdArray.Add(FCommandArray(fd, 2));

		// BACKWARD DASH
		EJHPInputKey bd[]{ EJHPInputKey::LEFT, EJHPInputKey::LEFT };
		cmdArray.Add(FCommandArray(bd, 2));

		// UPPER ATK
		EJHPInputKey ua[]{ EJHPInputKey::UP, EJHPInputKey::KEY_A };
		cmdArray.Add(FCommandArray(ua, 2));

		// DOWN ATK
		EJHPInputKey da[]{ EJHPInputKey::DOWN, EJHPInputKey::KEY_A };
		cmdArray.Add(FCommandArray(da, 2));

		// DOWN RIGHT ATK
		EJHPInputKey dra[]{ EJHPInputKey::DOWN, EJHPInputKey::RIGHT, EJHPInputKey::KEY_A };
		cmdArray.Add(FCommandArray(dra, 3));

		// DOWN JUMP
		EJHPInputKey dj[]{ EJHPInputKey::DOWN, EJHPInputKey::KEY_B };
		cmdArray.Add(FCommandArray(dj, 2));
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Command)
		TArray<FCommandArray> cmdArray;

};

USTRUCT(Atomic, BlueprintType)
struct FWeaponSkillData : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponSkillData()
	{}

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		FCommandMap cmdMoves;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TArray<FActingData> pressSkill;
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

		FWeaponData()
		:wpType(EWeaponType::NONE), dmg(1.f)
	{}

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Weapon")
		UTexture2D* wpThumbnail;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Weapon")
		EWeaponType wpType;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Weapon")
		float dmg;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Weapon")
		class USkeletalMesh* SKMesh;
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterSkill
{
	GENERATED_BODY()

	FCharacterSkill()
		:coolTime(0.f), curCoolTime(0.f), isCooling(false), range(0.f)
	{}
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FActingData skill;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	float coolTime;

	float curCoolTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isCooling;

	UPROPERTY(Editanywhere, BlueprintReadOnly)
	float range;

};

USTRUCT(Atomic, BlueprintType)
struct FCharacterSkillData : public FTableRowBase
{
	GENERATED_BODY()

	FCharacterSkillData()
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SkillData)
	TMap<FName, FCharacterSkill> skills;
};

USTRUCT(Atomic, BlueprintType)
struct FHitData
{
	GENERATED_BODY()

public:
	FHitData()
		:causer(nullptr), dmgType(EDmgType::NORMAL), forwardImpactPower(0.f), upwardImpactPower(0.f), hitResult(EHittedState::END)
	{}

	FHitData(AActor* _causer, EDmgType _dmgType, const float _forwardImpactPower = 0.f, const float _upwardImpactPower = 0.f)
		:causer(_causer), dmgType(_dmgType), forwardImpactPower(_forwardImpactPower), upwardImpactPower(_upwardImpactPower)
	{}

	UPROPERTY(BlueprintReadWrite)
		AActor* causer;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		EDmgType dmgType;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		float forwardImpactPower;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		float upwardImpactPower;

	UPROPERTY(BlueprintReadOnly)
		EHittedState hitResult;
};

USTRUCT(Atomic, BlueprintType)
struct FCamShakeData
{
	GENERATED_BODY()

	public:
		FCamShakeData()
			:shouldShake(false), camShake(nullptr), shakeScale(1.f)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 shouldShake:1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraShake> camShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float shakeScale;
};
