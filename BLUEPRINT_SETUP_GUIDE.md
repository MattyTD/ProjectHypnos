# Project Hypnos Battle System - Complete Implementation Guide

## 📋 Overview
This comprehensive guide will walk you through implementing the complete Persona/Metaphor-inspired battle system from start to finish. You will create:

- **Time-Based Turn System**: 12-second individual unit timers with TFN/SP mechanics
- **Position-Based Combat**: 4-quadrant battlefield (North/East/South/West) surrounding enemies
- **Command Circle UI**: Visual keyboard/controller indicators (like Persona 3 Reload)
- **Action Ring System**: Timed input sequences for melee combat (like Paper Mario)
- **EO Transformation**: Limit break system with 2D→3D model changes
- **Real-Time Defense**: Guard/Dodge/Parry with rhythm game timing
- **Party Status Display**: Final Fantasy X-style status bars

## 🎯 Important UI Design Philosophy
**CRITICAL**: All UI elements showing keyboard keys (E, Space, R, Q, etc.) are **VISUAL INDICATORS ONLY** - they are NOT clickable buttons. Players interact using their keyboard/controller, just like Persona and Metaphor. The UI simply shows which keys perform which actions.

## 🏗️ PHASE 1: Project Foundation Setup

### Step 1: Create Battle Level
> **Objective**: Set up the main battle scene with all required managers

**1.1 Create New Level**
1. **Right-click** in Content Browser
2. **Select** "Level" → "Empty Level"  
3. **Name**: "BattleLevel"
4. **Double-click** to open

**1.2 Add Core Battle Managers**
1. **Find** `BattleManager` C++ class in Content Browser
2. **Drag** into level at position (0, 0, 0)
3. **Find** `PositionManager` C++ class  
4. **Drag** into level at position (0, 0, 100)
5. **Find** `DefenseManager` C++ class
6. **Drag** into level at position (0, 0, 200)

> ✅ **Checkpoint**: You should now have 3 manager actors in your level

### Step 2: Create Combat Units
> **Objective**: Create the 4 player units (M1, M2, M3, M4) and 1 enemy unit (B1)

**2.1 Create Base Player Unit**
1. **Right-click** in Content/BattleSystem/Blueprints/
2. **Select** "Blueprint Class"
3. **Choose** "CombatUnit" as parent class
4. **Name**: "BP_PlayerUnit_Base"
5. **Double-click** to open

**2.2 Configure Base Player Settings**
1. **In Details Panel**, set these properties:
   - **Unit Type**: Player
   - **Max HP**: 100.0
   - **Timer Duration**: 12.0
   - **Max EO**: 100.0
   - **Max MP**: 50.0

2. **Set Elemental Weaknesses** (Example setup):
   - **Click "+"** next to "Elemental Resistances"
   - **Element Type**: Fire
   - **Resistance Multiplier**: 1.5 (weak to fire - takes 150% damage)
   - **Add more** as needed for your game balance

**2.3 Create Individual Player Units**
1. **Duplicate** BP_PlayerUnit_Base **4 times**
2. **Rename** them:
   - "BP_PlayerUnit_M1"
   - "BP_PlayerUnit_M2" 
   - "BP_PlayerUnit_M3"
   - "BP_PlayerUnit_M4"

3. **For each unit**, open and set:
   - **M1**: Unit Name = "M1"
   - **M2**: Unit Name = "M2"
   - **M3**: Unit Name = "M3"
   - **M4**: Unit Name = "M4"

**2.4 Create Enemy Unit**
1. **Right-click** in Content/BattleSystem/Blueprints/
2. **Select** "Blueprint Class" → "CombatUnit"
3. **Name**: "BP_EnemyUnit"
4. **Configure**:
   - **Unit Name**: "B1"
   - **Unit Type**: Enemy
   - **Max HP**: 150.0
   - **Current Position**: Center

> ✅ **Checkpoint**: You should have 5 unit Blueprints total (4 players + 1 enemy)

## 🎨 PHASE 2: User Interface System

> **REMINDER**: All keyboard key displays (E, Space, R, Q, etc.) are **VISUAL INDICATORS** only. Players press actual keyboard keys - the UI just shows which key does what, exactly like Persona 3 Reload.

### Step 3: Main Battle HUD Foundation
> **Objective**: Create the core HUD that manages all UI elements

1. **Right-click** in Content/BattleSystem/UI/
2. **Select** "User Interface" → "Widget Blueprint"
3. **Choose** "BattleHUDWidget" as parent class
4. **Name**: "WBP_BattleHUD"
5. **Double-click** to open

**3.2 Design Main HUD Layout**
> **Layout Overview**: The main HUD contains 5 key areas - Timer (top-left), Battle Info (top-center), Party Status (top-right), Movement Help (bottom-left), and Defense Prompts (bottom-right)

1. **Add Canvas Panel** as root widget
2. **Create these sections**:

**📍 Top-Left: Active Unit Timer**
- **Progress Bar** "TimerBar" 
  - Position: (50, 50)
  - Size: (300, 20)
  - Fill Color: Yellow
- **Text Block** "TimerText"
  - Position: (50, 75) 
  - Text: "12.0s"
- **Text Block** "CurrentUnitText"
  - Position: (50, 25)
  - Text: "M1" (updates dynamically)

**📍 Top-Center: Battle State Info**  
- **Text Block** "BattleStateText"
  - Anchor: Top-Center
  - Text: "Player Set" / "Enemy Set"
- **Text Block** "SetNumberText" 
  - Below BattleStateText
  - Text: "Set 1"

**📍 Top-Right: Party Status (FFX Style)**
- **Vertical Box** "PartyStatusContainer"
  - Anchor: Top-Right
  - Will contain WBP_PartyStatusEntry widgets

**📍 Bottom-Left: Movement Guide (Always Visible)**
- **Text Block** "MovementGuideText"
  - Anchor: Bottom-Left
  - Position: (50, -100)
  - Text: "W/A/S/D: Move | T: Turn Order"
  - Font Size: 14
  - Color: White

**📍 Bottom-Right: Defense Prompts (Enemy Turn Only)**
- **Vertical Box** "DefenseContainer"
  - Anchor: Bottom-Right  
  - Initially Hidden
  - Will show G/D/F key prompts during enemy attacks

### Step 4: Command Circle (Visual Key Indicators)
> **Objective**: Create the Persona-style command circle that shows available actions

**4.1 Create Command Circle Widget**
1. **Right-click** in Content/BattleSystem/UI/
2. **Select** "User Interface" → "Widget Blueprint" 
3. **Choose** "User Widget" as parent
4. **Name**: "WBP_CommandCircle"
5. **Double-click** to open

**4.2 Design Command Circle Layout**
> **Important**: These are NOT clickable buttons - they are visual indicators showing which keyboard keys perform actions

1. **Add Canvas Panel** as root (Size: 200x200)
2. **Create 4 Key Indicator Images**:

**🔵 North Position - E Key**
- **Image** "EKeyIndicator"
  - Position: (100, 25) - North of center
  - Size: (60, 60)
  - Background: Dark blue circle
- **Text Block** "EKeyText"  
  - Position: (115, 40)
  - Text: "E"
  - Font Size: 24, Color: White
- **Text Block** "EActionText"
  - Position: (100, 90)
  - Text: "Egbe Orun" (changes to "Skills" in EO form)
  - Font Size: 12

**🔵 West Position - Space Key**  
- **Image** "SpaceKeyIndicator"
  - Position: (25, 100) - West of center
  - Size: (60, 60)
- **Text Block** "SpaceKeyText"
  - Text: "⎵" (spacebar symbol)
- **Text Block** "SpaceActionText"
  - Text: "Melee"

**🔵 East Position - R Key**
- **Image** "RKeyIndicator" 
  - Position: (175, 100) - East of center
- **Text Block**: "R" 
- **Action Text**: "Items"

**🔵 South Position - Q Key**
- **Image** "QKeyIndicator"
  - Position: (100, 175) - South of center  
- **Text Block**: "Q"
- **Action Text**: "Ranti"

**4.3 Add Below-Circle Indicators**
- **Image** "RetryKeyIndicator" at (60, 220)
  - **Text**: "1" 
  - **Label**: "Retry"
- **Image** "PassKeyIndicator" at (140, 220)
  - **Text**: "N"
  - **Label**: "Pass"

### Step 5: Action Ring (Melee Combat System)
> **Objective**: Create the timed input system for melee attacks (like Paper Mario)

**5.1 Create Action Ring Widget**
1. **Right-click** in Content/BattleSystem/UI/
2. **Select** "User Interface" → "Widget Blueprint"
3. **Choose** "User Widget" as parent
4. **Name**: "WBP_ActionRing"
5. **Double-click** to open

**5.2 Design Action Ring Layout**
> **Function**: Shows 4 key indicators with randomized damage values for 0.8 second windows

1. **Add Canvas Panel** as root (Size: 400x400)
2. **Create Center Ring Visual**:
   - **Image** "RingBackground" (400x400, center screen)
   - **Circular border** graphic

**5.3 Create 4 Key Damage Indicators**
> **Note**: Damage labels (Delta/Theta/Alpha/Beta) randomly assign to different keys each stage

**🎯 E Key Position (North)**
- **Image** "EKeyRing" 
  - Position: (200, 50)
  - Size: (80, 80)
- **Text Block** "EKeyLabel": "E"
- **Text Block** "EDamageLabel": "Delta" (100% damage)

**🎯 Q Key Position (East)**  
- **Image** "QKeyRing" at (350, 200)
- **Text Block** "QKeyLabel": "Q"
- **Text Block** "QDamageLabel": "Beta" (25% damage)

**🎯 R Key Position (South)**
- **Image** "RKeyRing" at (200, 350)  
- **Text Block** "RKeyLabel": "R"
- **Text Block** "RDamageLabel": "Theta" (75% damage)

**🎯 Space Key Position (West)**
- **Image** "SpaceKeyRing" at (50, 200)
- **Text Block** "SpaceKeyLabel": "⎵"
- **Text Block** "SpaceDamageLabel": "Alpha" (50% damage)

**5.4 Add Timing Elements**
- **Progress Bar** "TimingBar" (center, shows 0.8s countdown)
- **Text Block** "CountdownText" (large font, shows "0.8", "0.7", etc.)
- **Text Block** "StageIndicator" ("Stage 1 of 3")

## 🎮 PHASE 3: Input System Setup

### Step 6: Configure Input Mappings
> **Objective**: Set up all keyboard controls for the battle system

**6.1 Open Input Settings**
1. **Go to** Edit → Project Settings
2. **Select** Input section
3. **Scroll to** Action Mappings

**6.2 Add Action Mappings**
> **Add these exact mappings** (case-sensitive):

| Action Name | Key | Function |
|-------------|-----|----------|
| **MoveNorth** | W | Move current unit North |
| **MoveWest** | A | Move current unit West |
| **MoveSouth** | S | Move current unit South |
| **MoveEast** | D | Move current unit East |
| **ShowTurnOrder** | T | Toggle enemy turn order display |
| **EgbeOrun** | E | Transform to EO / Open Skills menu |
| **Melee** | Space | Enter melee combat |
| **Items** | R | Open items menu |
| **Ranti** | Q | Open Ranti skills menu |
| **Pass** | N | End current unit's turn |
| **Retry** | 1 | Restart entire battle |
| **Guard** | G | Defensive guard (enemy turn) |
| **Dodge** | D | Defensive dodge (enemy turn) |
| **Parry** | F | Defensive parry (enemy turn) |

**6.3 Action Ring Input Mappings**
> **For melee combat timing**:

| Action Name | Key | Function |
|-------------|-----|----------|
| **ActionE** | E | Action ring E input |
| **ActionQ** | Q | Action ring Q input |
| **ActionR** | R | Action ring R input |
| **ActionSpace** | Space | Action ring Space input |

## 🔧 PHASE 4: Blueprint Logic Implementation

### Step 7: Main HUD Input Handling
> **Objective**: Connect keyboard inputs to battle actions in WBP_BattleHUD

**7.1 Open WBP_BattleHUD Event Graph**
1. **Open** WBP_BattleHUD
2. **Switch to** Graph tab
3. **Right-click** → Add Event → Event Construct

**7.2 Enable Input on HUD**
```
Event Construct
├── Set Input Mode Game and UI
├── Enable Input (checked)
└── Set Keyboard Focus (Self)
```

**7.3 Add Input Event Handlers**
> **For each input action, add these nodes**:

**Movement Input (Does NOT end turn)**
```
Input Action MoveNorth (Pressed)
├── Branch: Is Current UI State = Normal?
├── True: 
│   ├── Get Current Unit from Battle Manager
│   ├── Call Battle Manager → Move Unit (North)
│   └── Print String "Moving North"
└── False: (Do nothing - in menu or targeting mode)
```

**Repeat for MoveWest, MoveSouth, MoveEast**

**EO/Skills Input (Does NOT end turn)**
```
Input Action EgbeOrun (Pressed)
├── Get Current Unit
├── Branch: Is In EO Form?
├── True: Open Skills Menu (bottom-right)
├── False:
│   ├── Branch: Can Transform To EO?
│   ├── True: Call Transform To EO
│   └── False: Play Invalid Sound
```

**Melee Input (ENDS turn after completion)**
```
Input Action Melee (Pressed)
├── Set UI State = Melee Targeting
├── Hide Command Circle
├── Show Target Selection UI
└── Enable Enemy Selection with WASD
```

**Pass Input (ENDS turn immediately)**
```
Input Action Pass (Pressed)
├── Call Battle Manager → Pass Turn
└── Print String "Turn Passed"
```

**Retry Input (Resets entire battle)**
```
Input Action Retry (Pressed)
├── Call Battle Manager → Retry Battle
└── Print String "Battle Restarted"
```

## 🚀 PHASE 5: Final Setup and Testing

### Step 8: Battle Manager Configuration
> **Objective**: Connect all units and managers in the battle level

**8.1 Configure Battle Manager in Level**
1. **Select** BattleManager actor in level
2. **In Details Panel**:
   - **Base Timer Duration**: 12.0
   - **Player Units**: Click "+" 4 times, assign M1, M2, M3, M4
   - **Enemy Units**: Click "+", assign B1

**8.2 Configure Position Manager**
1. **Select** PositionManager actor
2. **In Details Panel**:
   - **Position Radius**: 500.0
   - **Center Position**: (0, 0, 0)
   - **Battle Positions**: Add 4 entries
     - North: (0, 500, 0)
     - East: (500, 0, 0) 
     - South: (0, -500, 0)
     - West: (-500, 0, 0)

**8.3 Configure Defense Manager**
1. **Select** DefenseManager actor
2. **Set timing windows**:
   - **Dodge Perfect Window**: 0.1
   - **Dodge Good Window**: 0.3
   - **Parry Perfect Window**: 0.05
   - **Parry Good Window**: 0.15

### Step 9: Create Game Mode
> **Objective**: Set up automatic battle start and HUD display

**9.1 Create Battle Game Mode**
1. **Right-click** in Content/BattleSystem/
2. **Select** Blueprint Class → Game Mode Base
3. **Name**: "BP_BattleGameMode"

**9.2 Configure Game Mode Logic**
```
Event BeginPlay
├── Get All Actors of Class (BattleManager)
├── Call Start Battle on BattleManager
├── Create Widget (WBP_BattleHUD)
├── Add to Viewport
└── Set Input Mode Game and UI
```

**9.3 Set Level Game Mode**
1. **In BattleLevel**, open World Settings
2. **Set Game Mode Override**: BP_BattleGameMode

### Step 10: Testing Guide
> **Objective**: Verify all systems work correctly

**10.1 Basic Functionality Test**
1. **Play in Editor**
2. **Verify UI Elements**:
   - ✅ Top-left timer counts down for active unit
   - ✅ Top-center shows "Player Set"
   - ✅ Top-right shows party status (current unit first/larger)
   - ✅ Bottom-left shows "W/A/S/D: Move | T: Turn Order"
   - ✅ Command circle appears left of active unit

**10.2 Input Testing (Does NOT End Turn)**
- **W/A/S/D**: Move current unit between positions
- **T**: Show/hide turn order overlay
- **E** (EO not full): Play invalid sound
- **E** (EO full): Transform unit, change command circle
- **E** (in EO form): Show skills menu bottom-right

**10.3 Action Testing (ENDS Turn)**
- **Space**: Enter melee targeting → Action Ring → Damage calculation
- **R**: Open items menu → Select item → Apply effect
- **Q**: Open Ranti menu → Select skill → Execute
- **N**: Pass turn immediately

**10.4 Battle Flow Testing**
- ✅ Timer pauses during Action Ring and menus
- ✅ TFN slows next unit when weakness hit
- ✅ SP adds time to next set
- ✅ Set ends when all unit timers reach 0
- ✅ Enemy turn starts after set completion

**10.5 Defense Testing (Enemy Turn)**
- **G**: Guard attempt with damage reduction
- **D**: Dodge attempt with timing window
- **F**: Parry attempt with counter-attack

> ✅ **Final Checkpoint**: All systems should work together seamlessly

## 🎉 Congratulations!

You have successfully implemented the complete Project Hypnos battle system! The game now features:

- ✨ **Persona-style command circle** with visual key indicators
- ⚡ **Time-based combat** with TFN/SP mechanics
- 🎯 **Action ring system** for engaging melee combat
- 🛡️ **Real-time defense** with rhythm game elements
- 📊 **FFX-style party status** display
- 🔄 **Position-based strategy** with 4-quadrant battlefield

## 🔧 Next Steps for Enhancement

1. **Visual Polish**: Add particle effects, animations, and sound
2. **Skill System**: Implement detailed magic/special abilities
3. **Enemy AI**: Create intelligent enemy behavior patterns  
4. **Ranti Skills**: Design complex combo move requirements
5. **Balancing**: Fine-tune damage, timing windows, and difficulty

---

## 📚 Additional Resources

### Key Blueprint Nodes Reference
- **Get All Actors of Class**: Find managers in the level
- **Project World to Screen**: Position UI elements relative to 3D units  
- **Set Timer by Event**: Create countdown timers for Action Ring
- **Branch**: Conditional logic for different game states
- **Cast To**: Convert generic objects to specific unit types

### Common Troubleshooting
1. **Command Circle not appearing**: Check ProjectWorldToScreen node
2. **Input not working**: Verify Action Mappings and Enable Input on widget  
3. **Timer not counting**: Ensure BattleManager is ticking
4. **UI not updating**: Check widget bindings and event connections

### Performance Tips
- Use **Set Timer by Event** instead of Tick for periodic updates
- **Cache manager references** instead of finding them every frame
- Use **Visibility Collapsed** instead of removing widgets