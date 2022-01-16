


#include "HUD/UI/UserWidgetBase.h"
#include "GamePlayMode.h"

void UUserWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	this->GamePlayMode = AGamePlayMode::Get(GetWorld());
	checkf(this->GamePlayMode, TEXT("Game play mode is nullptr"));
}
