# John Conways Game of Life

## TODO:
1. ~~make a readme.md file~~
2. ~~add something useful to the readme.md file~~
3. add an explanation to the game

In the meantime, check [the release page](https://github.com/LessVegetables/game-of-life/releases).


## Installation Guide for Game of Life

### Requirements:
- **Operating System:** Works on macOS, Linux^, or Windows^ (to be tested. [Create an issue](https://github.com/LessVegetables/game-of-life/issues) if it doesn't work)
- **CMake:** Ensure you have CMake installed
- **Compiler:** A C compiler

### Steps for building from source:

1. **Clone the repository:**

   Open your terminal and run the following command to clone the repository:
   ```bash
   git clone https://github.com/LessVegetables/game-of-life.git
   cd game-of-life
   ```

2. **Configure the project:**

   ```bash
   cmake -S . -B build
   ```

3. **Build the project:**

   ```bash
   cmake --build build
   ```

4. **Run the Game:**

   After the build is complete, run the game:
   ```bash
   ./build/game-of-life/game-of-life
   ```

## Credit:
(Ray)[https://github.com/raysan5] — the godfather of (Raylib)[www.raylib.com], without wich this whole project would just not work.
(Super Fast Hash by Paul Hseieh)[https://gist.github.com/CedricGuillemet/4978020] — hash algorithm for loop detection.
