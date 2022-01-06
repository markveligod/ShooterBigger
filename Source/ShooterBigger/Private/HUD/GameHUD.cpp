

#include "HUD/GameHUD.h"
#include "GamePlayMode.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	this->GamePlayMode = AGamePlayMode::Get(GetWorld());
	checkf(this->GamePlayMode, TEXT("Game play mode is nullptr"));
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (this->GamePlayMode && IsEnableDebugInfo())
	{
		AddText(TEXT("---|Player Data|---"), FText::FromString(""));
		AddText(TEXT("Locale role:"), FText::FromString(UEnum::GetValueAsString(GetLocalRole())));
		AddFloat(TEXT("Control Horizontal:"), this->GamePlayMode->GetPlayerCharacter()->Horizontal);
		AddFloat(TEXT("Control Vertical:"), this->GamePlayMode->GetPlayerCharacter()->Vertical);
		AddFloat(TEXT("Rotate Pitch:"), this->GamePlayMode->GetPlayerCharacter()->Pitch);
		AddFloat(TEXT("Rotate Yaw:"), this->GamePlayMode->GetPlayerCharacter()->Yaw);
		AddFloat(TEXT("Speed(cm/s):"), this->GamePlayMode->GetPlayerCharacter()->GetVelocity().Size());
		AddText(TEXT("State EMovementMode:"),
			FText::FromString(UEnum::GetValueAsString(this->GamePlayMode->GetPlayerCharacter()->GetCharacterMovement()->MovementMode)));
		AddText(
			TEXT("State move:"), FText::FromString(UEnum::GetValueAsString(this->GamePlayMode->GetPlayerCharacter()->StateMoveCharacter)));
	}
}
