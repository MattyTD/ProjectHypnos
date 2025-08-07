#include "BattleHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBattleHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // You can initialize defaults here
}

void UBattleHUDWidget::UpdateHUD(float HPPercent, float TimerPercent, const FText& Name)
{
    if (HPBar)    HPBar->SetPercent(HPPercent);
    if (TimerBar) TimerBar->SetPercent(TimerPercent);
    if (UnitNameText) UnitNameText->SetText(Name);
}