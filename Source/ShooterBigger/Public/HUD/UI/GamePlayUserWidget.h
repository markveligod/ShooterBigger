

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterDataTypes.h"
#include "HUD/UI/UserWidgetBase.h"
#include "GamePlayUserWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class SHOOTERBIGGER_API UGamePlayUserWidget : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Body image weapon 
	UPROPERTY(Transient, meta = (BindWidget))
	UImage* BodyWeaponImage;
	// Magazine image weapon
	UPROPERTY(Transient, meta = (BindWidget))
	UImage* MagazineWeaponImage;
	// Ironsights image weapon 
	UPROPERTY(Transient, meta = (BindWidget))
	UImage* IronsightsWeaponImage;

	// Display current ammo in clip weapon
	UPROPERTY(Transient, meta = (BindWidget))
	UTextBlock* AmmoText;
	// Display max ammo in
	UPROPERTY(Transient, meta = (BindWidget))
	UTextBlock* RemainAmmoText;
	
	// State weapon changed
	void OnStateWeaponChanged(EStateWeapon NewState);
};
