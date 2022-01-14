

#include "Characters/Player/PlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Weapons/WeaponBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

APlayerCharacter::APlayerCharacter()
{
	// Create spring arm component
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring arm"));
	this->SpringArm->SetupAttachment(GetRootComponent());

	// Create camera component
	this->Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	this->Camera->SetupAttachment(this->SpringArm);

	// Reattach mesh component
	GetMesh()->SetupAttachment(this->Camera);

	GetCharacterMovement()->MaxWalkSpeed = this->SpeedWalking;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(this->Camera);
	check(this->SpringArm);

	// Reduction to prevent intersections with objects
	this->SpringArm->SetWorldScale3D(FVector(0.1f));

	// Begin setup weapons
	this->SpawnAllWeapons();
	this->StateWeapon = EStateWeapon::Pistol;
	this->ChangeOnNewWeaponOnHand(this->StateWeapon);

	// Setup aiming
	this->StateAim = EStateAim::Hip;
}

void APlayerCharacter::SetupDebugTraceShot(const bool NewState)
{
	for (const auto Pair : InventoryWeapons)
	{
		Pair.Value->SetupDebugTraceShot(NewState);
	}
}

void APlayerCharacter::SpawnAllWeapons()
{
	FTransform ActorTransform;
	ActorTransform.SetLocation(GetActorLocation());
	FActorSpawnParameters ActorSpawnParameters;

	ActorSpawnParameters.Owner = this;
	ActorSpawnParameters.Instigator = this;

	UE_LOG(LogPlayerCharacter, Display, TEXT("---|Spawned All Weapons|---"))

	for (const auto Pair : this->SampleDataWeapons)
	{
		auto WeaponData = Pair.Value;
		AWeaponBase* TempWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponData.TypeWeapon, ActorTransform, ActorSpawnParameters);
		if (TempWeapon)
		{
			TempWeapon->SetActorHiddenInGame(true);
			this->InventoryWeapons.Add(Pair.Key, TempWeapon);
			UE_LOG(LogPlayerCharacter, Display, TEXT("%s | is spawned"), *WeaponData.ToString());
		}
		else
		{
			UE_LOG(LogPlayerCharacter, Error, TEXT("%s | is doesn't spawning"), *WeaponData.ToString())
		}
	}
}

void APlayerCharacter::SetupWeaponOnHand(EStateWeapon NewState)
{
	if (this->StateWeapon == NewState)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Current and new state equal %s"), *UEnum::GetValueAsString(NewState));
		return;
	}

	if (!this->InventoryWeapons.Contains(NewState))
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Name player: %s | Type state: %s | doesn't contain in inventory"), *GetName(),
			*UEnum::GetValueAsString(NewState));
		return;
	}

	this->StateAction = EStateAction::Holstering;
	if (this->CrossHairOnWeapon)
	{
		this->CrossHairOnWeapon->RemoveFromViewport();
	}
	const float RateTime = PlayAnimMontage(this->SampleDataWeapons[this->StateWeapon].MontageHolster);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &APlayerCharacter::ChangeOnNewWeaponOnHand, NewState);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
}

void APlayerCharacter::ChangeOnNewWeaponOnHand(EStateWeapon NewState)
{
	if (this->WeaponOnHand)
	{
		this->WeaponOnHand->SetActorHiddenInGame(true);
		const FDetachmentTransformRules TransformRules(EDetachmentRule::KeepWorld, false);
		this->WeaponOnHand->DetachFromActor(TransformRules);
		this->WeaponOnHand->SetActorLocation(GetActorLocation());
	}

	this->WeaponOnHand = this->InventoryWeapons[NewState];
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	this->WeaponOnHand->AttachToComponent(GetMesh(), TransformRules, this->SocketWeapon);

	PlayAnimMontage(this->SampleDataWeapons[NewState].MontageUnholster);

	this->CrossHairOnWeapon = CreateWidget(Cast<APlayerController>(GetController()), this->SampleDataWeapons[NewState].CrossHairWidget);
	this->CrossHairOnWeapon->AddToViewport();

	this->WeaponOnHand->SetActorHiddenInGame(false);
	this->StateAction = EStateAction::None;
	this->StateWeapon = NewState;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	this->CheckStateMoveCharacter();
	this->UpdateLocCamera(DeltaSeconds);
	this->UpdateRotCamera();
}

void APlayerCharacter::CheckStateMoveCharacter()
{
	if (!GetCharacterMovement()->IsWalking()) return;

	const float SpeedVelocity = GetVelocity().Size();
	if (SpeedVelocity == 0.0f)
	{
		this->StateMoveCharacter = EStateMoveCharacter::Idle;
	}
	else if (SpeedVelocity > 0.0f && SpeedVelocity <= this->SpeedWalking)
	{
		this->StateMoveCharacter = EStateMoveCharacter::Walk;
	}
	else if ((this->SpeedWalking + ((this->SpeedRunning - this->SpeedWalking) / 4)) <= SpeedVelocity && SpeedVelocity <= this->SpeedRunning)
	{
		this->StateMoveCharacter = EStateMoveCharacter::Running;
	}
}

void APlayerCharacter::UpdateLocCamera(float DeltaTime)
{
	const FVector CurrLocRelSA = this->SpringArm->GetRelativeLocation();
	const FVector NewLocRelSA = UKismetMathLibrary::VInterpTo(CurrLocRelSA, GetViewLocation(), DeltaTime, 10.0f);
	this->SpringArm->SetRelativeLocation(NewLocRelSA);

	this->GetMesh()->SetRelativeLocation(FVector::ZeroVector);
}

void APlayerCharacter::UpdateRotCamera()
{
	if (this->Pitch == 0.0f) return;

	const FRotator Rot = GetControlRotation();
	this->SpringArm->SetRelativeRotation(FRotator(Rot.Pitch, 0.0f, 0.0f));
}

FVector APlayerCharacter::GetViewLocation() const
{
	return (this->ViewOffset + FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (this->SpringArm)
	{
		this->SpringArm->SetRelativeLocation(GetViewLocation());
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Name player: %s | Loading input component fail"), *GetName());
		return;
	}

	PlayerInputComponent->BindAxis(FName("Vertical"), this, &APlayerCharacter::MoveVerticalInput);
	PlayerInputComponent->BindAxis(FName("Horizontal"), this, &APlayerCharacter::MoveHorizontalInput);
	PlayerInputComponent->BindAxis(FName("Pitch"), this, &APlayerCharacter::RotatePitchInput);
	PlayerInputComponent->BindAxis(FName("Yaw"), this, &APlayerCharacter::RotateYawInput);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Pressed, this, &APlayerCharacter::ActionCrouch);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &APlayerCharacter::ActionJump);
	PlayerInputComponent->BindAction(FName("Run"), IE_Pressed, this, &APlayerCharacter::ActionBoostRun);
	PlayerInputComponent->BindAction(FName("Run"), IE_Released, this, &APlayerCharacter::ActionStopRun);
	PlayerInputComponent->BindAction(FName("Aim"), IE_Pressed, this, &APlayerCharacter::ActionAim);
	PlayerInputComponent->BindAction(FName("Aim"), IE_Released, this, &APlayerCharacter::ActionHip);
	PlayerInputComponent->BindAction(FName("PistolInv"), IE_Pressed, this, &APlayerCharacter::ActionPistolInv);
	PlayerInputComponent->BindAction(FName("RifleInv"), IE_Pressed, this, &APlayerCharacter::ActionRifleInv);
	PlayerInputComponent->BindAction(FName("Inspect Weapon"), IE_Pressed, this, &APlayerCharacter::ActionInspectOn);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &APlayerCharacter::ActionFireOn);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &APlayerCharacter::ActionFireOff);
	PlayerInputComponent->BindAction(FName("Reload"), IE_Pressed, this, &APlayerCharacter::ActionReload);
}

void APlayerCharacter::MoveHorizontalInput(float Value)
{
	this->Horizontal = Value;
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking ||
		GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::MoveVerticalInput(float Value)
{
	this->Vertical = Value;
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking ||
		GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::RotateYawInput(float Value)
{
	this->Yaw = Value;
	AddControllerYawInput(Value);
}

void APlayerCharacter::RotatePitchInput(float Value)
{
	this->Pitch = Value;
	AddControllerPitchInput(Value);
}

/*
 * Chapter action controlling
 */

void APlayerCharacter::ActionCrouch()
{
	if (!GetCharacterMovement()->IsWalking()) return;

	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void APlayerCharacter::ActionJump()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
		return;
	}

	if (GetCharacterMovement()->IsWalking())
	{
		Jump();
		this->ActionStopRun();

		// Spawn sound jump
		UGameplayStatics::PlaySound2D(GetWorld(), this->SoundCueJump);
	}
}

void APlayerCharacter::ActionBoostRun()
{
	if (!GetCharacterMovement()->IsWalking()) return;

	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	GetCharacterMovement()->MaxWalkSpeed = this->SpeedRunning;
	this->ActionFireOff();
	StopAnimMontage(this->SampleDataWeapons[this->StateWeapon].MontageInspect);
	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::ActionStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = this->SpeedWalking;
	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::ActionAim()
{
	if (this->StateAction != EStateAction::None || this->StateMoveCharacter == EStateMoveCharacter::Running) return;

	this->StateAim = EStateAim::Aiming;
	GetCharacterMovement()->MaxWalkSpeed = this->SpeedAiming;
	UGameplayStatics::SpawnSound2D(GetWorld(), this->SoundCueAim);
	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::ActionHip()
{
	this->StateAim = EStateAim::Hip;
	GetCharacterMovement()->MaxWalkSpeed = this->SpeedWalking;
	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::ActionPistolInv()
{
	if (this->StateAction != EStateAction::None) return;

	this->SetupWeaponOnHand(EStateWeapon::Pistol);
}

void APlayerCharacter::ActionRifleInv()
{
	if (this->StateAction != EStateAction::None) return;

	this->SetupWeaponOnHand(EStateWeapon::Rifle);
}

void APlayerCharacter::ActionInspectOn()
{
	if (this->StateAction != EStateAction::None || this->StateMoveCharacter == EStateMoveCharacter::Running) return;

	const float RateTime = PlayAnimMontage(this->SampleDataWeapons[this->StateWeapon].MontageInspect);
	this->StateAction = EStateAction::Inspecting;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ActionInspectOff, RateTime, false);

	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::ActionInspectOff()
{
	this->StateAction = EStateAction::None;
	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::ActionFireOn()
{
	if (this->StateAction != EStateAction::None) return;

	this->StateAction = EStateAction::Fire;

	this->MakeShot();
	if (this->WeaponOnHand->IsWeaponAutomatic())
	{
		GetWorldTimerManager().SetTimer(
			this->TimerHandleFire, this, &APlayerCharacter::MakeShot, 60 / this->WeaponOnHand->GetRateOfFire(), true);
	}
}

void APlayerCharacter::MakeShot()
{
	if (this->StateMoveCharacter == EStateMoveCharacter::Running) return;

	if (this->WeaponOnHand->IsEmptyAmmoInClip())
	{
		// todo: anim or sound empty clip
		return;
	}

	PlayAnimMontage(this->SampleDataWeapons[this->StateWeapon].MontageFire);
	this->WeaponOnHand->MakeShot();
}

void APlayerCharacter::ActionFireOff()
{
	GetWorldTimerManager().ClearTimer(this->TimerHandleFire);
	this->StateAction = EStateAction::None;
}

void APlayerCharacter::ActionReload()
{
	if (this->StateAction != EStateAction::None) return;

	this->StateAction = EStateAction::Reloading;

	UAnimMontage* PlayMontageReload = (this->WeaponOnHand->IsEmptyAmmoInClip())
										  ? this->SampleDataWeapons[this->StateWeapon].MontageReloadEmpty
										  : this->SampleDataWeapons[this->StateWeapon].MontageReload;
	const float RateTime = PlayAnimMontage(PlayMontageReload);

	this->WeaponOnHand->ReloadWeapon();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ResetActionReload, RateTime, false);

	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::ResetActionReload()
{
	this->StateAction = EStateAction::None;
	this->CrossHairOnWeapon->SetVisibility(ESlateVisibility::Visible);
}
