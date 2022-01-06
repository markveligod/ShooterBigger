

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

/**
 *
 */
UCLASS()
class SHOOTERBIGGER_API ADebugHUD : public AHUD
{
	GENERATED_BODY()
public:
	// Construct the debugging HUD, mainly establishing a font to use for display.
	ADebugHUD();

protected:
	// Begin play
	virtual void BeginPlay() override;

	// Draw the HUD.
	virtual void DrawHUD() override;

	// Add a FText to the HUD for rendering.
	FORCEINLINE void AddText(const TCHAR* Title, const FText& Value) { RenderStatistic(Title, Value); }

	// Add a float to the HUD for rendering.
	FORCEINLINE void AddFloat(const TCHAR* Title, const float Value) { RenderStatistic(Title, FText::AsNumber(Value)); }

	// Add an int32 to the HUD for rendering.
	FORCEINLINE void AddInt(const TCHAR* Title, const int32 Value) { RenderStatistic(Title, FText::AsNumber(Value)); }

	// Add a bool to the HUD for rendering.
	FORCEINLINE void AddBool(const TCHAR* Title, const bool bValue)
	{
		RenderStatistic(Title, BoolToText(bValue), (bValue == false) ? FLinearColor::Red : FLinearColor::Green);
	}

	// Is debug information enable?
	FORCEINLINE bool IsEnableDebugInfo() const { return (this->EnableDebugHUD); }

private:
	// Enable debug information
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool EnableDebugHUD = true;

	// Font used to render the debug information.
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	UFont* MainFont = nullptr;

	// Size text for Main font.
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	int32 SizeFont = 32;

	// Color display of the title parameter text
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	FLinearColor TitleColor = FLinearColor::White;

	// The current X coordinate.
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	float DefaultX = 50.0f;

	// The current Y coordinate.
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	float DefaultY = 50.0f;

	// The line height to separate each HUD entry.
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	float LineHeight = 16.0f;

	// The horizontal offset to render the statistic values.
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (EditCondition = "EnableDebugHUD"))
	float HorizontalOffset = 150.0f;

	// Current X and Y
	float X;
	float Y;

	// Settings Font
	FSlateFontInfo ParamFont;

	// Render a statistic onto the HUD canvas.
	void RenderStatistic(const TCHAR* Title, const FText& Value, const FLinearColor& ValueColor = FLinearColor::White);

	// Convert a TCHAR pointer to FText.
	FORCEINLINE FText CStringToText(const TCHAR* Text) const { return FText::FromString(Text); }

	// Convert a bool to FText.
	FORCEINLINE FText BoolToText(bool bValue) const { return CStringToText((bValue == true) ? TEXT("true") : TEXT("false")); }
};
