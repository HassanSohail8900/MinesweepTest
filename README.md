Minesweeper Tool – Unreal Editor Plugin
Overview

This project implements a Minesweeper Tool inside the Unreal Editor as part of a trial task.
The tool allows users to configure and play a simple Minesweeper game directly in the editor.
It is built as an Unreal Editor Plugin using C++ and Slate only (no UMG or Editor Utility Widgets).

Features

Adds a toolbar button to open the Minesweeper Tool window.

Configurable grid width, grid height, and number of bombs.

Clicking a tile reveals its contents:

Bomb tile: Displays a “Game Over” message.

Non-bomb tile: Shows the number of adjacent bombs.

Empty tile (0 bombs nearby): Recursively reveals surrounding tiles.

Supports starting a new game at any time.

Exclusions

No bomb-flagging functionality.

No custom textures or visual polish (default Slate UI is sufficient).

Implementation Notes

Implemented as a plugin using Unreal’s toolbar extension template.

Relies entirely on Slate widgets for the UI.

Designed with clarity, modularity, and maintainability in mind.

Use of AI/LLMs

I used ChatGPT during development to:

Draft an initial project structure and plugin setup steps.

Suggest clean approaches for Slate widget layout.

Help with wording for this README.

All generated code and text were reviewed and adapted by me to ensure correctness, clarity, and maintainability. AI was used as an assistant for brainstorming and speeding up iteration, not as a replacement for design or problem-solving.

Evaluation Criteria (addressed in this project)

Correctness & completeness: Implements all required Minesweeper mechanics.

Code quality: Clear structure, consistent naming, and readability.

Appropriate use of C++ & Unreal framework: Uses Slate and Editor plugin architecture.

Software architecture: Designed with separation of UI, game logic, and editor integration.

How to Run

Clone this repository into your Unreal projects Plugins folder.

Open the Unreal Editor and enable the plugin.

A Minesweeper button will appear in the editor toolbar.

Click it to open the tool, configure the grid, and play.
