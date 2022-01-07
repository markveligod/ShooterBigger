

#include "HUD/GameHUD.h"
#include "GamePlayMode.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	this->GamePlayMode = AGamePlayMode::Get(GetWorld());
	checkf(this->GamePlayMode, TEXT("Game play mode is nullptr"));
	this->PlayerCharacter = this->GamePlayMode->GetPlayerCharacter();
	checkf(this->PlayerCharacter, TEXT("Player character is nullptr"));
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

#if !UE_BUILD_SHIPPING
	if (this->GamePlayMode && IsEnableDebugInfo())
	{
		AddText(TEXT("---|Player Data|---"), FText::FromString(""));
		AddText(TEXT("Locale role:"), FText::FromString(UEnum::GetValueAsString(GetLocalRole())));
		AddFloat(TEXT("Control Horizontal:"), this->PlayerCharacter->Horizontal);
		AddFloat(TEXT("Control Vertical:"), this->PlayerCharacter->Vertical);
		AddFloat(TEXT("Rotate Pitch:"), this->PlayerCharacter->Pitch);
		AddFloat(TEXT("Rotate Yaw:"), this->PlayerCharacter->Yaw);
		AddBool(TEXT("Is crouching:"), this->PlayerCharacter->GetCharacterMovement()->IsCrouching());
		AddFloat(TEXT("Speed(cm/s):"), this->PlayerCharacter->GetVelocity().Size());
		AddText(TEXT("State EMovementMode:"),
			FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->GetCharacterMovement()->MovementMode)));
		AddText(TEXT("State move:"), FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->StateMoveCharacter)));
		AddText(TEXT("State aim:"), FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->StateAim)));
	}
#endif
}
