# MyProject - Unreal Engine 5.6 Action RPG

A third-person action RPG game built with Unreal Engine 5.6, featuring combat systems, enemy AI, inventory management, and character progression.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Core Systems](#core-systems)
- [Development](#development)
- [Contributing](#contributing)

## Overview

This project is a comprehensive action RPG game built using Unreal Engine 5.6 with C++ and Blueprint integration. It includes a fully functional combat system, AI-driven enemies, inventory management, and character progression mechanics.

## Features

### Combat System
- **Melee Combat**: Sword-based combat with attack animations and hit detection
- **Damage System**: Interface-based damage system using `IDamageableInterface`
- **Line Trace Attacks**: Precision attack detection using line traces
- **Attack Animations**: Montage-based attack system with animation blending

### Enemy AI
- **Behavior Trees**: AI enemies with patrol, chase, and attack behaviors
- **Smart Pathfinding**: Navigation-based movement with patrol paths
- **Attack Ranges**: Dynamic attack range detection using `BTService_IsPlayerInMeleeRange`
- **Custom BT Tasks**:
  - `BTTask_MeleeAttack`: Execute melee attacks
  - `BTTask_ChasePlayer`: Chase and pursue the player
  - `BTTask_FindPlayerLocation`: Locate player position
  - `BTTask_FindPathPoint`: Navigate patrol paths
  - `BTTask_FindRandomLocation`: Random movement patterns

### Health & Combat Components
- **Health Component**: Modular health system with delegates
- **Damage Delegates**: Event-driven damage and death notifications
- **Hit Reactions**: Animation-based hit reactions
- **Death System**: Character death handling with animations

### Inventory System
- **Drag & Drop UI**: Full drag-and-drop inventory interface
- **Item Management**: Add, remove, and stack items
- **Item Types**: Support for different item categories and properties
- **Tooltips**: Interactive item tooltips with descriptions
- **Weight System**: Inventory capacity management

### Interaction System
- **Interaction Interface**: Interface-based interaction system
- **Widget Integration**: Dynamic interaction prompts
- **Multiple Interaction Types**:
  - PickUp items
  - NPCs
  - Devices
  - Toggles
  - Containers

### Character Features
- **Sprint System**: Stamina-based sprinting with UI feedback
- **XP & Progression**: Experience collection and leveling system
- **Coin Collection**: Currency system for pickups
- **Camera System**: Spring arm camera with adjustable settings

### UI Systems
- **Health Bar**: Real-time health display
- **Stamina Bar**: Stamina management UI
- **Inventory Panel**: Full inventory interface with slot management
- **Main Menu**: Game menu system
- **HUD**: Custom HUD implementation

## System Requirements

### Minimum Requirements
- **Unreal Engine**: 5.6
- **Visual Studio**: 2022 (recommended)
- **RAM**: 16GB
- **GPU**: DirectX 11/12 compatible graphics card
- **Storage**: 50GB available space

### Required Plugins
- **ModelingToolsEditorMode**: Enabled by default
- **ThreepeatAnimTools**: Animation toolkit (Fab Marketplace)

## Installation

1. **Clone or download** the project to your local machine
2. **Right-click** on `MyProject.uproject` and select **"Generate Visual Studio project files"**
3. **Open** `MyProject.sln` in Visual Studio 2022
4. **Build** the solution (Development Editor configuration)
5. **Launch** the project through the Unreal Engine editor

## Project Structure

```
MyProject/
├── Binaries/          # Compiled binaries and DLLs
├── Config/            # Configuration files
│   ├── DefaultEditor.ini
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   └── DefaultInput.ini
├── Content/           # Game assets and blueprints
│   ├── BP_Classes/    # Blueprint classes
│   ├── Inputs/        # Enhanced Input assets
│   ├── ItemData/      # Item data tables
│   ├── Levels/        # Game levels
│   ├── Sounds/        # Audio assets
│   └── Fab/           # Marketplace assets
├── Source/            # C++ source code
│   └── MyProject/     # Main project source
└── Saved/             # Save files and logs
```

## Core Systems

### Interface Architecture

The project uses C++ interfaces for clean, decoupled system design:

- **ICombatInterface**: Combat actions and attack methods
- **IDamageableInterface**: Damage reception and health queries
- **IInteractionInterface**: World interaction system
- **ITakingXp**: Experience point collection

### Component-Based Design

Key components for modular functionality:

- **UHealthComponent**: Health management with event delegates
- **UInventoryComponent**: Item storage and management
- **UStaminaBar**: Stamina UI widget
- **UAIPerceptionStimuliSourceComponent**: AI perception for player detection

### AI System

Enemy AI is built using Unreal's Behavior Tree system:

- **EnemyAIController**: Custom AI controller with perception
- **Behavior Trees**: Modular AI logic
- **Blackboard**: AI data storage and query
- **Patrol Paths**: Designer-defined patrol routes

### Damage Flow

```
Attacker (Player/Enemy)
    ↓
ApplyDamage() [IDamageableInterface]
    ↓
ReceiveDamage() [Target]
    ↓
HealthComponent->TakeDamage()
    ↓
OnDamaged/OnDeath Delegates
    ↓
Hit Reactions / Death Animations
```

## Development

### Key Classes

#### Character Classes
- **AMyFirstCharacter**: Player character with combat and inventory
- **AEnemy**: AI enemy with combat capabilities

#### AI Classes
- **AEnemyAIController**: Enemy AI controller
- **UBTTask_MeleeAttack**: Behavior tree attack task
- **UBTService_IsPlayerInMeleeRange**: Attack range service

#### Inventory Classes
- **UInventoryComponent**: Inventory logic
- **UItemBase**: Base item class
- **UInventoryItemSlot**: UI slot widget
- **UItemDragDropOperation**: Drag-drop operation

#### Combat Classes
- **ASword**: Sword actor with collision
- **UHealthComponent**: Health management
- **ICombatInterface**: Combat interface
- **IDamageableInterface**: Damage interface

### Building the Project

```powershell
# Generate project files
# Right-click MyProject.uproject → Generate Visual Studio project files

# Build from command line
cd "C:\Users\oseos\Documents\Unreal Projects\MyProject"
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" MyProject.sln /t:Build /p:Configuration="Development Editor"
```

### Debugging

- Use Visual Studio's debugger with "Development Editor" configuration
- Enable logs in `DefaultEngine.ini` for specific categories
- Check `Saved/Logs/` for runtime logs

## Contributing

When contributing to this project:

1. Follow Unreal Engine C++ coding standards
2. Use meaningful variable and function names
3. Comment complex logic sections
4. Test thoroughly in PIE (Play In Editor) before committing
5. Document any new systems in this README

### Code Style Guidelines

- **Naming Conventions**:
  - Classes: `AMyClass` (Actor), `UMyClass` (Object), `FMyStruct` (Struct)
  - Interfaces: `IMyInterface`
  - Member variables: `bIsValid`, `CurrentHealth`, `MaxSpeed`
  - Functions: `PascalCase`

- **Architecture**:
  - Prefer composition over inheritance
  - Use interfaces for cross-cutting concerns
  - Keep Blueprint/C++ communication minimal and clean
  - Use delegates for event-driven communication

## Credits

- **Unreal Engine 5.6** by Epic Games
- **ThreepeatAnimTools** from the Fab Marketplace

## Contact

Email: ruadfndiyev@gmail.com

Note: You can find me on any other social media platforms by searching YaSolo or YaSolo434

---

**Note**: This project is in active development. Systems and features may change during development iterations.


