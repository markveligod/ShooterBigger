

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

#pragma region GetData
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
#pragma endregion

	// Shooting
	UFUNCTION(BlueprintCallable, Category = "AWeaponBase|Action")
	void MakeShot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region Components
	// Base mesh weapon
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* MeshWeapon;

	// Mesh Ironsights
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshIronsights;

	// Mesh Magazine
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshMagazine;

#pragma endregion

#pragma region WeaponSettings
	// Should this Weapon be automatic?
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	bool bAutomatic;

	// Amount of rounds per minute that the Weapon can fire.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings", meta = (EditCondition = "bAutomatic"))
	float RateOfFire;

	// Maximum ammunition that the Weapon's magazine can hold.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	int32 AmmunitionMax;

	// Distance of the shot
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings", meta = (Units = "cm"))
	float DistanceShot = 10000.0f;

	// The amount of damage dealt
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	float AmountDamage = 1.0f;

	// Damage type weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Settings")
	FPointDamageEvent PointDamageInfo;

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

	// Spawn effect Emitter for weapons
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | VFX")
	UParticleSystem* EmitterEffect;

	// Socket muzzle for spawn effect Emitter of weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | VFX")
	FName SocketMuzzle = "SOCKET_Muzzle";

	// Correction for the angle of rotation if necessary
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | VFX", meta = (Units = "deg"))
	FRotator DeltaMuzzleRot = FRotator::ZeroRotator;
#pragma endregion

#pragma region DebugTraceWeapon
	// [Debug] Enable debug shot ?
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug")
	bool bEnableDebugShot = true;

	// [Debug] color trace
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug", meta = (EditCondition = "bEnableDebugShot"))
	FColor ColorTrace = FColor::Green;

	// [Debug] color hit point
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug", meta = (EditCondition = "bEnableDebugShot"))
	FColor ColorHit = FColor::Red;

	// [Debug] Line thickness
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug", meta = (EditCondition = "bEnableDebugShot"))
	float Thickness = 2.0f;

	// [Debug] Draw time line
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug", meta = (EditCondition = "bEnableDebugShot"))
	float DrawTime = 5.0f;

	// [Debug] Line radius Hit
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug", meta = (EditCondition = "bEnableDebugShot"))
	float RadiusHit = 12.0f;

	// [Debug] Segments hit
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Debug", meta = (EditCondition = "bEnableDebugShot"))
	int32 SegmentsHit = 8;

#pragma endregion

	// Result from hitting something when firing.
	FHitResult HitResult;

	// Current amount of bullets in the magazine.
	int32 AmmunitionCurrent;

	/*
	 * Attempt to cause damage to any visible object
	 * @return The status of the hit made
	 */
	bool TryMakeHit();

	friend class AGameHUD;
};
