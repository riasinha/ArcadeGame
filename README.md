# ArcadeGame

Welcome to the ArcadeGame repository! This project is a maze-based game where players navigate through a maze, avoiding ghosts along the way. The player's objective is to move from the starting point to the end point without being caught by the ghosts. The game is built using OpenGL, GLFW for window management, and GLAD for loading OpenGL functions.

## Group Members
Sanjana Jilla
Ria Sinha
Bhavya Ranjan

## Prerequisites

Before you begin, ensure you have the following requirements:

* The [GLFW](https://www.glfw.org/download.html) library installed.
* The [GLAD](https://glad.dav1d.de/) OpenGL loader generated and installed.

## Project Structure

The project is organized as follows:

* `main.cpp` - The main game file with GLFW window creation and the game loop.
* `dependencies/` - This folder should contain the GLAD and GLFW header files and libraries.
* `README.md` - Describes how the project is organized and provides instructions for building and running the game.

## Installing Dependencies

To set up the development environment, you need to download and install GLFW and GLAD:

1. **GLFW**: Go to the [GLFW download page](https://www.glfw.org/download.html), download the pre-compiled binaries or source code for your platform, and set up according to their instructions.

2. **GLAD**: Visit [GLAD's web service](https://glad.dav1d.de/), configure the OpenGL loader for your needs (OpenGL version and extensions), and download the generated files. Place the `glad.c` file in your project's source directory and the header files in your include path.

## Building the Project

To build the project, follow these steps:

1. Clone the repository to your local machine:
2. Navigate to the project directory:
3. Build the project. If you're using a specific build system or IDE, the command might differ:
'command shift b' - This command should be configured to compile the code and link the GLFW and GLAD libraries.

## Running the Game

After building the project, you can start the game by running the generated executable:

Control your character using the arrow keys, and navigate through the maze. The objective is to avoid the ghosts, represented by circles, and reach the end of the maze.

## Understanding the Code

The main aspects of the code are:

* `maze` - A 2D vector representing the maze layout, where `1` is a wall, `2` is a boundary, `3` is the starting point, and `4` is the end point.
* `characterX`, `characterY` - Variables representing the player's position in the maze.
* `Ghost` - A struct representing the ghosts in the maze, each with a position, direction, color, and speed.
* `isPositionValid` - A function that checks whether a proposed movement is within the bounds of the maze and not within a wall.
* `drawMaze`, `drawCharacter`, `drawGhost` - Functions that render the maze, player, and ghosts respectively.
* `processInput` - Handles user input to move the player around the maze.
* `updateGhostPositions` - Updates the positions of the ghosts, giving them a new random direction periodically.
