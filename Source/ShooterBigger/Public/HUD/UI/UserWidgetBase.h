

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

class AGamePlayMode;
/**
 * 
 */
UCLASS()
class SHOOTERBIGGER_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	// Getting current pointer on GamePlayMode
	UFUNCTION(BlueprintCallable, Category = "UUserWidgetBase|Data")
	FORCEINLINE AGamePlayMode* GetGamePlayMode() const { return this->GamePlayMode; }
	
private:
	// Current pointer on AGamePlayMode
	UPROPERTY()
	AGamePlayMode* GamePlayMode;
};
