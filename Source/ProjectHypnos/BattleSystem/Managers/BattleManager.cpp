// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleManager.h"
#include "../Units/CombatUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ABattleManager::ABattleManager()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentBattleState = EBattleState::PlayerTurn;
    CurrentUnitIndex = 0;
    CurrentSetNumber = 1;
    bIsSetComplete = false;
    BaseTimerDuration = 12.0f;
    CurrentTimerRemaining = BaseTimerDuration;
    bTFNActive = false;
    TFNSpeedMultiplier = 1.0f;
}

void ABattleManager::BeginPlay()
{
    Super::BeginPlay();
    InitializePlayerUnits();
    InitializeEnemyUnits();
}

void ABattleManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentBattleState == EBattleState::PlayerTurn && !bIsSetComplete)
    {
        // During action animations, do not tick the timer
        if (!bIsActionAnimationPlaying)
        {
            // Apply TFN effect to timer and decrement the active unit's remaining time
            float ActualDeltaTime = DeltaTime * TFNSpeedMultiplier;

            if (ACombatUnit* ActiveUnit = GetCurrentUnit())
            {
                ActiveUnit->TimerRemaining = FMath::Max(0.0f, ActiveUnit->TimerRemaining - ActualDeltaTime);
                CurrentTimerRemaining = ActiveUnit->TimerRemaining;
            }
        }

        // Check if current unit's time is up
        if (CurrentTimerRemaining <= 0.0f)
        {
            EndCurrentUnitTurn();
        }
    }

    // Check battle end conditions
    if (CheckBattleEndConditions())
    {
        return;
    }
}

void ABattleManager::StartBattle()
{
    CurrentBattleState = EBattleState::PlayerTurn;
    CurrentUnitIndex = 0;
    CurrentSetNumber = 1;
    bIsSetComplete = false;
    CurrentTimerRemaining = BaseTimerDuration;
    bTFNActive = false;
    TFNSpeedMultiplier = 1.0f;

    // Reset all player units
    for (ACombatUnit* Unit : PlayerUnits)
    {
        if (Unit)
        {
            Unit->ResetForBattle();
            Unit->SetPosition(EBattlePosition::West); // Start all units in West
        }
    }

    // Start first unit's turn
    StartNextUnitTurn();
    OnBattleStateChanged(CurrentBattleState);
}

void ABattleManager::EndCurrentUnitTurn()
{
    ACombatUnit* CurrentUnit = GetCurrentUnit();
    if (CurrentUnit)
    {
        OnUnitTurnEnded(CurrentUnit);
    }

    // Move to next unit
    CurrentUnitIndex++;

    // Check if we've completed a full cycle through all units
    if (CurrentUnitIndex >= PlayerUnits.Num())
    {
        // Check if any units still have time remaining
        bool bAnyUnitHasTime = false;
        for (ACombatUnit* Unit : PlayerUnits)
        {
            if (Unit && Unit->TimerRemaining > 0.0f)
            {
                bAnyUnitHasTime = true;
                break;
            }
        }

        if (bAnyUnitHasTime)
        {
            // Continue the set with remaining time
            CurrentUnitIndex = 0;
            StartNextUnitTurn();
        }
        else
        {
            // Set is complete
            bIsSetComplete = true;
            OnSetComplete();
            StartEnemyTurn();
        }
    }
    else
    {
        StartNextUnitTurn();
    }
}

void ABattleManager::PassTurn()
{
    // Explicit pass command from input/UI
    EndCurrentUnitTurn();
}

void ABattleManager::StartNextUnitTurn()
{
    ACombatUnit* CurrentUnit = GetCurrentUnit();
    if (CurrentUnit)
    {
        // If unit cannot act (KO or incapacitated), immediately skip
        if (!CurrentUnit->CanAct())
        {
            EndCurrentUnitTurn();
            return;
        }

        // Reset timer for this unit
        CurrentTimerRemaining = CurrentUnit->TimerRemaining;
        
        // Apply TFN effect if active
        if (bTFNActive)
        {
            CurrentUnit->ApplyTFN(TFNSpeedMultiplier);
            bTFNActive = false; // Reset TFN after applying
        }

        OnUnitTurnStarted(CurrentUnit);
    }
}

void ABattleManager::ExecuteAction(const FBattleAction& Action)
{
    if (!Action.ActingUnit) return;

    switch (Action.ActionType)
    {
        case EActionType::Move:
            MoveUnit(Action.ActingUnit, Action.TargetPosition);
            break;
        case EActionType::Attack:
            if (Action.TargetUnit)
            {
                AttackUnit(Action.ActingUnit, Action.TargetUnit);
            }
            break;
        case EActionType::Skill:
            UseSkill(Action.ActingUnit, Action.SkillName, Action.TargetUnit);
            break;
        case EActionType::Item:
            // TODO: Implement item system
            break;
        case EActionType::Pass:
            // Just pass turn
            break;
        case EActionType::Ranti:
            // TODO: Implement Ranti skill system
            break;
    }
}

void ABattleManager::MoveUnit(ACombatUnit* Unit, EBattlePosition NewPosition)
{
    if (!Unit) return;

    Unit->SetPosition(NewPosition);
    UE_LOG(LogTemp, Log, TEXT("%s moved to position %d"), *Unit->UnitName, (int32)NewPosition);
}

void ABattleManager::AttackUnit(ACombatUnit* Attacker, ACombatUnit* Target, EElementalType ElementType)
{
    if (!Attacker || !Target) return;

    // Calculate damage (simplified for now)
    float BaseDamage = 20.0f;
    
    // Apply elemental damage
    float ElementalMultiplier = Target->GetElementalDamageMultiplier(ElementType);
    float FinalDamage = BaseDamage * ElementalMultiplier;

    // Apply damage
    Target->TakeDamageCustom(FinalDamage, ElementType);

    // Check for weakness hit
    if (ElementalMultiplier > 1.0f)
    {
        HandleWeaknessHit(Attacker, Target, ElementType);
    }

    UE_LOG(LogTemp, Log, TEXT("%s attacked %s for %f damage (Elemental Multiplier: %f)"), 
           *Attacker->UnitName, *Target->UnitName, FinalDamage, ElementalMultiplier);
}

void ABattleManager::UseSkill(ACombatUnit* Caster, const FString& SkillName, ACombatUnit* Target)
{
    if (!Caster) return;

    // TODO: Implement skill system
    UE_LOG(LogTemp, Log, TEXT("%s used skill: %s"), *Caster->UnitName, *SkillName);
}

void ABattleManager::UseRantiSkill(const FString& SkillName, TArray<ACombatUnit*> RequiredUnits)
{
    // TODO: Implement Ranti skill system
    UE_LOG(LogTemp, Log, TEXT("Ranti skill used: %s"), *SkillName);
}

void ABattleManager::ApplyTFNToNextUnit(float SpeedMultiplier)
{
    bTFNActive = true;
    TFNSpeedMultiplier = FMath::Max(0.25f, SpeedMultiplier); // Capped at 0.25x
    UE_LOG(LogTemp, Log, TEXT("TFN applied to next unit with speed multiplier: %f"), TFNSpeedMultiplier);
}

void ABattleManager::AddStockpiledTime(ACombatUnit* Unit, float TimeToAdd)
{
    if (Unit)
    {
        Unit->AddStockpiledTime(TimeToAdd);
    }
}

void ABattleManager::HandleWeaknessHit(ACombatUnit* Attacker, ACombatUnit* Target, EElementalType ElementType)
{
    // Add SP time to attacker
    float SPGain = 2.0f; // Base SP gain for weakness hit
    AddStockpiledTime(Attacker, SPGain);

    // Apply TFN to next unit in turn order
    const float NextUnitSpeedMultiplier = 0.75f; // Slow down next unit
    ApplyTFNToNextUnit(NextUnitSpeedMultiplier);

    UE_LOG(LogTemp, Warning, TEXT("Weakness hit! %s gained %f SP, TFN applied to next unit"), 
           *Attacker->UnitName, SPGain);
}

void ABattleManager::CheckSetCompletion()
{
    bool bAllUnitsOutOfTime = true;
    for (ACombatUnit* Unit : PlayerUnits)
    {
        if (Unit && Unit->TimerRemaining > 0.0f)
        {
            bAllUnitsOutOfTime = false;
            break;
        }
    }

    if (bAllUnitsOutOfTime)
    {
        bIsSetComplete = true;
        OnSetComplete();
        StartEnemyTurn();
    }
}

void ABattleManager::StartEnemyTurn()
{
    CurrentBattleState = EBattleState::EnemyTurn;
    OnEnemyTurnStarted();
    OnBattleStateChanged(CurrentBattleState);

    // TODO: Implement enemy AI logic
    UE_LOG(LogTemp, Warning, TEXT("Enemy turn started!"));

    // For now, just end enemy turn immediately
    EndEnemyTurn();
}

void ABattleManager::EndEnemyTurn()
{
    CurrentBattleState = EBattleState::PlayerTurn;
    CurrentUnitIndex = 0;
    CurrentSetNumber++;
    bIsSetComplete = false;

    // Reset all player units for new set
    for (ACombatUnit* Unit : PlayerUnits)
    {
        if (Unit)
        {
            Unit->ResetTimer();
        }
    }

    // Start new set
    StartNewSet();
}

void ABattleManager::RetryBattle()
{
    // Reset battle state to beginning
    CurrentBattleState = EBattleState::PlayerTurn;
    CurrentUnitIndex = 0;
    CurrentSetNumber = 1;
    bIsSetComplete = false;
    bTFNActive = false;
    TFNSpeedMultiplier = 1.0f;
    bIsActionAnimationPlaying = false;

    for (ACombatUnit* Unit : PlayerUnits)
    {
        if (Unit)
        {
            Unit->ResetForBattle();
            Unit->SetPosition(EBattlePosition::West);
        }
    }

    for (ACombatUnit* Enemy : EnemyUnits)
    {
        if (Enemy)
        {
            Enemy->ResetForBattle();
            Enemy->SetPosition(EBattlePosition::Center);
        }
    }

    StartNextUnitTurn();
    OnBattleStateChanged(CurrentBattleState);
}

void ABattleManager::StartNewSet()
{
    CurrentTimerRemaining = BaseTimerDuration;
    bTFNActive = false;
    TFNSpeedMultiplier = 1.0f;

    StartNextUnitTurn();
    OnBattleStateChanged(CurrentBattleState);
}

ACombatUnit* ABattleManager::GetCurrentUnit() const
{
    if (PlayerUnits.IsValidIndex(CurrentUnitIndex))
    {
        return PlayerUnits[CurrentUnitIndex];
    }
    return nullptr;
}

bool ABattleManager::IsPlayerTurn() const
{
    return CurrentBattleState == EBattleState::PlayerTurn;
}

bool ABattleManager::IsSetComplete() const
{
    return bIsSetComplete;
}

void ABattleManager::PauseBattle()
{
    CurrentBattleState = EBattleState::Paused;
    OnBattleStateChanged(CurrentBattleState);
}

void ABattleManager::ResumeBattle()
{
    CurrentBattleState = EBattleState::PlayerTurn;
    OnBattleStateChanged(CurrentBattleState);
}

void ABattleManager::BeginActionAnimation()
{
    bIsActionAnimationPlaying = true;
}

void ABattleManager::EndActionAnimation()
{
    bIsActionAnimationPlaying = false;
}

void ABattleManager::InitializePlayerUnits()
{
    // TODO: Initialize player units from level or save data
    UE_LOG(LogTemp, Log, TEXT("Initializing player units"));
}

void ABattleManager::InitializeEnemyUnits()
{
    // TODO: Initialize enemy units from level or spawn system
    UE_LOG(LogTemp, Log, TEXT("Initializing enemy units"));
}

bool ABattleManager::CheckBattleEndConditions()
{
    // Check if all player units are defeated
    bool bAllPlayersDefeated = true;
    for (ACombatUnit* Unit : PlayerUnits)
    {
        if (Unit && Unit->IsAlive())
        {
            bAllPlayersDefeated = false;
            break;
        }
    }

    if (bAllPlayersDefeated)
    {
        CurrentBattleState = EBattleState::Defeat;
        OnBattleStateChanged(CurrentBattleState);
        return true;
    }

    // Check if all enemy units are defeated
    bool bAllEnemiesDefeated = true;
    for (ACombatUnit* Unit : EnemyUnits)
    {
        if (Unit && Unit->IsAlive())
        {
            bAllEnemiesDefeated = false;
            break;
        }
    }

    if (bAllEnemiesDefeated)
    {
        CurrentBattleState = EBattleState::Victory;
        OnBattleStateChanged(CurrentBattleState);
        return true;
    }

    return false;
}