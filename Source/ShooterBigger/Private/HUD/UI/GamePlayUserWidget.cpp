

#include "HUD/UI/GamePlayUserWidget.h"
#include "GamePlayMode.h"
#include "Components/Image.h"

void UGamePlayUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetGamePlayMode()->GetPlayerCharacter()->OnStateWeaponChanged.AddUObject(this, &UGamePlayUserWidget::OnStateWeaponChanged);
}

void UGamePlayUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGamePlayUserWidget::OnStateWeaponChanged(EStateWeapon NewState)
{
	const auto CurrentWeapon = GetGamePlayMode()->GetPlayerCharacter()->GetWeaponOnHand();

	FSlateBrush SlateBrushBody;
	SlateBrushBody.SetResourceObject(CurrentWeapon->GetTextureWeaponBody());
	this->BodyWeaponImage->SetBrush(SlateBrushBody);
	FSlateBrush SlateBrushMagazine;
	SlateBrushMagazine.SetResourceObject(CurrentWeapon->GetTextureWeaponMagazine());
	this->MagazineWeaponImage->SetBrush(SlateBrushMagazine);
	FSlateBrush SlateBrushIronsights;
	SlateBrushIronsights.SetResourceObject(CurrentWeapon->GetTextureWeaponIronsights());
	this->IronsightsWeaponImage->SetBrush(SlateBrushIronsights);
}
