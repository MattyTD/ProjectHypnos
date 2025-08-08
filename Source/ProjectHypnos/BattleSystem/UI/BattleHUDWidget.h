// BattleHUDWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Units/CombatUnit.h"
#include "../Managers/BattleManager.h" // For EBattleState
#include "BattleHUDWidget.generated.h"

UCLASS(Blueprintable)
class PROJECTHYPNOS_API UBattleHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UBattleHUDWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // UI Elements - Timer
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* TimerBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimerText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentUnitText;

    // UI Elements - Unit Status
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* EOBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* MPBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HPText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EOText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MPText;

    // UI Elements - Battle Info
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SetNumberText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BattleStateText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TFNStatusText;

    // UI Elements - Action Buttons
    UPROPERTY(meta = (BindWidget))
    class UButton* MoveButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* AttackButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SkillButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ItemButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* PassButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* RantiButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* TransformButton;

    // UI Elements - Position Buttons
    UPROPERTY(meta = (BindWidget))
    class UButton* NorthButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* EastButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SouthButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* WestButton;

    // UI Elements - Defense Buttons
    UPROPERTY(meta = (BindWidget))
    class UButton* GuardButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* DodgeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ParryButton;

    // UI Elements - Status Effects
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StressedOutText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StockpileText;

    // Events
    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnBattleStateChanged(EBattleState NewState);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnUnitTurnStarted(ACombatUnit* InUnit);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnUnitTurnEnded(ACombatUnit* InUnit);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnSetComplete();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnEnemyTurnStarted();

    // Button Click Handlers
    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnMoveButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnAttackButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnSkillButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnItemButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnPassButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnRantiButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnTransformButtonClicked();

    // Position Button Handlers
    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnNorthButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnEastButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnSouthButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnWestButtonClicked();

    // Defense Button Handlers
    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnGuardButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnDodgeButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void OnParryButtonClicked();

    // Core Functions
    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void UpdateTimerDisplay(float RemainingTime, float MaxTime);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void UpdateUnitStatus(ACombatUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void UpdateBattleInfo(int32 SetNumber, EBattleState BattleState);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void UpdateActionButtons(bool bIsPlayerTurn, ACombatUnit* InCurrentUnit);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void UpdatePositionButtons(ACombatUnit* InCurrentUnit);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void UpdateDefenseButtons(EBattlePosition CurrentPosition);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void ShowTFNStatus(bool bActive, float SpeedMultiplier);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void ShowStatusEffects(ACombatUnit* Unit);

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void SetButtonEnabled(UButton* Button, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    void SetButtonVisible(UButton* Button, bool bVisible);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    FString FormatTime(float TimeInSeconds);

    UFUNCTION(BlueprintCallable, Category = "Battle UI")
    FString GetBattleStateString(EBattleState State);

protected:
    class ABattleManager* BattleManager;
    class ADefenseManager* DefenseManager;
    class APositionManager* PositionManager;

    ACombatUnit* CurrentUnit;
    EBattleState CurrentBattleState;
    int32 CurrentSetNumber;

    void InitializeManagers();
    void BindButtonEvents();
    void UpdateAllUI();
};