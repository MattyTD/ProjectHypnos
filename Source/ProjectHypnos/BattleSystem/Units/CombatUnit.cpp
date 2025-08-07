// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatUnit.h"

ACombatUnit::ACombatUnit()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentHP = MaxHP;
    TimerRemaining = TimerDuration;
}

void ACombatUnit::BeginPlay()
{
    Super::BeginPlay();
}

void ACombatUnit::ResetTimer()
{
    TimerRemaining = TimerDuration;
}

bool ACombatUnit::IsAlive() const
{
    return CurrentHP > 0;
}