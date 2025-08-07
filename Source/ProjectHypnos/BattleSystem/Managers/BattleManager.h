// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Units/CombatUnit.h"
#include "BattleManager.generated.h"

UCLASS(Blueprintable)
class PROJECTHYPNOS_API ABattleManager : public AActor
{
    GENERATED_BODY()

public:
    ABattleManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<ACombatUnit*> PlayerUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<ACombatUnit*> EnemyUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CurrentUnitIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsPlayerTurn = true;

    void NextTurn();
    void EndSetAndStartEnemyTurn();
};