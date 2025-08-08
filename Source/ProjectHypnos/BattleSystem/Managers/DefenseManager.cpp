// DefenseManager.cpp
#include "DefenseManager.h"
#include "PositionManager.h"
#include "BattleManager.h"
#include "Kismet/GameplayStatics.h"

ADefenseManager::ADefenseManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ADefenseManager::BeginPlay()
{
    Super::BeginPlay();
    InitializeManagers();
}

void ADefenseManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADefenseManager::InitializeManagers()
{
    // Find the position manager in the world
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APositionManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        PositionManager = Cast<APositionManager>(FoundActors[0]);
    }

    // Find the battle manager in the world
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        BattleManager = Cast<ABattleManager>(FoundActors[0]);
    }
}

bool ADefenseManager::CanUseGuard(EBattlePosition Position) const
{
    if (!PositionManager) return false;
    
    // Guard can only be used when multiple units are in the same position
    int32 UnitCount = PositionManager->GetUnitCountAtPosition(Position);
    return UnitCount >= 2;
}

bool ADefenseManager::CanUseParry(EBattlePosition Position) const
{
    if (!PositionManager) return false;
    
    // Parry can only be used when a single unit is alone in a position
    int32 UnitCount = PositionManager->GetUnitCountAtPosition(Position);
    return UnitCount == 1;
}

bool ADefenseManager::CanUseDodge(EBattlePosition Position) const
{
    if (!PositionManager) return false;
    
    // Dodge can be used in any position
    return true;
}

FDefenseAttempt ADefenseManager::AttemptGuard(ACombatUnit* Unit, float TimingAccuracy)
{
    FDefenseAttempt Attempt;
    Attempt.DefendingUnit = Unit;
    Attempt.DefenseType = EDefenseType::Guard;
    Attempt.TimingAccuracy = TimingAccuracy;
    Attempt.Result = EDefenseResult::Success; // Guard always works

    if (Unit)
    {
        Unit->SetDefenseType(EDefenseType::Guard);
        OnDefenseSuccess(Unit, EDefenseType::Guard);
    }

    OnDefenseAttempt(Attempt);
    return Attempt;
}

FDefenseAttempt ADefenseManager::AttemptDodge(ACombatUnit* Unit, float TimingAccuracy)
{
    FDefenseAttempt Attempt;
    Attempt.DefendingUnit = Unit;
    Attempt.DefenseType = EDefenseType::Dodge;
    Attempt.TimingAccuracy = TimingAccuracy;
    Attempt.Result = EvaluateDefenseResult(EDefenseType::Dodge, TimingAccuracy);

    if (Unit)
    {
        Unit->SetDefenseType(EDefenseType::Dodge);
        
        if (Attempt.Result == EDefenseResult::Success || Attempt.Result == EDefenseResult::Partial)
        {
            OnDefenseSuccess(Unit, EDefenseType::Dodge);
        }
        else
        {
            OnDefenseFailure(Unit, EDefenseType::Dodge);
        }
    }

    OnDefenseAttempt(Attempt);
    return Attempt;
}

FDefenseAttempt ADefenseManager::AttemptParry(ACombatUnit* Unit, float TimingAccuracy)
{
    FDefenseAttempt Attempt;
    Attempt.DefendingUnit = Unit;
    Attempt.DefenseType = EDefenseType::Parry;
    Attempt.TimingAccuracy = TimingAccuracy;
    Attempt.Result = EvaluateDefenseResult(EDefenseType::Parry, TimingAccuracy);

    if (Unit)
    {
        Unit->SetDefenseType(EDefenseType::Parry);
        
        if (Attempt.Result == EDefenseResult::Success || Attempt.Result == EDefenseResult::Counter)
        {
            OnDefenseSuccess(Unit, EDefenseType::Parry);
        }
        else
        {
            OnDefenseFailure(Unit, EDefenseType::Parry);
        }
    }

    OnDefenseAttempt(Attempt);
    return Attempt;
}

float ADefenseManager::CalculateDamageReduction(const FDefenseAttempt& Attempt) const
{
    switch (Attempt.DefenseType)
    {
        case EDefenseType::Guard:
            return GuardDamageReduction;
            
        case EDefenseType::Dodge:
            if (Attempt.Result == EDefenseResult::Success)
                return DodgeDamageReduction;
            else if (Attempt.Result == EDefenseResult::Partial)
                return DodgeDamageReduction * 0.5f; // Partial dodge
            else
                return 0.0f; // Failed dodge
            
        case EDefenseType::Parry:
            if (Attempt.Result == EDefenseResult::Success || Attempt.Result == EDefenseResult::Counter)
                return ParryDamageReduction;
            else
                return 0.0f; // Failed parry
            
        default:
            return 0.0f;
    }
}

float ADefenseManager::CalculateEOGain(const FDefenseAttempt& Attempt) const
{
    float BaseEOGain = 0.0f;
    
    switch (Attempt.DefenseType)
    {
        case EDefenseType::Guard:
            BaseEOGain = GuardEOGain;
            break;
        case EDefenseType::Dodge:
            BaseEOGain = DodgeEOGain;
            break;
        case EDefenseType::Parry:
            BaseEOGain = ParryEOGain;
            break;
        default:
            return 0.0f;
    }

    // Apply timing accuracy modifier
    float TimingMultiplier = 1.0f - Attempt.TimingAccuracy; // Better timing = more EO
    return BaseEOGain * TimingMultiplier;
}

bool ADefenseManager::ShouldTriggerCounter(const FDefenseAttempt& Attempt) const
{
    return Attempt.DefenseType == EDefenseType::Parry && 
           Attempt.Result == EDefenseResult::Counter;
}

float ADefenseManager::GetDefenseDifficulty(EDefenseType DefenseType, EBattlePosition Position) const
{
    float BaseDifficulty = 1.0f;
    
    switch (DefenseType)
    {
        case EDefenseType::Dodge:
            // Dodge is easier when alone
            if (PositionManager && PositionManager->GetUnitCountAtPosition(Position) == 1)
            {
                BaseDifficulty = 0.5f; // Easier when alone
            }
            break;
            
        case EDefenseType::Parry:
            // Parry is always difficult
            BaseDifficulty = 2.0f;
            break;
            
        case EDefenseType::Guard:
            // Guard is always easy
            BaseDifficulty = 0.1f;
            break;
            
        default:
            break;
    }
    
    return BaseDifficulty;
}

EDefenseResult ADefenseManager::EvaluateDefenseResult(EDefenseType DefenseType, float TimingAccuracy) const
{
    switch (DefenseType)
    {
        case EDefenseType::Dodge:
            if (TimingAccuracy <= DodgePerfectWindow)
                return EDefenseResult::Success;
            else if (TimingAccuracy <= DodgeGoodWindow)
                return EDefenseResult::Partial;
            else
                return EDefenseResult::Failure;
                
        case EDefenseType::Parry:
            if (TimingAccuracy <= ParryPerfectWindow)
                return EDefenseResult::Counter; // Perfect parry = counter attack
            else if (TimingAccuracy <= ParryGoodWindow)
                return EDefenseResult::Success;
            else
                return EDefenseResult::Failure;
                
        case EDefenseType::Guard:
            return EDefenseResult::Success; // Guard always works
            
        default:
            return EDefenseResult::Failure;
    }
}

void ADefenseManager::ProcessDefenseAttempt(const FDefenseAttempt& Attempt, ACombatUnit* Attacker, float Damage)
{
    if (!Attempt.DefendingUnit) return;

    // Calculate damage reduction
    float DamageReduction = CalculateDamageReduction(Attempt);
    float FinalDamage = Damage * (1.0f - DamageReduction);

    // Apply damage
    Attempt.DefendingUnit->TakeDamageCustom(FinalDamage, EElementalType::Physical);

    // Award EO for successful defense
    float EOGain = CalculateEOGain(Attempt);
    if (EOGain > 0.0f)
    {
        Attempt.DefendingUnit->GainEO(EOGain);
    }

    // Handle counter attack
    if (ShouldTriggerCounter(Attempt) && Attacker)
    {
        OnCounterAttack(Attempt.DefendingUnit, Attacker);
        
        // Perform counter attack
        if (BattleManager)
        {
            BattleManager->AttackUnit(Attempt.DefendingUnit, Attacker, EElementalType::Physical);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("%s defended against %s's attack. Damage: %f -> %f, EO gained: %f"), 
           *Attempt.DefendingUnit->UnitName, 
           Attacker ? *Attacker->UnitName : TEXT("Unknown"), 
           Damage, FinalDamage, EOGain);
}

float ADefenseManager::GetPositionMultiplier(EBattlePosition Position) const
{
    // Position can affect defense difficulty
    switch (Position)
    {
        case EBattlePosition::North:
        case EBattlePosition::South:
            return 1.0f; // Standard difficulty
        case EBattlePosition::East:
        case EBattlePosition::West:
            return 1.2f; // Slightly harder
        default:
            return 1.0f;
    }
}
