

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
		AddText(TEXT("---|Player Data -> Controlling|---"), FText::FromString(""));
		AddFloat(TEXT("Control Horizontal:"), this->PlayerCharacter->Horizontal);
		AddFloat(TEXT("Control Vertical:"), this->PlayerCharacter->Vertical);
		AddFloat(TEXT("Rotate Pitch:"), this->PlayerCharacter->Pitch);
		AddFloat(TEXT("Rotate Yaw:"), this->PlayerCharacter->Yaw);
		AddFloat(TEXT("Speed(cm/s):"), this->PlayerCharacter->GetVelocity().Size());

		AddText(TEXT(""), FText::FromString(""));
		AddText(TEXT("---|Player Data -> State|---"), FText::FromString(""));
		AddBool(TEXT("Is crouching:"), this->PlayerCharacter->GetCharacterMovement()->IsCrouching());
		AddText(TEXT("State EMovementMode:"),
			FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->GetCharacterMovement()->MovementMode)));
		AddText(TEXT("State move:"), FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->StateMoveCharacter)));
		AddText(TEXT("State aim:"), FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->StateAim)));
		AddText(TEXT("State action:"), FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->StateAction)));
		AddText(TEXT("Inventory weapon:"), FText::FromString(UEnum::GetValueAsString(this->PlayerCharacter->StateWeapon)));

		AddText(TEXT(""), FText::FromString(""));
		AddText(TEXT("---|Player Data -> Weapon|---"), FText::FromString(""));
		AddBool(TEXT("Is automatic:"), this->PlayerCharacter->WeaponOnHand->bAutomatic);
		if (this->PlayerCharacter->WeaponOnHand->bAutomatic)
		{
			AddFloat(TEXT("Rate on Fire:"), this->PlayerCharacter->WeaponOnHand->RateOfFire);
		}
		AddText(TEXT("Ammunition:"), FText::FromString(FString::FromInt(this->PlayerCharacter->WeaponOnHand->AmmunitionCurrent) + "/" +
													   FString::FromInt(this->PlayerCharacter->WeaponOnHand->AmmunitionMax)));
		AddFloat(TEXT("Distance shot:"), this->PlayerCharacter->WeaponOnHand->DistanceShot);
		AddFloat(TEXT("Amount damage:"), this->PlayerCharacter->WeaponOnHand->AmountDamage);
	}
#endif
}
