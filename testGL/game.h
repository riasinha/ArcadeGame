
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <GLUT/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Constants
const int MAZE_WIDTH = 20;
const int MAZE_HEIGHT = 20;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
int currentLevel = 1;
std::vector<std::vector<int> > maze;


// Character configuration
float characterSize = 0.5f; // The size of the character
float characterX = 1.5f; // The initial X position of the character
float characterY = 1.5f; // The initial Y position of the character
float movementSpeed = 0.8f; // Adjust the movement speed as needed


// GameState enum
enum GameState {
    START_SCREEN,
    IN_GAME,
    PAUSE_SCREEN,
    GAME_OVER,
    LEADERBOARD_SCREEN
};

GameState currentGameState = GameState::START_SCREEN;

// Ghost struct
struct Ghost {
    float x, y; // Position
    float dx, dy; // Direction
    float radius; // Radius of the ghost circle
    float r, g, b; // Color
    float speed; // Speed of the ghost

    Ghost(float x, float y, float radius, float r, float g, float b);
};

// Diamond struct
struct Diamond {
    float x, y;
    bool collected;
};

vector<Diamond> diamonds;
int score = 0;
int lives = 3;
bool isWon;
bool isDead = false;
bool levelComplete = false;

const int MAX_LEVELS = 5; // Define how many levels you have
string leaderboardMessage = "";
const string HIGH_SCORE_FILE = "highscores.txt";
vector<int> highScores;  // Global variable for storing high scores


// Function Prototypes
void readHighScores();
void displayLeaderboard();
void writeHighScores();
void updateLeaderboard(int newScore);
void drawStartScreen();
void drawStar(float centerX, float centerY, float size);
void initializeDiamonds(int level);
void drawDiamond(float x, float y);
void initializeMazeForLevel(int level);
void startNextLevel();
bool isPositionValid(float x, float y);
void keyboard(unsigned char key, int x, int y);
void drawCharacter();
void drawGhost(Ghost ghost);
void drawMaze();
void updateGhostPositions(double deltaTime);
void updateGameLogic();
bool checkCollisionWithGhosts();
void displayScore();
void display();

#endif

