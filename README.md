## MazeGame
Welcome to the MazeGame repository! This project is a challenging game where players navigate through a series of mazes, avoiding ghosts and collecting diamonds. The objective is to complete all levels while maximizing your score. The game is developed using OpenGL and GLUT for graphics rendering.

## Group Members
Sanjana Jilla
Ria Sinha
Bhavya Ranjan

## Prerequisites
Before starting, ensure you have the following:

* GLUT library installed.

## Project Structure
* main.cpp - Main game file containing game logic, rendering, and input handling.
* dependencies/ - Contains the GLUT header files and libraries.
* README.md - Provides a project overview and setup instructions.
* highscores.txt - Stores the high scores of the game.

## Installing Dependencies
* GLUT: Download from the GLUT downloads page. Follow the installation instructions provided there.

## Building the Project
* Clone the repository.
* Navigate to the project directory.
* Compile the project using a suitable C++ compiler, linking the GLUT library. Example command for g++: g++ -o MazeGame main.cpp -lGL -lGLU -lglut.

## Running the Game
Execute the compiled program to start the game. Control the character using 'W', 'A', 'S', 'D' keys, avoid ghosts, and collect diamonds for points.
To run it on csil: 
g++ main.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lGL -lglut
DISPLAY=:0 ./a.out


## Game Features
* Multiple levels with increasing difficulty.
* Ghost enemies with AI movements.
* Diamond collection for score boosting.
* High score tracking and leaderboard display.
* Lives system and game-over logic.
* Start, pause, and game-over screens with instructions.
* Code Explanation

## Key components of the code include:

* maze - 2D vector representing the maze layout with walls, paths, and start/end points.
* Ghost struct - Represents ghosts with position, direction, and speed.
* Diamond struct - Represents diamonds with position and collection status.
* GameState enum - Manages different states of the game (start, in-game, pause, etc.).
* drawMaze, drawCharacter, drawGhost, drawDiamond - Functions to render game elements.
* keyboard - Handles keyboard input for character movement and game controls.
* updateGameLogic - Manages game logic like collision detection and level progression.
* display - GLUT display function to render the game based on the current state.


