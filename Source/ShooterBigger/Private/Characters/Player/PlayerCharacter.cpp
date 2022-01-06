

#include "Characters/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

APlayerCharacter::APlayerCharacter()
{
	GetMesh()->DestroyComponent();

	// Create spring arm component
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring arm"));
	this->SpringArm->SetupAttachment(GetCapsuleComponent());

	// Create camera component
	this->Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	this->Camera->SetupAttachment(this->SpringArm);

	// Create mesh base
	this->MeshBase = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh Hand"));
	this->MeshBase->SetupAttachment(this->Camera);

	GetCharacterMovement()->MaxWalkSpeed = this->SpeedWalking;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	this->CheckStateMoveCharacter();
	this->UpdateRotCamera();
}

void APlayerCharacter::CheckStateMoveCharacter()
{
	if (GetCharacterMovement()->MovementMode != MOVE_Walking) return;

	const float SpeedVelocity = GetVelocity().Size();
	if (SpeedVelocity == 0.0f)
		this->StateMoveCharacter = EStateMoveCharacter::Idle;
	else if (SpeedVelocity > 0.0f && SpeedVelocity <= this->SpeedWalking)
		this->StateMoveCharacter = EStateMoveCharacter::Walk;
	else if (SpeedVelocity == this->SpeedWalking)
		this->StateMoveCharacter = EStateMoveCharacter::Running;
}

void APlayerCharacter::UpdateRotCamera()
{
	if (this->Pitch == 0.0f) return;

	const FRotator Rot = GetControlRotation();
	this->SpringArm->SetRelativeRotation(FRotator(Rot.Pitch, 0.0f, 0.0f));
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (this->SpringArm)
	{
		const FVector RelLoc = this->ViewOffset + FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		this->SpringArm->SetRelativeLocation(RelLoc);
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
