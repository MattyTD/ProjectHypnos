// DefenseManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Units/CombatUnit.h"
#include "DefenseManager.generated.h"

UENUM(BlueprintType)
enum class EDefenseResult : uint8
{
    Success,
    Partial,
    Failure,
    Counter
};

USTRUCT(BlueprintType)
struct FDefenseAttempt
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    ACombatUnit* DefendingUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    EDefenseType DefenseType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float TimingAccuracy; // 0.0 = perfect, 1.0 = completely missed

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    EDefenseResult Result;

    FDefenseAttempt()
    {
        DefendingUnit = nullptr;
        DefenseType = EDefenseType::None;
        TimingAccuracy = 1.0f;
        Result = EDefenseResult::Failure;
    }
};

UCLASS(Blueprintable)
class PROJECTHYPNOS_API ADefenseManager : public AActor
{
    GENERATED_BODY()

public:
    ADefenseManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Defense timing windows
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float DodgePerfectWindow = 0.1f; // 100ms perfect window

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float DodgeGoodWindow = 0.3f; // 300ms good window

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float ParryPerfectWindow = 0.05f; // 50ms perfect window

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float ParryGoodWindow = 0.15f; // 150ms good window

    // Defense effectiveness
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float GuardDamageReduction = 0.5f; // 50% damage reduction

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float DodgeDamageReduction = 1.0f; // 100% damage reduction (when successful)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float ParryDamageReduction = 0.0f; // 0% damage taken (when successful)

    // EO gain rates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float GuardEOGain = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float DodgeEOGain = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
    float ParryEOGain = 25.0f;

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Defense")
    void OnDefenseAttempt(const FDefenseAttempt& Attempt);

    UFUNCTION(BlueprintImplementableEvent, Category = "Defense")
    void OnDefenseSuccess(ACombatUnit* Unit, EDefenseType DefenseType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Defense")
    void OnDefenseFailure(ACombatUnit* Unit, EDefenseType DefenseType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Defense")
    void OnCounterAttack(ACombatUnit* DefendingUnit, ACombatUnit* AttackingUnit);

    // Core Functions
    UFUNCTION(BlueprintCallable, Category = "Defense")
    bool CanUseGuard(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    bool CanUseParry(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    bool CanUseDodge(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    FDefenseAttempt AttemptGuard(ACombatUnit* Unit, float TimingAccuracy = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Defense")
    FDefenseAttempt AttemptDodge(ACombatUnit* Unit, float TimingAccuracy = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Defense")
    FDefenseAttempt AttemptParry(ACombatUnit* Unit, float TimingAccuracy = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Defense")
    float CalculateDamageReduction(const FDefenseAttempt& Attempt) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    float CalculateEOGain(const FDefenseAttempt& Attempt) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    bool ShouldTriggerCounter(const FDefenseAttempt& Attempt) const;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "Defense")
    float GetDefenseDifficulty(EDefenseType DefenseType, EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    EDefenseResult EvaluateDefenseResult(EDefenseType DefenseType, float TimingAccuracy) const;

    UFUNCTION(BlueprintCallable, Category = "Defense")
    void ProcessDefenseAttempt(const FDefenseAttempt& Attempt, ACombatUnit* Attacker, float Damage);

protected:
    class APositionManager* PositionManager;
    class ABattleManager* BattleManager;

    void InitializeManagers();
    float GetPositionMultiplier(EBattlePosition Position) const;
};
