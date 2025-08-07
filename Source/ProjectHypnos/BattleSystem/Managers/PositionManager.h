// PositionManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Units/CombatUnit.h"
#include "PositionManager.generated.h"

USTRUCT(BlueprintType)
struct FBattlePositionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
    EBattlePosition Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
    FVector WorldLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position")
    TArray<ACombatUnit*> UnitsAtPosition;

    FBattlePositionInfo()
    {
        Position = EBattlePosition::West;
        WorldLocation = FVector::ZeroVector;
    }
};

UCLASS(Blueprintable)
class PROJECTHYPNOS_API APositionManager : public AActor
{
    GENERATED_BODY()

public:
    APositionManager();

    virtual void BeginPlay() override;

    // Position data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    TArray<FBattlePositionInfo> BattlePositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    FVector CenterPosition; // Where enemies are located

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions")
    float PositionRadius = 500.0f; // How far positions are from center

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Position")
    void InitializePositions();

    UFUNCTION(BlueprintCallable, Category = "Position")
    bool MoveUnitToPosition(ACombatUnit* Unit, EBattlePosition NewPosition);

    UFUNCTION(BlueprintCallable, Category = "Position")
    TArray<ACombatUnit*> GetUnitsAtPosition(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Position")
    int32 GetUnitCountAtPosition(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Position")
    FVector GetPositionWorldLocation(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Position")
    bool CanUseGuard(EBattlePosition Position) const;

    UFUNCTION(BlueprintCallable, Category = "Position")
    bool CanUseParry(EBattlePosition Position) const;

protected:
    void RemoveUnitFromAllPositions(ACombatUnit* Unit);
    FBattlePositionInfo* GetPositionInfo(EBattlePosition Position);
};