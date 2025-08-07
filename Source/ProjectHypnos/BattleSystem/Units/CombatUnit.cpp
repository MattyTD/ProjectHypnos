// CombatUnit.cpp
#include "CombatUnit.h"
#include "Engine/DamageEvents.h"

ACombatUnit::ACombatUnit()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentHP = MaxHP;
    TimerRemaining = TimerDuration;
    CurrentEO = 0.0f;
    CurrentMP = 0.0f;
    TimerTickRate = 1.0f;
    EOGainRate = 1.0f;
    bIsInEOForm = false;
    bIsStressedOut = false;
}

void ACombatUnit::BeginPlay()
{
    Super::BeginPlay();
    CurrentHP = MaxHP;
    CurrentMP = MaxMP; // Will be 0 unless in EO form
}

void ACombatUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Passive EO gain over time (very small)
    if (!bIsInEOForm && IsAlive())
    {
        GainEO(5.0f * DeltaTime * EOGainRate); // 5 EO per second base rate
    }
}

void ACombatUnit::ResetTimer()
{
    TimerRemaining = TimerDuration + StockpiledTime;
    StockpiledTime = 0.0f; // Reset stockpiled time after using it
    TimerTickRate = 1.0f; // Reset TFN effect
}

bool ACombatUnit::IsAlive() const
{
    return CurrentHP > 0;
}

void ACombatUnit::SetPosition(EBattlePosition NewPosition)
{
    CurrentPosition = NewPosition;
    UE_LOG(LogTemp, Log, TEXT("%s moved to position %d"), *UnitName, (int32)NewPosition);
}

void ACombatUnit::ApplyTFN(float SpeedMultiplier)
{
    TimerTickRate = FMath::Max(0.25f, SpeedMultiplier); // Capped at 0.25x speed
    UE_LOG(LogTemp, Log, TEXT("%s timer speed set to %f"), *UnitName, TimerTickRate);
}

void ACombatUnit::AddStockpiledTime(float TimeToAdd)
{
    StockpiledTime += TimeToAdd;
    UE_LOG(LogTemp, Log, TEXT("%s gained %f stockpiled time (Total: %f)"), *UnitName, TimeToAdd, StockpiledTime);
}

void ACombatUnit::GainEO(float Amount)
{
    if (bIsInEOForm) return; // Can't gain EO while in EO form

    float ActualGain = Amount * EOGainRate;
    CurrentEO = FMath::Clamp(CurrentEO + ActualGain, 0.0f, MaxEO);

    if (CurrentEO >= MaxEO)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s EO bar is full! Can transform!"), *UnitName);
    }
}

bool ACombatUnit::CanTransformToEO() const
{
    return CurrentEO >= MaxEO && !bIsInEOForm && IsAlive();
}

void ACombatUnit::TransformToEO()
{
    if (!CanTransformToEO()) return;

    bIsInEOForm = true;
    CurrentMP = MaxMP; // Gain access to MP

    // TODO: Apply stat boosts here
    // TODO: Change visual representation later

    UE_LOG(LogTemp, Warning, TEXT("%s transformed into EO form!"), *UnitName);
}

void ACombatUnit::ExitEOForm(bool bForced)
{
    if (!bIsInEOForm) return;

    bIsInEOForm = false;
    CurrentEO = 0.0f;
    CurrentMP = 0.0f;

    if (bForced)
    {
        ApplyStressedOut();
        UE_LOG(LogTemp, Error, TEXT("%s was forced out of EO form and is now Stressed Out!"), *UnitName);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("%s exited EO form normally"), *UnitName);
    }
}

float ACombatUnit::GetElementalDamageMultiplier(EElementalType AttackElement) const
{
    for (const FElementalResistance& Resistance : ElementalResistances)
    {
        if (Resistance.ElementType == AttackElement)
        {
            return Resistance.ResistanceMultiplier;
        }
    }
    return 1.0f; // No special resistance/weakness
}

// Override AActor's TakeDamage function
float ACombatUnit::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    // Call the custom damage function with Physical damage type as default
    TakeDamageCustom(DamageAmount, EElementalType::Physical);

    // Return the amount of damage actually applied (for AActor interface compatibility)
    return DamageAmount;
}

// Custom damage function for your battle system
void ACombatUnit::TakeDamageCustom(float DamageAmount, EElementalType ElementType)
{
    if (!IsAlive()) return;

    float Multiplier = GetElementalDamageMultiplier(ElementType);
    float FinalDamage = DamageAmount * Multiplier;

    if (bIsInEOForm)
    {
        // In EO form, damage goes to EO bar instead of HP
        CurrentEO -= FinalDamage;
        if (CurrentEO <= 0.0f)
        {
            ExitEOForm(true); // Forced exit
        }
    }
    else
    {
        // Normal damage to HP
        CurrentHP = FMath::Max(0.0f, CurrentHP - FinalDamage);

        if (CurrentHP <= 0.0f)
        {
            UE_LOG(LogTemp, Error, TEXT("%s has been defeated!"), *UnitName);
        }
    }

    // Check if this was a weakness hit
    if (Multiplier > 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weakness hit on %s! Damage multiplier: %f"), *UnitName, Multiplier);
    }
}

void ACombatUnit::ApplyStressedOut()
{
    bIsStressedOut = true;
    EOGainRate = 0.5f; // Slower EO gain

    // Set HP to 25% if it's higher than that
    float TwentyFivePercent = MaxHP * 0.25f;
    if (CurrentHP > TwentyFivePercent)
    {
        CurrentHP = TwentyFivePercent;
    }

    UE_LOG(LogTemp, Error, TEXT("%s is now Stressed Out! EO gain reduced, HP set to 25%%"), *UnitName);
}

void ACombatUnit::SetDefenseType(EDefenseType DefenseType)
{
    CurrentDefenseType = DefenseType;
}