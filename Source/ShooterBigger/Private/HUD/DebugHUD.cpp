

#include "HUD/DebugHUD.h"
#include "CanvasItem.h"
#include "Engine/Canvas.h"

DEFINE_LOG_CATEGORY_STATIC(LogDebugHUD, All, All);

ADebugHUD::ADebugHUD()
{
	// Setup standart Font in Engine
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/Roboto"));
	this->MainFont = Font.Object;

	// Tick settings
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
}

void ADebugHUD::BeginPlay()
{
	Super::BeginPlay();

	// Setup Font params
	this->ParamFont.FontObject = this->MainFont;
	this->ParamFont.Size = this->SizeFont;

	// print log all debug info
	UE_LOG(LogDebugHUD, Display, TEXT("---|Debug HUD info|---"));
	UE_LOG(LogDebugHUD, Display, TEXT("Enable state debug info: %s"), *this->BoolToText(this->EnableDebugHUD).ToString());
	UE_LOG(LogDebugHUD, Display, TEXT("Font: %s"), *this->MainFont->GetName());
	UE_LOG(LogDebugHUD, Display, TEXT("SizeFont: %i"), this->SizeFont);
	UE_LOG(LogDebugHUD, Display, TEXT("Title color: %s"), *this->TitleColor.ToString());
	UE_LOG(LogDebugHUD, Display, TEXT("Default coordinate X: %f Y: %f"), this->DefaultX, this->DefaultY);
	UE_LOG(LogDebugHUD, Display, TEXT("Line Height: %f"), this->LineHeight);
	UE_LOG(LogDebugHUD, Display, TEXT("Horizontal Offset: %f"), this->HorizontalOffset);
	UE_LOG(LogDebugHUD, Display, TEXT("---|Debug HUD info|---"));
}

void ADebugHUD::DrawHUD()
{
	this->X = this->DefaultX;
	this->Y = this->DefaultY;
}

void ADebugHUD::RenderStatistic(const TCHAR* Title, const FText& Value, const FLinearColor& ValueColor)
{
	FCanvasTextItem TitleItem(FVector2D(this->X, this->Y), CStringToText(Title), this->ParamFont, this->TitleColor);
	TitleItem.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
	Canvas->DrawItem(TitleItem);

	FCanvasTextItem ValueItem(FVector2D(this->X + this->HorizontalOffset, this->Y), Value, this->ParamFont, ValueColor);
	ValueItem.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
	Canvas->DrawItem(ValueItem);
	this->Y += this->LineHeight;
}
