🌍 Planet vs Zombies

A simple Plants vs Zombies-style game written in C using the raylib library.
This project was created for learning game development fundamentals such as game loops, rendering, and basic game mechanics.

🎮 Features
Grid-based tower defense gameplay
Plant/defense placement system
Zombie enemy waves
Simple 2D graphics using raylib
Basic collision and game logic
Wave-based difficulty

🛠️ Technologies Used
C language
raylib library
📦 Installation
1. Clone the repository
git clone https://github.com/AlirezaBolhasani-creator/Planet_vs_zombies.git
cd Planet_vs_zombies
2. Install raylib

You need raylib installed on your system.

Windows: follow raylib installation guide
Linux: sudo apt install libraylib-dev
macOS: brew install raylib
▶️ How to Build & Run
Using gcc:
gcc main.c -o game -lraylib -lm -lpthread -ldl -lrt -lX11
Run the game:
./game
🎯 How to Play
Place defensive units on the grid
Zombies will spawn in waves
Prevent zombies from reaching your base
Survive as long as possible
📁 Project Structure 
Planet_vs_zombies/
│
│   animation.c
│   animation.h
│   bullet.c
│   bullet.h
│   config.h
│   game.c
│   game.h
│   grid.c
│   grid.h
│   levels.c
│   levels.h
│   main.c
│   mower.c
│   mower.h
│   plant.c
│   plant.h
│   shop.c
│   shop.h
│   sounds.c
│   sounds.h
│   src.zip
│   sun.c
│   sun.h
│   ui_menu.c
│   ui_menu.h
│   ui_shop.c
│   ui_shop.h
│   zombie.c
│   zombie.h
└───output


📌 Notes

This project was built for learning purposes to practice:

C programming
Game development basics
raylib graphics library
Game loop structure and logic


👨‍💻 Author
Alireza Bolhasani
