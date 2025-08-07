#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "BattleHUDWidget.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTHYPNOS_API UBattleHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Bind these in UMG or via code to your progress bars */
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* TimerBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UnitNameText;

    /** Called when the widget is added to viewport */
    virtual void NativeConstruct() override;

    /** Expose a function to update bars from C++ */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUD(float HPPercent, float TimerPercent, const FText& Name);
};