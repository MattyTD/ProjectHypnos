// BattleHUDWidget.cpp
#include "BattleHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../Managers/BattleManager.h"
#include "../Managers/DefenseManager.h"
#include "../Managers/PositionManager.h"
#include "Kismet/GameplayStatics.h"

UBattleHUDWidget::UBattleHUDWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CurrentUnit = nullptr;
    CurrentBattleState = EBattleState::PlayerTurn;
    CurrentSetNumber = 1;
}

void UBattleHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitializeManagers();
    BindButtonEvents();
}

void UBattleHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Update timer display if we have a current unit
    if (BattleManager && BattleManager->GetCurrentUnit())
    {
        ACombatUnit* Unit = BattleManager->GetCurrentUnit();
        UpdateTimerDisplay(Unit->TimerRemaining, Unit->TimerDuration);
        UpdateUnitStatus(Unit);
    }
}

void UBattleHUDWidget::InitializeManagers()
{
    // Find managers in the world
    TArray<AActor*> FoundActors;
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        BattleManager = Cast<ABattleManager>(FoundActors[0]);
    }

    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADefenseManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        DefenseManager = Cast<ADefenseManager>(FoundActors[0]);
    }

    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APositionManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        PositionManager = Cast<APositionManager>(FoundActors[0]);
    }
}

void UBattleHUDWidget::BindButtonEvents()
{
    // Bind action buttons
    if (MoveButton)
        MoveButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnMoveButtonClicked);
    
    if (AttackButton)
        AttackButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnAttackButtonClicked);
    
    if (SkillButton)
        SkillButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnSkillButtonClicked);
    
    if (ItemButton)
        ItemButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnItemButtonClicked);
    
    if (PassButton)
        PassButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnPassButtonClicked);

    if (RetryButton)
        RetryButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnRetryButtonClicked);
    
    if (RantiButton)
        RantiButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnRantiButtonClicked);
    
    if (TransformButton)
        TransformButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnTransformButtonClicked);

    // Bind position buttons
    if (NorthButton)
        NorthButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnNorthButtonClicked);
    
    if (EastButton)
        EastButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnEastButtonClicked);
    
    if (SouthButton)
        SouthButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnSouthButtonClicked);
    
    if (WestButton)
        WestButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnWestButtonClicked);

    // Bind defense buttons
    if (GuardButton)
        GuardButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnGuardButtonClicked);
    
    if (DodgeButton)
        DodgeButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnDodgeButtonClicked);
    
    if (ParryButton)
        ParryButton->OnClicked.AddDynamic(this, &UBattleHUDWidget::OnParryButtonClicked);
}

void UBattleHUDWidget::OnBattleStateChanged(EBattleState NewState)
{
    CurrentBattleState = NewState;
    UpdateBattleInfo(CurrentSetNumber, CurrentBattleState);
    UpdateAllUI();
}

void UBattleHUDWidget::OnUnitTurnStarted(ACombatUnit* InUnit)
{
    CurrentUnit = InUnit;
    if (CurrentUnitText)
    {
        CurrentUnitText->SetText(FText::FromString(InUnit ? InUnit->UnitName : TEXT("Unknown")));
    }
    UpdateAllUI();
}

void UBattleHUDWidget::OnUnitTurnEnded(ACombatUnit* InUnit)
{
    // Clear current unit display
    if (CurrentUnitText)
    {
        CurrentUnitText->SetText(FText::FromString(TEXT("")));
    }
}

void UBattleHUDWidget::OnSetComplete()
{
    if (SetNumberText)
    {
        CurrentSetNumber++;
        SetNumberText->SetText(FText::FromString(FString::Printf(TEXT("Set %d"), CurrentSetNumber)));
    }
}

void UBattleHUDWidget::OnEnemyTurnStarted()
{
    if (BattleStateText)
    {
        BattleStateText->SetText(FText::FromString(TEXT("Enemy Turn")));
    }
}

void UBattleHUDWidget::UpdateTimerDisplay(float RemainingTime, float MaxTime)
{
    if (TimerBar)
    {
        float Percent = FMath::Clamp(RemainingTime / MaxTime, 0.0f, 1.0f);
        TimerBar->SetPercent(Percent);
    }

    if (TimerText)
    {
        TimerText->SetText(FText::FromString(FormatTime(RemainingTime)));
    }
}

void UBattleHUDWidget::UpdateUnitStatus(ACombatUnit* Unit)
{
    if (!Unit) return;

    // Update HP Bar
    if (HPBar)
    {
        float HPPercent = Unit->CurrentHP / Unit->MaxHP;
        HPBar->SetPercent(HPPercent);
    }

    if (HPText)
    {
        HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), Unit->CurrentHP, Unit->MaxHP)));
    }

    // Update EO Bar
    if (EOBar)
    {
        float EOPercent = Unit->CurrentEO / Unit->MaxEO;
        EOBar->SetPercent(EOPercent);
    }

    if (EOText)
    {
        EOText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), Unit->CurrentEO, Unit->MaxEO)));
    }

    // Update MP Bar (only visible in EO form)
    if (MPBar)
    {
        if (Unit->bIsInEOForm)
        {
            float MPPercent = Unit->CurrentMP / Unit->MaxMP;
            MPBar->SetPercent(MPPercent);
            MPBar->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            MPBar->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (MPText)
    {
        if (Unit->bIsInEOForm)
        {
            MPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), Unit->CurrentMP, Unit->MaxMP)));
            MPText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            MPText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // Update status effects
    ShowStatusEffects(Unit);
}

void UBattleHUDWidget::UpdateBattleInfo(int32 SetNumber, EBattleState BattleState)
{
    if (SetNumberText)
    {
        SetNumberText->SetText(FText::FromString(FString::Printf(TEXT("Set %d"), SetNumber)));
    }

    if (BattleStateText)
    {
        BattleStateText->SetText(FText::FromString(GetBattleStateString(BattleState)));
    }
}

void UBattleHUDWidget::UpdateActionButtons(bool bIsPlayerTurn, ACombatUnit* InCurrentUnit)
{
    bool bCanAct = bIsPlayerTurn && InCurrentUnit && InCurrentUnit->IsAlive();

    SetButtonEnabled(MoveButton, bCanAct);
    SetButtonEnabled(AttackButton, bCanAct);
    SetButtonEnabled(SkillButton, bCanAct);
    SetButtonEnabled(ItemButton, bCanAct);
    SetButtonEnabled(PassButton, bCanAct);
    SetButtonEnabled(RantiButton, bCanAct);
    SetButtonEnabled(TransformButton, bCanAct && InCurrentUnit && InCurrentUnit->CanTransformToEO());
}

void UBattleHUDWidget::UpdatePositionButtons(ACombatUnit* InCurrentUnit)
{
    if (!InCurrentUnit) return;

    // Enable position buttons based on current position
    SetButtonEnabled(NorthButton, InCurrentUnit->CurrentPosition != EBattlePosition::North);
    SetButtonEnabled(EastButton, InCurrentUnit->CurrentPosition != EBattlePosition::East);
    SetButtonEnabled(SouthButton, InCurrentUnit->CurrentPosition != EBattlePosition::South);
    SetButtonEnabled(WestButton, InCurrentUnit->CurrentPosition != EBattlePosition::West);
}

void UBattleHUDWidget::UpdateDefenseButtons(EBattlePosition CurrentPosition)
{
    if (!DefenseManager) return;

    SetButtonEnabled(GuardButton, DefenseManager->CanUseGuard(CurrentPosition));
    SetButtonEnabled(DodgeButton, DefenseManager->CanUseDodge(CurrentPosition));
    SetButtonEnabled(ParryButton, DefenseManager->CanUseParry(CurrentPosition));
}

void UBattleHUDWidget::ShowTFNStatus(bool bActive, float SpeedMultiplier)
{
    if (TFNStatusText)
    {
        if (bActive)
        {
            TFNStatusText->SetText(FText::FromString(FString::Printf(TEXT("TFN Active: %.2fx Speed"), SpeedMultiplier)));
            TFNStatusText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            TFNStatusText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UBattleHUDWidget::ShowStatusEffects(ACombatUnit* Unit)
{
    if (!Unit) return;

    // Show Stressed Out status
    if (StressedOutText)
    {
        if (Unit->bIsStressedOut)
        {
            StressedOutText->SetText(FText::FromString(TEXT("STRESSED OUT")));
            StressedOutText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            StressedOutText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // Show Stockpile status
    if (StockpileText)
    {
        if (Unit->StockpiledTime > 0.0f)
        {
            StockpileText->SetText(FText::FromString(FString::Printf(TEXT("SP: +%.1fs"), Unit->StockpiledTime)));
            StockpileText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            StockpileText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

// Button Click Handlers
void UBattleHUDWidget::OnMoveButtonClicked()
{
    // Show position buttons
    SetButtonVisible(NorthButton, true);
    SetButtonVisible(EastButton, true);
    SetButtonVisible(SouthButton, true);
    SetButtonVisible(WestButton, true);
}

void UBattleHUDWidget::OnAttackButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        // TODO: Implement target selection
        UE_LOG(LogTemp, Log, TEXT("%s attacks!"), *CurrentUnit->UnitName);
    }
}

void UBattleHUDWidget::OnSkillButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        // TODO: Implement skill selection
        UE_LOG(LogTemp, Log, TEXT("%s uses skill!"), *CurrentUnit->UnitName);
    }
}

void UBattleHUDWidget::OnItemButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        // TODO: Implement item selection
        UE_LOG(LogTemp, Log, TEXT("%s uses item!"), *CurrentUnit->UnitName);
    }
}

void UBattleHUDWidget::OnPassButtonClicked()
{
    if (BattleManager)
    {
        BattleManager->PassTurn();
    }
}

void UBattleHUDWidget::OnRetryButtonClicked()
{
    if (BattleManager)
    {
        BattleManager->RetryBattle();
    }
}

void UBattleHUDWidget::OnRantiButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        // TODO: Implement Ranti skill selection
        UE_LOG(LogTemp, Log, TEXT("%s uses Ranti skill!"), *CurrentUnit->UnitName);
    }
}

void UBattleHUDWidget::OnTransformButtonClicked()
{
    if (CurrentUnit && CurrentUnit->CanTransformToEO())
    {
        CurrentUnit->TransformToEO();
        UpdateUnitStatus(CurrentUnit);
    }
}

// Position Button Handlers
void UBattleHUDWidget::OnNorthButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        BattleManager->MoveUnit(CurrentUnit, EBattlePosition::North);
        UpdatePositionButtons(CurrentUnit);
        UpdateDefenseButtons(EBattlePosition::North);
    }
}

void UBattleHUDWidget::OnEastButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        BattleManager->MoveUnit(CurrentUnit, EBattlePosition::East);
        UpdatePositionButtons(CurrentUnit);
        UpdateDefenseButtons(EBattlePosition::East);
    }
}

void UBattleHUDWidget::OnSouthButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        BattleManager->MoveUnit(CurrentUnit, EBattlePosition::South);
        UpdatePositionButtons(CurrentUnit);
        UpdateDefenseButtons(EBattlePosition::South);
    }
}

void UBattleHUDWidget::OnWestButtonClicked()
{
    if (BattleManager && CurrentUnit)
    {
        BattleManager->MoveUnit(CurrentUnit, EBattlePosition::West);
        UpdatePositionButtons(CurrentUnit);
        UpdateDefenseButtons(EBattlePosition::West);
    }
}

// Defense Button Handlers
void UBattleHUDWidget::OnGuardButtonClicked()
{
    if (DefenseManager && CurrentUnit)
    {
        DefenseManager->AttemptGuard(CurrentUnit, 1.0f); // Perfect timing for now
    }
}

void UBattleHUDWidget::OnDodgeButtonClicked()
{
    if (DefenseManager && CurrentUnit)
    {
        // TODO: Implement timing-based dodge
        DefenseManager->AttemptDodge(CurrentUnit, 0.5f); // Good timing for now
    }
}

void UBattleHUDWidget::OnParryButtonClicked()
{
    if (DefenseManager && CurrentUnit)
    {
        // TODO: Implement timing-based parry
        DefenseManager->AttemptParry(CurrentUnit, 0.3f); // Good timing for now
    }
}

// Utility Functions
void UBattleHUDWidget::SetButtonEnabled(UButton* Button, bool bEnabled)
{
    if (Button)
    {
        Button->SetIsEnabled(bEnabled);
    }
}

void UBattleHUDWidget::SetButtonVisible(UButton* Button, bool bVisible)
{
    if (Button)
    {
        Button->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

FString UBattleHUDWidget::FormatTime(float TimeInSeconds)
{
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
    int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;
    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

FString UBattleHUDWidget::GetBattleStateString(EBattleState State)
{
    switch (State)
    {
        case EBattleState::PlayerTurn:
            return TEXT("Player Turn");
        case EBattleState::EnemyTurn:
            return TEXT("Enemy Turn");
        case EBattleState::Victory:
            return TEXT("Victory!");
        case EBattleState::Defeat:
            return TEXT("Defeat!");
        case EBattleState::Paused:
            return TEXT("Paused");
        default:
            return TEXT("Unknown");
    }
}

void UBattleHUDWidget::UpdateAllUI()
{
    if (BattleManager)
    {
        ACombatUnit* Unit = BattleManager->GetCurrentUnit();
        bool bIsPlayerTurn = BattleManager->IsPlayerTurn();
        
        UpdateActionButtons(bIsPlayerTurn, Unit);
        UpdatePositionButtons(Unit);
        
        if (Unit)
        {
            UpdateDefenseButtons(Unit->CurrentPosition);
        }
    }
}