// CombatUnit.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"
#include "CombatUnit.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Player,
    Enemy
};

UENUM(BlueprintType)
enum class EBattlePosition : uint8
{
    North,
    East,
    South,
    West,
    Center  // For enemies
};

UENUM(BlueprintType)
enum class EElementalType : uint8
{
    None,
    Fire,
    Water,
    Earth,
    Air,
    Light,
    Dark,
    Physical
};

UENUM(BlueprintType)
enum class EDefenseType : uint8
{
    None,
    Guard,
    Dodge,
    Parry
};

USTRUCT(BlueprintType)
struct PROJECTHYPNOS_API FElementalResistance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
    EElementalType ElementType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
    float ResistanceMultiplier;

    FElementalResistance()
    {
        ElementType = EElementalType::None;
        ResistanceMultiplier = 1.0f;
    }
};

UCLASS(Blueprintable)
class PROJECTHYPNOS_API ACombatUnit : public AActor
{
    GENERATED_BODY()

public:
    ACombatUnit();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Basic Unit Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FString UnitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EUnitType UnitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EBattlePosition CurrentPosition = EBattlePosition::West;

    // Health System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MaxHP = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float CurrentHP;

    // Timer System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float TimerDuration = 12.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float TimerRemaining;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float TimerTickRate = 1.0f; // For TFN mechanic

    // SP (Stockpile) System
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float StockpiledTime = 0.0f;

    // EO (Egbe Orun) System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MaxEO = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float CurrentEO = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsInEOForm = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float EOGainRate = 1.0f; // Modified by SO debuff

    // MP System (only available in EO form)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MaxMP = 50.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float CurrentMP = 0.0f;

    // Status Effects
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsStressedOut = false;

    // Incapacitation (e.g., stun/sleep). Incapacitated or KO'd units cannot act
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsIncapacitated = false;

    // Elemental System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FElementalResistance> ElementalResistances;

    // Defense System
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    EDefenseType CurrentDefenseType = EDefenseType::None;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ResetTimer();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ResetForBattle();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool CanAct() const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetIncapacitated(bool bIncapacitated);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetPosition(EBattlePosition NewPosition);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyTFN(float SpeedMultiplier);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AddStockpiledTime(float TimeToAdd);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void GainEO(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool CanTransformToEO() const;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TransformToEO();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ExitEOForm(bool bForced = false);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    float GetElementalDamageMultiplier(EElementalType AttackElement) const;

    // Override the AActor TakeDamage function properly
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // Custom TakeDamage function for your battle system
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TakeDamageCustom(float DamageAmount, EElementalType ElementType = EElementalType::Physical);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyStressedOut();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetDefenseType(EDefenseType DefenseType);
};