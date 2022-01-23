

#include "HUD/UI/GamePlayUserWidget.h"
#include "GamePlayMode.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGamePlayUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetGamePlayMode()->GetPlayerCharacter()->OnStateWeaponChanged.AddUObject(this, &UGamePlayUserWidget::OnStateWeaponChanged);
	GetGamePlayMode()->GetPlayerCharacter()->OnStateAimChanged.AddUObject(this, &UGamePlayUserWidget::OnStateAimChanged);
	GetGamePlayMode()->GetPlayerCharacter()->OnStateMoveCharacterChanged.AddUObject(this, &UGamePlayUserWidget::OnStateMoveCharacterChanged);
}

void UGamePlayUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const int32 AmmoInClip = GetGamePlayMode()->GetPlayerCharacter()->GetWeaponOnHand()->GetAmmoInClip();
	const int32 RemainAmmo = GetGamePlayMode()->GetPlayerCharacter()->GetWeaponOnHand()->GetRemainAmmo();

	this->AmmoText->SetText(FText::FromString(FString::FromInt(AmmoInClip)));
	this->RemainAmmoText->SetText(FText::FromString(FString::FromInt(RemainAmmo)));

	const bool IsEmptyAmmoInClip = GetGamePlayMode()->GetPlayerCharacter()->GetWeaponOnHand()->IsEmptyAmmoInClip();
	this->MagazineWeaponImage->SetVisibility((IsEmptyAmmoInClip) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UGamePlayUserWidget::OnStateWeaponChanged(EStateWeapon NewState)
{
	const auto CurrentWeapon = GetGamePlayMode()->GetPlayerCharacter()->GetWeaponOnHand();

	FSlateBrush SlateBrushBody;
	SlateBrushBody.SetResourceObject(CurrentWeapon->GetTextureWeaponBody());
	SlateBrushBody.SetImageSize(this->BodyWeaponImage->Brush.ImageSize);
	this->BodyWeaponImage->SetBrush(SlateBrushBody);
	
	FSlateBrush SlateBrushMagazine;
	SlateBrushMagazine.SetResourceObject(CurrentWeapon->GetTextureWeaponMagazine());
	SlateBrushMagazine.SetImageSize(this->MagazineWeaponImage->Brush.ImageSize);
	this->MagazineWeaponImage->SetBrush(SlateBrushMagazine);

	FSlateBrush SlateBrushIronsights;
	SlateBrushIronsights.SetResourceObject(CurrentWeapon->GetTextureWeaponIronsights());
	SlateBrushIronsights.SetImageSize(this->IronsightsWeaponImage->Brush.ImageSize);
	this->IronsightsWeaponImage->SetBrush(SlateBrushIronsights);

	FSlateBrush SlateBrushCrosshair;
	SlateBrushCrosshair.SetResourceObject(CurrentWeapon->GetTextureCrossHair());
	SlateBrushCrosshair.SetImageSize(FVector2D(CurrentWeapon->GetTextureCrossHair()->GetSizeX(), CurrentWeapon->GetTextureCrossHair()->GetSizeY()));
	this->CrossHairImage->SetBrush(SlateBrushCrosshair);
}

void UGamePlayUserWidget::OnStateAimChanged(EStateAim NewState)
{
	this->CrossHairImage->SetVisibility(NewState == EStateAim::Aiming ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UGamePlayUserWidget::OnStateMoveCharacterChanged(EStateMoveCharacter NewState)
{
	this->CrossHairImage->SetVisibility(NewState == EStateMoveCharacter::Running ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

}
