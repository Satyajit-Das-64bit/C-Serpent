🐍 Cross-Platform Console Snake Game in C
A classic Snake game implementation written in C that runs natively on both Windows and Linux/macOS. This project demonstrates low-level console manipulation, memory management, and game loop logic without using external game engines.
✨ Features
Cross-Platform Compatibility: Works seamlessly on Windows (using <conio.h>/windows.h) and Linux/macOS (using <termios.h>/ANSI escape codes).
Flicker-Free Rendering: Uses cursor positioning logic rather than clearing the screen every frame, resulting in smooth animations.
Persistent High Score: Saves your highest score to a local file (highscore.txt) so it remains even after you close the game.
Progressive Difficulty: The game speed increases dynamically as your score gets higher.
Crash Protection: Includes logic to prevent the snake from reversing into itself and array bounds checking.
🎮 ControlsKeyAction
W Move Up
A Move Left
S Move Down
D Move Right
X Quit Game
🚀 How to Run
Prerequisites
You need a C compiler installed (like GCC or Clang).Linux & macOSOpen your terminal.Navigate to the project folder.Compile the code:Bashgcc snake.c -o snake
Run the game:Bash./snake
WindowsOpen Command Prompt or PowerShell.Navigate to the project folder.Compile using GCC (MinGW):DOSgcc snake.c -o snake.exe
Run the game:DOSsnake.exe
🛠️ Technical Details
This project handles platform differences using Preprocessor Directives:Input Handling:Windows: Uses _kbhit() and _getch() for non-blocking input.Linux: Uses select() and modifies termios struct to enable raw mode.Timing:Windows: Uses Sleep().Linux: Uses usleep().Data Structure:The snake's body is tracked using parallel arrays (tail_x[] and tail_y[]).Movement is handled by shifting array values from the tail towards the head (Shift Register logic).
📝 License
This project is open-source and available for educational purposes.
