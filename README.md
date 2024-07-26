# K_Cards Game Feature

## Introduction
**K_Cards Game Feature** is a comprehensive card game framework built on Unreal Engine 5.2, utilizing both C++ and Blueprints for flexibility and scalability. This project is designed to provide a solid foundation for developing turn-based card games, incorporating advanced AI behavior, player state management, and the Unreal Engine Gameplay Ability System (GAS).

## Project Structure and Major Classes

### 1. `K_CardsRuntime`
This module contains the core runtime logic of the card game feature, including classes for card management, gameplay mechanics, and player interactions.

- **`UK_CardManager`**: Manages the creation, registration, and lifecycle of card objects within the game. It handles the overall state of the card deck and individual cards.
  
- **`UK_CardsPlayerStateComponent`**: Manages player-specific attributes and states, such as the player’s deck, hand, and in-game stats. This class integrates with the Gameplay Ability System (GAS) to track and modify player attributes dynamically.
  
- **`UK_CardsPlayerAttributeSet`**: Defines custom attributes for the card game, including health, hunger, and exposure. It uses GAS to calculate and apply attribute changes during gameplay.

### 2. `Card Object Structure`
The card objects are designed to support both 3D and UI representations, allowing for versatile gameplay mechanics and interactions.

- **`A3DCard`**: Represents a 3D card actor in the game world. It handles the physical presence and interactions of cards in the game environment.

- **`W_Card`**: Represents a UI card widget, used for displaying card information and interactions within the game's user interface.

- **`UCardObject`**: A data class that holds the shared information and state of both 3D and UI card representations. It includes properties like card state, purpose, and associated gameplay tags.

### 3. `Ability System`
Handles the integration of abilities and gameplay effects using Unreal Engine's GAS.

- **`UK_HungerHealthExecutionCalculation`**: Custom execution calculation for managing hunger and health attributes. It modifies health based on the player's hunger level and applies the appropriate gameplay effects.
  
- **`UK_ExposureLevelExecutionCalculation`**: Calculates and updates the player’s exposure level based on the presence of Shelter and Fire cards in play, dynamically adjusting the exposure damage attribute.

- **`UK_HandLimitCalculation`**: Increases the player’s hand limit based on the number of Basket cards in play, ensuring the limit is adjusted correctly when cards are destroyed or played.

### 4. `Turn Manager`
Manages the turn-based logic of the game, ensuring smooth transitions and consistent game flow.

- **`UK_TurnManagerComponent`**: Handles the overall turn-based mechanics, including player turns, AI turns, and card actions. It ensures that each player and AI follows the correct sequence of actions during their turn.

### 5. `AI Behavior`
Includes AI logic and behavior trees for non-player card interactions.

- **`AK_CardsAIController`**: AI controller that manages the behavior of AI players, including decision-making and card interactions.
  
- **`UK_Task_DealDamage`**: AI task for dealing damage to players using specific cards. Integrates with GAS to apply damage effects.
  
- **`UK_Task_MoveCardToLocation`**: AI task for moving cards to specific locations on the table, used in AI decision-making processes.

### 6. `Selection Manager`
Handles the selection and highlighting of cards, allowing players and AI to interact with specific cards effectively.

- **`UK_CardsSelectionManagerComponent`**: Manages card selection logic, including highlighting selected cards and processing player or AI selections during gameplay.

### 7. `Debugging and Utilities`
Provides tools and utilities for debugging and managing game state.

- **`UK_CardsDebugUtility`**: Utility class for logging and debugging card states, player attributes, and game events. Includes functions for listing card contents, checking attribute values, and more.

- **`K_CardsBlueprintFunctionLibrary`**: Contains Blueprint-accessible functions for common tasks, such as removing and destroying cards, setting card states, and managing gameplay effects.

## Flexibility and Design
This framework's clean design and modularity make it highly adaptable for various card game genres. The integration of GAS and the clear separation between C++ and Blueprints ensure maintainable and scalable code. The use of state components and utility functions streamlines gameplay logic, making it easier to extend and customize.

## Getting Started
1. Clone the repository from GitHub.
2. Open the project in Unreal Engine 5.2.
3. Explore the provided classes and Blueprints to understand the core functionality.
4. Customize and extend the framework to fit your specific card game requirements.

## Contact
For any questions or support, please contact Isaac Hayward at [kodingroo@gmail.com](mailto:kodingroo@gmail.com).
