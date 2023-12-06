#include <GLUT/glut.h>
//#include < GLUT/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Maze configuration
const int MAZE_WIDTH = 15;
const int MAZE_HEIGHT = 15;
std::vector<std::vector<int> > maze;

// Window configuration
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Character configuration
float characterSize = 0.1f; // The size of the character
float characterX = 1.5f; // The initial X position of the character
float characterY = 1.5f; // The initial Y position of the character
float movementSpeed = 0.15f; // Adjust the movement speed as needed

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
    Ghost(5.5f, 5.5f, 0.2f, 1.0f, 0.0f, 1.0f), // Purple ghost
    Ghost(7.5f, 7.5f, 0.2f, 0.0f, 1.0f, 1.0f)  // Turquoise ghost
};


bool isDead = false;
bool levelComplete = false;


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
        // Reset game state
        isDead = false;
        levelComplete = false;
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
}

void initializeMaze() {

    maze.resize(MAZE_HEIGHT);

    int rawMaze[MAZE_HEIGHT][MAZE_WIDTH] = {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 2},
        {2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2},
        {2, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 2},
        {2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2},
        {2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2},
        {2, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2},
        {2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        maze[y].resize(MAZE_WIDTH);
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            maze[y][x] = rawMaze[y][x];
        }
    }
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

void update(int value) {
    // Update logic here
    double deltaTime = value / 1000.0;
    updateGhostPositions(deltaTime);

    glutPostRedisplay(); // Redraw the window
    glutTimerFunc(16, update, 16); // Schedule next update in 16ms
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!isDead) {
        isDead = checkCollisionWithGhosts();
    }

    drawMaze();
    drawCharacter();
    for (Ghost &ghost : ghosts) {
        drawGhost(ghost);
    }

    if (isDead) {
        // Display "You Died" message
        glColor3f(1.0, 0.0, 0.0); // Red color
        glRasterPos2f(5.0f, 5.0f); // Example position, adjust as needed
        glColor3f(1.0, 0.0, 0.0); // Red color for the text
        glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT / 2); // Set position
        std::string message = "You Died";
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    if (levelComplete) {
        // Display "You Won" message
        glColor3f(0.0, 1.0, 0.0); // Green color
        glColor3f(0.0, 1.0, 0.0); // Green color for the text
        glWindowPos2i(SCR_WIDTH / 2 - 50, SCR_HEIGHT / 2); // Set position
        std::string message = "You Won!";
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

    initializeMaze();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}