#pragma once
#include "Weapons/WeaponBase.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDataTypes.generated.h"

// The basic state move of the character
UENUM()
enum class EStateMoveCharacter : uint8
{
	None = 0,
	Idle,
	Walk,
	Running
};

// Signature to change the state of the move character
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateMoveCharacterChangedSignature, EStateMoveCharacter);

// The state of the player when aiming or from the hip
UENUM()
enum class EStateAim : uint8
{
	None = 0,
	Hip,
	Aiming
};

// Signature to change the state of the Aim
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateAimChangedSignature, EStateAim);

// The state of the weapon that is in the pseudo inventory
UENUM()
enum class EStateWeapon : uint8
{
	None = 0,
	Pistol,
	Rifle
};

// Signature to change the state of the weapon
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateWeaponChangedSignature, EStateWeapon);

// State of continuous action animations montage
UENUM()
enum class EStateAction : uint8
{
	None = 0,
	Inspecting,
	Reloading,
	Holstering,
	Fire
};

// Signature to change the state of the action
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateActionChangedSignature, EStateAction);

// A structure for storing data about weapons in a pseudo inventory
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	// Type of Weapon used by this Character.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> TypeWeapon;

	// Montage played when the character fires their weapon.
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MontageFire;

	// Montage played when the character inspects a weapon.
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MontageInspect;

	// Montage played when the character reloads.
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MontageReload;

	// Montage played when the character reloads while having no ammo in their weapon's magazine.
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MontageReloadEmpty;

	// Animation Montage played when trying to holster the character's weapon.
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MontageHolster;

	// Animation Montage played when the character is unholstering their weapon.
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MontageUnholster;

	// Cross hair on weapon
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CrossHairWidget;
	
	// To string information about weapon
	FORCEINLINE FString ToString() const
	{
		return FString("Type weapon: " + TypeWeapon->GetName() + " | Montage Fire: " + MontageFire->GetName() +
					   " | Montage Inspect: " + MontageInspect->GetName() + " | Montage Reload: " + MontageReload->GetName() +
					   " | Montage Reload Empty: " + MontageReloadEmpty->GetName() + " | Montage Holster: " + MontageHolster->GetName() +
					   " | Montage Unholster: " + MontageUnholster->GetName());
	}
};
