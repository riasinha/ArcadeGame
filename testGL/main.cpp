#include <GLUT/glut.h>
//#include < GLUT/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Maze configuration
const int MAZE_WIDTH = 20;
const int MAZE_HEIGHT = 20;
std::vector<std::vector<int> > maze;

// Window configuration
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Character configuration
float characterSize = 0.5f; // The size of the character
float characterX = 1.5f; // The initial X position of the character
float characterY = 1.5f; // The initial Y position of the character
float movementSpeed = 0.8f; // Adjust the movement speed as needed

struct Ghost {
    float x, y; // Position
    float dx, dy; // Direction
    float radius; // Radius of the ghost circle
    float r, g, b; // Color
    float speed = 0.5f; // Speed of the ghost
    // Initialize dx, dy with random direction
    Ghost(float x, float y, float radius, float r, float g, float b) 
        : x(x), y(y), radius(radius), r(r), g(g), b(b), dx(0), dy(0) {}
} ghosts[] = {
    Ghost(3.5f, 3.5f, 0.2f, 1.0f, 0.0f, 0.0f), // Red ghost
    Ghost(2.5f, 5.5f, 0.2f, 1.0f, 0.0f, 1.0f), // Purple ghost
    Ghost(7.5f, 7.5f, 0.2f, 0.0f, 1.0f, 1.0f)  // Turquoise ghost
};

struct Diamond{
    float x, y;
    bool collected;
};

vector<Diamond> diamonds;
int score = 0;
int lives = 3;
bool isWon;
bool isDead = false;
bool levelComplete = false;

int currentLevel = 1;
const int MAX_LEVELS = 5; // Define how many levels you have




void drawHeart(float x, float y, float size) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the heart

    // Upper left triangle
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);
        glVertex2f(x - size, y + size);
        glVertex2f(x, y + 2 * size);
    glEnd();

    // Upper right triangle
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);
        glVertex2f(x + size, y + size);
        glVertex2f(x, y + 2 * size);
    glEnd();

    // Lower rectangle (square)
    glBegin(GL_QUADS);
        glVertex2f(x - size, y);
        glVertex2f(x + size, y);
        glVertex2f(x + size, y - size);
        glVertex2f(x - size, y - size);
    glEnd();
}

void initializeDiamonds(int level){
    diamonds.clear();

    Diamond d1;
    d1.collected = false;

    Diamond d2;
    d2.collected = false;

    if(level == 1){
        d1.x = 18.0f;
        d1.y = 6.0f;
        d2.x = 4.0f;
        d2.y = 13.0f;
    }
    else if(level == 2){
        d1.x = 18.0f;
        d1.y = 4.0f;
        d2.x = 2.0f;
        d2.y = 18.0f;
    }
    else if(level == 3){
        d1.x = 16.0f;
        d1.y = 4.0f;
        d2.x = 18.0f;
        d2.y = 14.0f;
    }
    else if(level == 4){
        d1.x = 17.5f;
        d1.y = 6.0f;
        d2.x = 7.5f;
        d2.y = 9.0f;
    }
    else{
        d1.x = 16.5f;
        d1.y = 9.0f;
        d2.x = 14.0f;
        d2.y = 18.5f;
    }

    
    diamonds.push_back(d1);
    diamonds.push_back(d2);
}

void drawDiamond(float x, float y) {
    glColor3f(1.0f, 0.8f, 0.0f); // Set diamond color
    glBegin(GL_POLYGON); // Start drawing a polygon
    glVertex2f(x, y + 0.2f); // Top
    glVertex2f(x - 0.2f, y); // Left
    glVertex2f(x, y - 0.2f); // Bottom
    glVertex2f(x + 0.2f, y); // Right
    glEnd(); // End of polygon
}


void initializeMazeForLevel(int level) {
    maze.clear(); // Clear existing maze
    const int mazeWidth = 20; // 20x20 matrix
    const int mazeHeight = 20; 
    maze.resize(mazeHeight);
    initializeDiamonds(level);
    if (level == 1) {
        // matrix values for Maze 1
        int rawMaze [20][20] = {
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {2, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2},
            {2, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2},
            {2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 4, 4, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 2},
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
        };
        for (int y = 0; y < mazeHeight; ++y) {
            maze[y].resize(mazeWidth);
            for (int x = 0; x < mazeWidth; ++x) {
                maze[y][x] = rawMaze[y][x];
            }
        }

    } else if (level == 2) {
        // matrix values for Maze 2
        int rawMaze[20][20] = {
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {2, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2},
            {2, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4, 4, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4, 4, 2},
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
        };
        for (int y = 0; y < mazeHeight; ++y) {
            maze[y].resize(mazeWidth);
            for (int x = 0; x < mazeWidth; ++x) {
                maze[y][x] = rawMaze[y][x];
            }
        }
    } else if (level == 3) {
        // matrix values for Maze 3
        int rawMaze[20][20] = {
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2},
            {2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2},
            {2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2},
            {2, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
            {2, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
            {2, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 4, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 4, 4, 2},
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
        };
        for (int y = 0; y < mazeHeight; ++y) {
            maze[y].resize(mazeWidth);
            for (int x = 0; x < mazeWidth; ++x) {
                maze[y][x] = rawMaze[y][x];
            }
        }
    } else if (level == 4) {
        // matrix values for Maze 4
        int rawMaze[20][20] = {
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {2, 3, 3, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2},
            {2, 3, 3, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
            {2, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2},
            {2, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
            {2, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2},
            {2, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
            {2, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2},
            {2, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2},
            {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 4, 4, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 4, 4, 2},
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
        };
        for (int y = 0; y < mazeHeight; ++y) {
            maze[y].resize(mazeWidth);
            for (int x = 0; x < mazeWidth; ++x) {
                maze[y][x] = rawMaze[y][x];
            }
        }
    } else if (level == 5) {
        // matrix values for Maze 5
        int rawMaze[20][20] = {
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {2, 3, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 2},
            {2, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 2},
            {2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 2},
            {2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 2},
            {2, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2},
            {2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2},
            {2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2},
            {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
            {2, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2},
            {2, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 4, 2},
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
        };
        for (int y = 0; y < mazeHeight; ++y) {
            maze[y].resize(mazeWidth);
            for (int x = 0; x < mazeWidth; ++x) {
                maze[y][x] = rawMaze[y][x];
            }
        }
    }

}
void startNextLevel() {
    if (currentLevel < MAX_LEVELS) {
        currentLevel++;
        initializeMazeForLevel(currentLevel);
        levelComplete = false;
        isDead = false;


    } else {
        glColor3f(0.0, 1.0, 0.0); // Red color
        glRasterPos2f(5.0f, 5.0f); // Example position, adjust as needed
        glColor3f(1.0, 0.0, 0.0); // Red color for the text
        glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT / 2); // Set position

        string message = "Congrats you won! Press G to restart the game.";
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }
}

bool isPositionValid(float x, float y) {
    // Check for window boundaries
    if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT) {
        return false;
    }

    // Check for wall collisions (assuming 1 represents a wall in your maze array)
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    // Check for wall and boundary collisions
    // Assuming 1 represents a wall and 2 represents the boundary in your maze array
    if (maze[iy][ix] == 1 || maze[iy][ix] == 2) {
        return false;
    }

    if (maze[iy][ix] == 4) {
        levelComplete = true;
        std::cout << "level complete" << std::endl;
    }

    return true;
}

void keyboard(unsigned char key, int x, int y) {
    float newX, newY;

    if ((isDead || levelComplete) && (key == 'r' || key == 'R')) {
        // Reset game states
        isDead = false;
        levelComplete = false;
        characterX = 1.5f; // Reset to start position
        characterY = 1.5f;
        // You can also reset the ghosts' positions and other game states if needed
        return;
    }

    if ((levelComplete) && (key == 'n' || key == 'N')) {
        // Reset game states
        isDead = false;
        levelComplete = false;
        characterX = 1.5f; // Reset to start position
        characterY = 1.5f;
        startNextLevel();
        // You can also reset the ghosts' positions and other game states if needed
        return;
    }

    if ((isDead || levelComplete) && (key == 'g' || key == 'G')) {
        // Reset game states
        isDead = false;
        levelComplete = false;
        lives = 3;
        initializeMazeForLevel(1);
        characterX = 1.5f; // Reset to start position
        characterY = 1.5f;
        // You can also reset the ghosts' positions and other game states if needed
        return;
    }

    if (!isDead && !levelComplete) {  
        switch (key) {
            case 27: // ESC key
                exit(0);
                break;
            case 'a': // Left
            case 'A':
                newX = characterX - movementSpeed;
                newY = characterY;
                if (isPositionValid(newX, newY)) {
                    characterX = newX;
                }
                break;
            case 'd': // Right
            case 'D':
                newX = characterX + movementSpeed;
                newY = characterY;
                if (isPositionValid(newX, newY)) {
                    characterX = newX;
                }
                break;
            case 'w': // Up
            case 'W':
                newX = characterX;
                newY = characterY + movementSpeed;
                if (isPositionValid(newX, newY)) {
                    characterY = newY;
                }
                break;
            case 's': // Down
            case 'S':
                newX = characterX;
                newY = characterY - movementSpeed;
                if (isPositionValid(newX, newY)) {
                    characterY = newY;
                }
                break;

            if (maze[static_cast<int>(characterY)][static_cast<int>(characterX)] == 4) {
                levelComplete = true;
                std::cout << "You Won!" << std::endl; // Print to console
            }
        }
    }

    glutPostRedisplay();
}

void drawCharacter() {
    glColor3f(0.53f, 0.81f, 0.98f); // Character color

    glBegin(GL_QUADS);
        glVertex2f(characterX - characterSize, characterY - characterSize); // bottom-left
        glVertex2f(characterX + characterSize, characterY - characterSize); // bottom-right
        glVertex2f(characterX + characterSize, characterY + characterSize); // top-right
        glVertex2f(characterX - characterSize, characterY + characterSize); // top-left
    glEnd();
}

void drawGhost(Ghost ghost) {

    // Calculate direction vector towards the player
    float dx = characterX - ghost.x;
    float dy = characterY - ghost.y;
    float distance = sqrt(dx * dx + dy * dy);
    
    // Normalize the direction
    dx /= distance;
    dy /= distance;
    
    // Move the ghost towards the player
    ghost.x += dx * ghost.speed;
    ghost.y += dy * ghost.speed;

    const int numSegments = 16; // Number of segments used to draw the circle

    glColor3f(ghost.r, ghost.g, ghost.b); // Ghost color
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(ghost.x, ghost.y); // Center of circle
        for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
            float theta = 2.0f * 3.1415926f * float(i) / float(numSegments); // Current angle
            float x = ghost.radius * cosf(theta); // Calculate the x component
            float y = ghost.radius * sinf(theta); // Calculate the y component
            glVertex2f(x + ghost.x, y + ghost.y);
        }
    glEnd();
}

void drawMaze() {
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (maze[y][x] == 1) {
                glColor3f(0.0f, 0.0f, 1.0f); // Blue color for walls
            } else if (maze[y][x] == 2) {
                glColor3f(0.0f, 0.0f, 0.0f); // Black color for the border
            } else if (maze[y][x] == 3) {
                glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the start
            } else if (maze[y][x] == 4) {
                glColor3f(0.0f, 1.0f, 0.0f); // Green color for the end
            } else {
                glColor3f(1.0f, 1.0f, 1.0f); // White color for paths
            }

            glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + 1, y);
                glVertex2f(x + 1, y + 1);
                glVertex2f(x, y + 1);
            glEnd();
        }
    }
}

void updateGhostPositions(double deltaTime) {
    if (isDead) {
        return; // Don't update ghost positions if the player is dead
    }
    static float changeDirectionTimer = 0;
    static float directionChangeInterval = 2.0f; // Change direction every 2 seconds

    changeDirectionTimer += static_cast<float>(deltaTime);

    // Change direction periodically
    if (changeDirectionTimer >= directionChangeInterval) {
        for (Ghost& ghost : ghosts) {
            // Randomly assign new directions
            ghost.dx = (rand() % 3 - 1) * ghost.speed; // -1, 0, or 1
            ghost.dy = (rand() % 3 - 1) * ghost.speed;
        }
        changeDirectionTimer = 0; // Reset the timer
    }

    // Update ghost positions smoothly
    for (Ghost& ghost : ghosts) {
        float newX = ghost.x + ghost.dx * static_cast<float>(deltaTime);
        float newY = ghost.y + ghost.dy * static_cast<float>(deltaTime);

        // Check if the new position is valid before updating
        if (isPositionValid(newX, newY)) {
            ghost.x = newX;
            ghost.y = newY;
        }
    }
}

void updateGameLogic(){
    // Check for diamond collection
    for (auto& diamond : diamonds) {
        if (!diamond.collected && fabs(characterX - diamond.x) < 0.5f && fabs(characterY - diamond.y) < 0.5f) {
            diamond.collected = true;
            score += 1000; // Increase score
        }
    }
}



bool checkCollisionWithGhosts() {
    for (const Ghost& ghost : ghosts) {
        float distance = sqrt((characterX - ghost.x) * (characterX - ghost.x) +
                              (characterY - ghost.y) * (characterY - ghost.y));
        if (distance < characterSize + ghost.radius) {
            return true;
        }
    }
    return false;
}

void displayScore(){
    glColor3f(1.0, 0.0, 0.0); // Red color
    glRasterPos2f(5.0f, 5.0f); // Example position, adjust as needed
    glColor3f(1.0, 0.0, 0.0); // Red color for the text
    glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT / 2); // Set position

    string message = "";
    message = "Score: ";
    cout << score << endl;
    for (char c : message) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!isDead) {
        isDead = checkCollisionWithGhosts();
        if (isDead) {
            lives--;
            if(score >= 500) score -= 500;
        }
    }
    
    drawMaze();
    drawCharacter();
    updateGameLogic();

    for (Ghost &ghost : ghosts) {
        drawGhost(ghost);
    }


    for (const auto& diamond : diamonds) {
        if (!diamond.collected) {
            drawDiamond(diamond.x, diamond.y);
        }
    }

    float heartSize = 0.5f; // Adjust the size as needed
    float startX = MAZE_WIDTH - 3 * heartSize - 1; // Starting X position
    float startY = MAZE_HEIGHT - heartSize - 1; // Starting Y position
    for (int i = 0; i < lives; i++) {
        drawHeart(startX + i * heartSize, startY, heartSize);
    }

    // Put Score at Top
    glColor3f(1.0, 0.0, 0.0); // Red color
    glRasterPos2f(19.5f, 19.5f); // Example position, adjust as needed
    glColor3f(1.0, 0.0, 0.0); // Red color for the text
    glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT-27); // Set position

    string message = "SCORE: " + to_string(score);
    
    for (char c : message) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    if (isDead) {
        // Display "You Died" message
        glColor3f(1.0, 0.0, 0.0); // Red color
        glRasterPos2f(5.0f, 5.0f); // Example position, adjust as needed
        glColor3f(1.0, 0.0, 0.0); // Red color for the text
        glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT / 2); // Set position

        string message = "";
        if (lives == 0) {
            message = "You lost! Game over. Press G to play again.";
            displayScore();

        }
        else {
            message = "Oh no! You lost a life!";
        }
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }


    }

    if (levelComplete) {
        // Display "You Won" message
        glColor3f(0.0, 1.0, 0.0); // Green color
        glColor3f(0.0, 1.0, 0.0); // Green color for the text
        glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT / 2); // Set position
        std::string message = "Level complete! Press N to go to the next level!";
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }



    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    glutCreateWindow("Maze Game");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, MAZE_WIDTH, 0.0, MAZE_HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    initializeMazeForLevel(currentLevel);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}