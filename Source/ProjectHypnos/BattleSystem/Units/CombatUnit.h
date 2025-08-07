// CombatUnit.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"            // or Character if you need movement
#include "CombatUnit.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Player,
    Enemy
};

UCLASS(Blueprintable)                     // ← Make it Blueprintable
class PROJECTHYPNOS_API ACombatUnit : public AActor
{
    GENERATED_BODY()

public:
    ACombatUnit();

    virtual void BeginPlay() override;

    /** Name shown in the HUD */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FString UnitName;

    /** Is this a player or an enemy? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EUnitType UnitType;

    /** Maximum health */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MaxHP = 100.0f;

    /** Current health */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float CurrentHP;

    /** How many seconds this unit starts with each set */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float TimerDuration = 12.0f;

    /** How many seconds remain */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    float TimerRemaining;

    /** Resets TimerRemaining = TimerDuration */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ResetTimer();

    /** True if CurrentHP > 0 */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool IsAlive() const;
};