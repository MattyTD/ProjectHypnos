// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Units/CombatUnit.h"
#include "BattleManager.generated.h"

UENUM(BlueprintType)
enum class EBattleState : uint8
{
    PlayerTurn,
    EnemyTurn,
    Victory,
    Defeat,
    Paused
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
    Move,
    Attack,
    Skill,
    Item,
    Pass,
    Ranti
};

USTRUCT(BlueprintType)
struct FBattleAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    ACombatUnit* ActingUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    EActionType ActionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    EBattlePosition TargetPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    ACombatUnit* TargetUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
    FString SkillName;

    FBattleAction()
    {
        ActingUnit = nullptr;
        ActionType = EActionType::Pass;
        TargetPosition = EBattlePosition::West;
        TargetUnit = nullptr;
        SkillName = "";
    }
};

UCLASS(Blueprintable)
class PROJECTHYPNOS_API ABattleManager : public AActor
{
    GENERATED_BODY()

public:
    ABattleManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Battle State
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    EBattleState CurrentBattleState = EBattleState::PlayerTurn;

    // Units
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<ACombatUnit*> PlayerUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<ACombatUnit*> EnemyUnits;

    // Turn Management
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CurrentUnitIndex = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CurrentSetNumber = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsSetComplete = false;

    // Timer Management
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float BaseTimerDuration = 12.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float CurrentTimerRemaining = 12.0f;

    // TFN (Time For Now) System
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bTFNActive = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float TFNSpeedMultiplier = 1.0f;

    // Ranti Skills
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FString> AvailableRantiSkills;

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnBattleStateChanged(EBattleState NewState);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnUnitTurnStarted(ACombatUnit* Unit);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnUnitTurnEnded(ACombatUnit* Unit);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnSetComplete();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnEnemyTurnStarted();

    // Core Functions
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartBattle();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EndCurrentUnitTurn();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void PassTurn();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void RetryBattle();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ExecuteAction(const FBattleAction& Action);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void MoveUnit(ACombatUnit* Unit, EBattlePosition NewPosition);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackUnit(ACombatUnit* Attacker, ACombatUnit* Target, EElementalType ElementType = EElementalType::Physical);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void UseSkill(ACombatUnit* Caster, const FString& SkillName, ACombatUnit* Target = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void UseRantiSkill(const FString& SkillName, TArray<ACombatUnit*> RequiredUnits);

    // TFN/SP System
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyTFNToNextUnit(float SpeedMultiplier);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AddStockpiledTime(ACombatUnit* Unit, float TimeToAdd);

    // Battle Flow
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void CheckSetCompletion();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartEnemyTurn();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EndEnemyTurn();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartNewSet();

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "Combat")
    ACombatUnit* GetCurrentUnit() const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool IsPlayerTurn() const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool IsSetComplete() const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void PauseBattle();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ResumeBattle();

    // Animation gating for timers
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void BeginActionAnimation();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EndActionAnimation();

protected:
    void InitializePlayerUnits();
    void InitializeEnemyUnits();
    void StartNextUnitTurn();
    void HandleWeaknessHit(ACombatUnit* Attacker, ACombatUnit* Target, EElementalType ElementType);
    bool CheckBattleEndConditions();

    // If true, the current unit's timer should not tick (e.g., during attack/skill animations)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsActionAnimationPlaying = false;
};