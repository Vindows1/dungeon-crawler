# 🏛️Dungeon Crawler

A text-based dungeon crawler game written in C++. Explore dungeons, fight monsters, collect items, and navigate through levels to find the treasure!

## ❓About
<img src="https://github.com/user-attachments/assets/9c148062-c433-4391-91db-2229f0ff2344" alt="Screenshot-main" width="800">
<img src="https://github.com/user-attachments/assets/1faded57-4f22-42f0-b23c-349c7845a4a3" alt="Screenshot-main" width="800">


In this game, you control a hero navigating a 10x10 grid-based dungeon. Your goal is to defeat monsters, avoid traps, collect items (weapons, armor, potions, and artifacts), and progress through levels by reaching the exit. The game features a simple combat system with critical hits, an inventory system, and randomly generated items based on the player's level.

## ⚙️ Features

- **Dungeon Navigation**: Move through a grid-based map with walls, monsters, traps, and exits.
- **Combat System**: Fight monsters with a chance for critical hits, earning experience points (EP) to level up.
- **Inventory Management**: Collect and equip items like weapons, armor, potions, and artifacts.
- **Level Progression**: Advance through multiple levels loaded from text files.
- **Traps**: Encounter traps that deal damage, with increasing severity in higher levels.

## 📥Installation

1. **Prerequisites**:
   - A C++ compiler supporting C++11 or later (e.g., g++, MSVC).
   - Windows operating system for full color support (ANSI color codes may work on Unix-like systems with compatible terminals).

2. **Steps**:
   - Clone or download this repository.
   - Ensure the files `level1.txt` and `monsters.csv` are in the same directory as the executable.
   - Compile the source code:
     ```bash
     g++ -std=c++11 main.cpp -o dungeon
     ```
   - Run the game:
     - Windows: `dungeon.exe`
     - Linux/macOS (with compatible terminal): `./dungeon`

## 🕹️Gameplay Controls

- **W**: Move up
- **A**: Move left
- **S**: Move down
- **D**: Move right
- **I**: Open inventory to equip or use items
- **Q**: Quit the game
- **S** (in the start menu): Start the game
- **I** (in the start menu): View inventory

## 📜File Formats

The game relies on two external files:

### `level1.txt`
- A 10x10 grid representing the dungeon map.
- Symbols:
  - `W`: Wall (impassable)
  - `S`: Starting position of the player
  - `E`: Exit to the next level
  - `T`: Treasure chest (contains an item)
  - `M`: Monster
  - ` ` (space): Empty floor
- Example:
  ```
  WWWWWWWWWW
  W    S   W
  WM W W W W
  W  M E S W
  WM W W   W
  WS     M W
  WWWWWW M W
  WE   W   W
  WS     W W
  WWWWWWWWWW
  ```

### `monsters.csv`
- A CSV file defining monsters with their stats.
- Format: `id,ep,level,name,hp,atk,def,critChance`
- Columns:
  - `id`: Unique monster ID (integer)
  - `ep`: Experience points awarded (integer)
  - `level`: Monster level (integer)
  - `name`: Monster name (string)
  - `hp`: Hit points (integer)
  - `atk`: Attack value (integer)
  - `def`: Defense value (integer)
  - `critChance`: Critical hit chance (float, e.g., 0.05 for 5%)
- Example:
  ```
  id,ep,level,name,hp,atk,def,critChance
  1,50,1,Goblin,30,5,2,0.05
  2,100,2,Troll,50,8,4,0.1
  ```

## System Requirements

- **Operating System**: Windows (recommended for full color support). Linux/macOS with an ANSI-compatible terminal may work, but some features (e.g., console colors) may not function as intended.
- **Compiler**: Any C++ compiler supporting C++11 (e.g., g++, MSVC).
- **Dependencies**: Standard C++ libraries only.

## Notes

- The game uses Windows-specific console functions (`SetConsoleTextAttribute`, `Sleep`, `_getch`). For better cross-platform compatibility, consider modifying these to use standard C++ or libraries like `ncurses`.
- Additional levels can be added by creating files named `level2.txt`, `level3.txt`, etc., following the same format as `level1.txt`.
- The game currently supports a single player and does not save progress between sessions.

## License

MIT License

Copyright (c) 2025 Vindows1

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
