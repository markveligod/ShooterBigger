

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterDataTypes.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AWeaponBase;
class UCameraComponent;
class USpringArmComponent;
class USoundCue;
/**
 *
 */
UCLASS()
class SHOOTERBIGGER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

#pragma region GetState
	// Getting current state move character
	UFUNCTION(BlueprintCallable, Category = "APlayerCharacter|State")
	FORCEINLINE EStateMoveCharacter GetStateMoveCharacter() const { return this->StateMoveCharacter; }

	// Getting current state aim of player character
	UFUNCTION(BlueprintCallable, Category = "APlayerCharacter|State")
	FORCEINLINE EStateAim GetStateAim() const { return this->StateAim; }

	// Getting current state weapon of player character
	UFUNCTION(BlueprintCallable, Category = "APlayerCharacter|State")
	FORCEINLINE EStateWeapon GetStateWeapon() const { return (this->StateWeapon); }

	// Getting current state montage of begin animation
	UFUNCTION(BlueprintCallable, Category = "APlayerCharacter|State")
	FORCEINLINE EStateAction GetStateActionMontage() const { return this->StateAction; }
#pragma endregion

protected:
#pragma region OverRiding
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#pragma endregion

private:
#pragma region Components
	// A component for holding the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;

	// Camera component for player
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

#pragma endregion

#pragma region CharacterSettings
	// Name of sockets for attaching weapons
	UPROPERTY(EditDefaultsOnly, Category = "Character | Sockets")
	FName SocketWeapon = "SOCKET_Weapon";

	// Sound Cue played when this character aims their weapon. This can just be left empty for games that do not need an aiming sound.
	UPROPERTY(EditDefaultsOnly, Category = "Character | Audio")
	USoundCue* SoundCueAim;

	// Sound Cue played when this character jumps. It only plays once, as soon as the character leaves the ground.
	UPROPERTY(EditDefaultsOnly, Category = "Character | Audio")
	USoundCue* SoundCueJump;

	// Maximum speed at which the character can move when walking. This value may be modified by a myriad of other values in this blueprint
	// based on the situation, like the sideways and backward movement multipliers.
	UPROPERTY(EditDefaultsOnly, Category = "Character | Movement Settings")
	float SpeedWalking = 400.0f;

	// Maximum speed at which this character can move when running.
	UPROPERTY(EditDefaultsOnly, Category = "Character | Movement Settings")
	float SpeedRunning = 700.0f;

	// Maximum speed at which the character can move when walking and aiming their weapon.
	UPROPERTY(EditDefaultsOnly, Category = "Character | Movement Settings")
	float SpeedAiming = 350.0f;

	// Offset from the top of the character's capsule at which the first-person will be located.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | View", meta = (AllowPrivateAccess = true))
	FVector ViewOffset = FVector(0.0f, 0.0f, -35.0f);

	// Will determine what Weapon is spawned when starting to play this Character.
	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
	TMap<EStateWeapon, FWeaponData> SampleDataWeapons;
#pragma endregion

#pragma region Inventory
	// A map simulating inventory for storing weapons by condition
	UPROPERTY()
	TMap<EStateWeapon, AWeaponBase*> InventoryWeapons;

	// Current weapon on hands in Character
	UPROPERTY()
	AWeaponBase* WeaponOnHand;
#pragma endregion

#pragma region States
	// State move character
	EStateMoveCharacter StateMoveCharacter = EStateMoveCharacter::None;

	// State aim
	EStateAim StateAim = EStateAim::None;

	// State weapon
	EStateWeapon StateWeapon = EStateWeapon::None;

	// State action montage
	EStateAction StateAction;
#pragma endregion

	// Returns the time at which the Character last fired a Weapon.
	float LastFireTime;

	// Pitch Input and Yaw Input.
	float Pitch;
	float Yaw;

	// Horizontal Input and Vertical Input.
	float Horizontal;
	float Vertical;

	// Timer handle for fire automatic
	FTimerHandle TimerHandleFire;

	// Function for Horizontal and Vertical input
	void MoveHorizontalInput(float Value);
	void MoveVerticalInput(float Value);

	// Function for Yaw and Pitch input
	void RotateYawInput(float Value);
	void RotatePitchInput(float Value);

	// Check state character on Frame
	void CheckStateMoveCharacter();

	// Camera adjustment for independent location when crouching
	void UpdateLocCamera(float DeltaTime);

	// Camera adjustment for independent rotation when moving
	void UpdateRotCamera();

#pragma region ActionControlling

	// Crouching controlling
	void ActionCrouch();

	// Jumping controlling
	void ActionJump();

	// Running controlling
	void ActionBoostRun();
	void ActionStopRun();

	// Aim controlling
	void ActionAim();
	void ActionHip();

	// Inventory controlling
	void ActionPistolInv();
	void ActionRifleInv();

	// Inspecting controlling
	void ActionInspectOn();
	void ActionInspectOff();

	// Fire controlling
	void ActionFireOn();
	void ActionFireOff();
	void MakeShot();

	// Reload controlling
	void ActionReload();
	void ResetActionReload();

#pragma endregion

	// Get view location independent from Size capsule component
	FORCEINLINE FVector GetViewLocation() const;

	// Spawn All weapons in inventory
	void SpawnAllWeapons();
	// Setup new weapon on hand in character
	void SetupWeaponOnHand(EStateWeapon NewState);
	void ChangeOnNewWeaponOnHand(EStateWeapon NewState);

	friend class AGameHUD;
};
