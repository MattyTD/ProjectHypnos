// PositionManager.cpp
#include "PositionManager.h"

APositionManager::APositionManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CenterPosition = FVector(0.0f, 0.0f, 0.0f);
    PositionRadius = 500.0f;
}

void APositionManager::BeginPlay()
{
    Super::BeginPlay();
    InitializePositions();
}

void APositionManager::InitializePositions()
{
    BattlePositions.Empty();

    // Create the four positions around center
    FBattlePositionInfo NorthPos;
    NorthPos.Position = EBattlePosition::North;
    NorthPos.WorldLocation = CenterPosition + FVector(0.0f, PositionRadius, 0.0f);

    FBattlePositionInfo EastPos;
    EastPos.Position = EBattlePosition::East;
    EastPos.WorldLocation = CenterPosition + FVector(PositionRadius, 0.0f, 0.0f);

    FBattlePositionInfo SouthPos;
    SouthPos.Position = EBattlePosition::South;
    SouthPos.WorldLocation = CenterPosition + FVector(0.0f, -PositionRadius, 0.0f);

    FBattlePositionInfo WestPos;
    WestPos.Position = EBattlePosition::West;
    WestPos.WorldLocation = CenterPosition + FVector(-PositionRadius, 0.0f, 0.0f);

    BattlePositions.Add(NorthPos);
    BattlePositions.Add(EastPos);
    BattlePositions.Add(SouthPos);
    BattlePositions.Add(WestPos);

    UE_LOG(LogTemp, Log, TEXT("Initialized %d battle positions"), BattlePositions.Num());
}

bool APositionManager::MoveUnitToPosition(ACombatUnit* Unit, EBattlePosition NewPosition)
{
    if (!Unit) return false;

    // Remove unit from current position
    RemoveUnitFromAllPositions(Unit);

    // Add to new position
    FBattlePositionInfo* PositionInfo = GetPositionInfo(NewPosition);
    if (PositionInfo)
    {
        PositionInfo->UnitsAtPosition.Add(Unit);
        Unit->SetPosition(NewPosition);

        UE_LOG(LogTemp, Log, TEXT("%s moved to position %d. Units at this position: %d"),
            *Unit->UnitName, (int32)NewPosition, PositionInfo->UnitsAtPosition.Num());

        return true;
    }

    return false;
}

TArray<ACombatUnit*> APositionManager::GetUnitsAtPosition(EBattlePosition Position) const
{
    for (const FBattlePositionInfo& PositionInfo : BattlePositions)
    {
        if (PositionInfo.Position == Position)
        {
            return PositionInfo.UnitsAtPosition;
        }
    }
    return TArray<ACombatUnit*>();
}

int32 APositionManager::GetUnitCountAtPosition(EBattlePosition Position) const
{
    return GetUnitsAtPosition(Position).Num();
}

FVector APositionManager::GetPositionWorldLocation(EBattlePosition Position) const
{
    for (const FBattlePositionInfo& PositionInfo : BattlePositions)
    {
        if (PositionInfo.Position == Position)
        {
            return PositionInfo.WorldLocation;
        }
    }
    return FVector::ZeroVector;
}

bool APositionManager::CanUseGuard(EBattlePosition Position) const
{
    // Guard is available when 2+ units are at the same position
    return GetUnitCountAtPosition(Position) >= 2;
}

bool APositionManager::CanUseParry(EBattlePosition Position) const
{
    // Parry is available when exactly 1 unit is at the position
    return GetUnitCountAtPosition(Position) == 1;
}

void APositionManager::RemoveUnitFromAllPositions(ACombatUnit* Unit)
{
    for (FBattlePositionInfo& PositionInfo : BattlePositions)
    {
        PositionInfo.UnitsAtPosition.Remove(Unit);
    }
}

FBattlePositionInfo* APositionManager::GetPositionInfo(EBattlePosition Position)
{
    for (FBattlePositionInfo& PositionInfo : BattlePositions)
    {
        if (PositionInfo.Position == Position)
        {
            return &PositionInfo;
        }
    }
    return nullptr;
}