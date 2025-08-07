// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleManager.h"
#include "../Units/CombatUnit.h"
#include "Kismet/GameplayStatics.h"

ABattleManager::ABattleManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABattleManager::BeginPlay()
{
    Super::BeginPlay();
}

void ABattleManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPlayerTurn && PlayerUnits.IsValidIndex(CurrentUnitIndex))
    {
        ACombatUnit* CurrentUnit = PlayerUnits[CurrentUnitIndex];

        if (CurrentUnit->TimerRemaining > 0.0f)
        {
            CurrentUnit->TimerRemaining -= DeltaTime;
        }
        else
        {
            NextTurn();  // move to next unit
        }
    }
}

void ABattleManager::NextTurn()
{
    CurrentUnitIndex++;

    if (CurrentUnitIndex >= PlayerUnits.Num())
    {
        EndSetAndStartEnemyTurn();
    }
}

void ABattleManager::EndSetAndStartEnemyTurn()
{
    bIsPlayerTurn = false;
    CurrentUnitIndex = 0;

    // For now, just print to log
    UE_LOG(LogTemp, Warning, TEXT("Enemy turn begins!"));
    // TODO: Handle enemy logic later

    // Reset for player next cycle
    for (ACombatUnit* Unit : PlayerUnits)
    {
        Unit->ResetTimer();
    }

    bIsPlayerTurn = true;
}