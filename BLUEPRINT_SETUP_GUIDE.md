# Battle System Blueprint Setup Guide

## Overview
This guide will walk you through setting up the complete battle system in Unreal Engine Blueprints. The system includes:
- Time-based turn system with TFN/SP mechanics
- Position-based combat with Guard/Dodge/Parry
- EO transformation system
- Real-time defense mechanics
- Comprehensive UI system

## Step 1: Create the Battle Level

### 1.1 Create a New Level
1. **Right-click** in the Content Browser
2. **Select** "Level" → "Empty Level"
3. **Name it** "BattleLevel"
4. **Double-click** to open it

### 1.2 Add Battle Managers
1. **Drag** the `BattleManager` C++ class from the Content Browser into the level
2. **Position it** at (0, 0, 0)
3. **Drag** the `PositionManager` C++ class into the level
4. **Position it** at (0, 0, 100)
5. **Drag** the `DefenseManager` C++ class into the level
6. **Position it** at (0, 0, 200)

## Step 2: Create Player Units

### 2.1 Create Player Unit Blueprint
1. **Right-click** in Content Browser
2. **Select** "Blueprint Class"
3. **Choose** "CombatUnit" as parent
4. **Name it** "BP_PlayerUnit"
5. **Double-click** to open

### 2.2 Configure Player Unit
1. **In the Details Panel**, set these properties:
   - **Unit Name**: "M1" (for first unit)
   - **Unit Type**: Player
   - **Max HP**: 100
   - **Timer Duration**: 12.0
   - **Max EO**: 100
   - **Max MP**: 50

2. **Add Elemental Resistances**:
   - **Click** the "+" next to "Elemental Resistances"
   - **Set Element Type**: Fire
   - **Set Resistance Multiplier**: 0.5 (weak to fire)
   - **Repeat** for other elements as needed

### 2.3 Create Multiple Player Units
1. **Duplicate** BP_PlayerUnit
2. **Name them** "BP_PlayerUnit_M1", "BP_PlayerUnit_M2", "BP_PlayerUnit_M3", "BP_PlayerUnit_M4"
3. **Set Unit Names** to "M1", "M2", "M3", "M4" respectively

## Step 3: Create Enemy Unit

### 3.1 Create Enemy Unit Blueprint
1. **Right-click** in Content Browser
2. **Select** "Blueprint Class"
3. **Choose** "CombatUnit" as parent
4. **Name it** "BP_EnemyUnit"
5. **Double-click** to open

### 3.2 Configure Enemy Unit
1. **In the Details Panel**, set these properties:
   - **Unit Name**: "B1"
   - **Unit Type**: Enemy
   - **Max HP**: 150
   - **Current Position**: Center

## Step 4: Create Battle HUD Widget

### 4.1 Create Widget Blueprint
1. **Right-click** in Content Browser
2. **Select** "User Interface" → "Widget Blueprint"
3. **Choose** "BattleHUDWidget" as parent
4. **Name it** "WBP_BattleHUD"
5. **Double-click** to open

### 4.2 Design the UI Layout
1. **Add a Canvas Panel** as root
2. **Create the following UI elements**:

#### Timer Section (Top Left)
- **Progress Bar** named "TimerBar"
- **Text Block** named "TimerText"
- **Text Block** named "CurrentUnitText"

#### Unit Status Section (Top Right)
- **Progress Bar** named "HPBar"
- **Progress Bar** named "EOBar"
- **Progress Bar** named "MPBar"
- **Text Block** named "HPText"
- **Text Block** named "EOText"
- **Text Block** named "MPText"

#### Battle Info Section (Top Center)
- **Text Block** named "SetNumberText"
- **Text Block** named "BattleStateText"
- **Text Block** named "TFNStatusText"

#### Action Buttons Section (Bottom Left)
- **Button** named "MoveButton" (Text: "Move")
- **Button** named "AttackButton" (Text: "Attack")
- **Button** named "SkillButton" (Text: "Skill")
- **Button** named "ItemButton" (Text: "Item")
- **Button** named "PassButton" (Text: "Pass")
- **Button** named "RantiButton" (Text: "Ranti")
- **Button** named "TransformButton" (Text: "Transform")

#### Position Buttons Section (Bottom Center)
- **Button** named "NorthButton" (Text: "North")
- **Button** named "EastButton" (Text: "East")
- **Button** named "SouthButton" (Text: "South")
- **Button** named "WestButton" (Text: "West")

#### Defense Buttons Section (Bottom Right)
- **Button** named "GuardButton" (Text: "Guard")
- **Button** named "DodgeButton" (Text: "Dodge")
- **Button** named "ParryButton" (Text: "Parry")

#### Status Effects Section (Right Side)
- **Text Block** named "StressedOutText"
- **Text Block** named "StockpileText"

### 4.3 Style the UI Elements
1. **Set colors** for each progress bar:
   - **HPBar**: Red (255, 0, 0)
   - **EOBar**: Blue (0, 0, 255)
   - **MPBar**: Green (0, 255, 0)
   - **TimerBar**: Yellow (255, 255, 0)

2. **Set button colors**:
   - **Action buttons**: Blue
   - **Position buttons**: Green
   - **Defense buttons**: Orange

3. **Set text colors**:
   - **Normal text**: White
   - **Status effects**: Red for negative, Green for positive

## Step 5: Set Up Battle Manager

### 5.1 Configure Battle Manager in Level
1. **Select** the BattleManager actor in the level
2. **In Details Panel**, set:
   - **Base Timer Duration**: 12.0
   - **Player Units**: Add all 4 player units
   - **Enemy Units**: Add the enemy unit

### 5.2 Create Battle Manager Blueprint
1. **Right-click** on BattleManager actor
2. **Select** "Create Blueprint based on this"
3. **Name it** "BP_BattleManager"
4. **Double-click** to open

### 5.3 Implement Battle Events
1. **In the Event Graph**, add these events:

#### On Battle State Changed
```
Event OnBattleStateChanged(EBattleState NewState)
├── Call Function: UpdateBattleInfo
└── Call Function: UpdateAllUI
```

#### On Unit Turn Started
```
Event OnUnitTurnStarted(ACombatUnit Unit)
├── Call Function: UpdateTimerDisplay
├── Call Function: UpdateUnitStatus
└── Call Function: UpdateActionButtons
```

#### On Set Complete
```
Event OnSetComplete()
├── Call Function: UpdateBattleInfo
└── Print String: "Set Complete!"
```

## Step 6: Set Up Position Manager

### 6.1 Configure Position Manager
1. **Select** the PositionManager actor in the level
2. **In Details Panel**, set:
   - **Position Radius**: 500.0
   - **Center Position**: (0, 0, 0)

### 6.2 Set Up Battle Positions
1. **In the Details Panel**, expand "Battle Positions"
2. **Add 4 positions**:
   - **North**: (0, 500, 0)
   - **East**: (500, 0, 0)
   - **South**: (0, -500, 0)
   - **West**: (-500, 0, 0)

## Step 7: Set Up Defense Manager

### 7.1 Configure Defense Manager
1. **Select** the DefenseManager actor in the level
2. **In Details Panel**, set timing windows:
   - **Dodge Perfect Window**: 0.1
   - **Dodge Good Window**: 0.3
   - **Parry Perfect Window**: 0.05
   - **Parry Good Window**: 0.15

## Step 8: Create Game Mode

### 8.1 Create Battle Game Mode
1. **Right-click** in Content Browser
2. **Select** "Blueprint Class"
3. **Choose** "Game Mode Base" as parent
4. **Name it** "BP_BattleGameMode"
5. **Double-click** to open

### 8.2 Configure Game Mode
1. **In the Event Graph**, add:
```
Event BeginPlay
├── Get All Actors Of Class (BattleManager)
├── Call Function: StartBattle
└── Create Widget (WBP_BattleHUD)
    └── Add To Viewport
```

## Step 9: Set Up Input System

### 9.1 Create Input Actions
1. **Go to** Project Settings → Input
2. **Add Action Mappings**:
   - **Move**: Spacebar
   - **Attack**: Left Mouse Button
   - **Skill**: Right Mouse Button
   - **Item**: I
   - **Pass**: P
   - **Guard**: G
   - **Dodge**: D
   - **Parry**: F

### 9.2 Bind Input in Widget
1. **In WBP_BattleHUD**, add input bindings:
```
Event Construct
├── Bind Action (Move)
├── Bind Action (Attack)
├── Bind Action (Skill)
├── Bind Action (Item)
├── Bind Action (Pass)
├── Bind Action (Guard)
├── Bind Action (Dodge)
└── Bind Action (Parry)
```

## Step 10: Test the System

### 10.1 Set Up Test Level
1. **Open** BattleLevel
2. **Set Game Mode Override** to BP_BattleGameMode
3. **Place units** in the level:
   - **Player units** at West position
   - **Enemy unit** at Center position

### 10.2 Test Battle Flow
1. **Play in Editor**
2. **Verify**:
   - Timer counts down
   - Units can move between positions
   - Actions can be performed
   - TFN/SP mechanics work
   - Defense system functions
   - EO transformation works

## Step 11: Add Visual Feedback

### 11.1 Create Visual Effects
1. **Create particle effects** for:
   - **Attacks**: Red particles
   - **Skills**: Blue particles
   - **EO Transformation**: Golden particles
   - **Defense**: Shield particles

### 11.2 Add Sound Effects
1. **Import audio files** for:
   - **Attack sounds**
   - **Skill sounds**
   - **Defense sounds**
   - **UI button clicks**

### 11.3 Add Animations
1. **Create animation blueprints** for:
   - **Unit movement**
   - **Attack animations**
   - **Defense animations**
   - **EO transformation**

## Troubleshooting

### Common Issues:
1. **Units not appearing**: Check if units are added to BattleManager's PlayerUnits array
2. **Timer not working**: Verify BattleManager is ticking and timer logic is correct
3. **UI not updating**: Check widget bindings and event connections
4. **Defense not working**: Verify DefenseManager is properly configured

### Debug Tips:
1. **Use Print String** nodes to debug flow
2. **Check Output Log** for error messages
3. **Use Breakpoints** in Blueprint graphs
4. **Verify** all manager references are set correctly

## Next Steps

After basic setup is complete:
1. **Implement** skill system
2. **Add** item system
3. **Create** Ranti skill system
4. **Add** enemy AI
5. **Implement** save/load system
6. **Add** visual effects and animations
7. **Create** sound system
8. **Add** difficulty settings

This setup provides the foundation for your complex battle system. Each component can be expanded and refined as needed.
