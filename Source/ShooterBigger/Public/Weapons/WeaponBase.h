

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS(Abstract)
class SHOOTERBIGGER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	// Getting TextureWeaponBody
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Texture")
	FORCEINLINE UTexture2D* GetTextureWeaponBody() const { return this->TextureWeaponBody; }

	// Getting TextureWeaponMagazine
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Texture")
	FORCEINLINE UTexture2D* GetTextureWeaponMagazine() const { return this->TextureWeaponMagazine; }

	// Getting TextureWeaponIronsights
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Texture")
	FORCEINLINE UTexture2D* GetTextureWeaponIronsights() const { return this->TextureWeaponIronsights; }

	// Getting Rate of fire
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Data")
	FORCEINLINE float GetRateOfFire() const { return this->RateOfFire; }

	// Is empty ammo in climb ?
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Data")
	FORCEINLINE bool IsEmptyAmmoClimb() const { return this->AmmunitionCurrent == 0; }

	// Get Ammunition Max
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Data")
	FORCEINLINE int32 GetAmmunitionMax() const { return this->AmmunitionMax; }

	// Get Ammunition Current
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Data")
	FORCEINLINE int32 GetAmmunitionCurrent() const { return this->AmmunitionCurrent; }

	// Is Automatic?
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Data")
	FORCEINLINE bool IsWeaponAutomatic() const { return this->bAutomatic; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Base mesh weapon
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* MeshWeapon;

	// Mesh Ironsights
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshIronsights;

	// Mesh Magazine
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshMagazine;

	// Should this Weapon be automatic?
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	bool bAutomatic;

	// Amount of rounds per minute that the Weapon can fire.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	float RateOfFire;

	// Maximum ammunition that the Weapon's magazine can hold.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	uint16 AmmunitionMax;

	// Firing Montage.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animation")
	UAnimMontage* MontageFire;

	// Reload Montage.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animation")
	UAnimMontage* MontageReload;

	// Reload Empty Montage. Played when reloading without any leftover ammunition.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animation")
	UAnimMontage* MontageReloadEmpty;

	// Texture displayed in the player's heads up display to represent this Weapon's shape/body.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Display")
	UTexture2D* TextureWeaponBody;

	// Texture displayed in the player's heads up display to represent this Weapon's magazine.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Display")
	UTexture2D* TextureWeaponMagazine;

	// Texture displayed in the player's heads up display to represent this Weapon's ironsights.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Display")
	UTexture2D* TextureWeaponIronsights;

	// Result from hitting something when firing.
	FHitResult HitResult;

	// Current amount of bullets in the magazine.
	uint16 AmmunitionCurrent;
};
