#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

// Maze configuration
const int MAZE_WIDTH = 15;
const int MAZE_HEIGHT = 15;
std::vector<std::vector<int>> maze;

// Window configuration
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Character configuration
float characterSize = 0.1f; // The size of the character
float characterX = 1.5f; // The initial X position of the character
float characterY = 1.5f; // The initial Y position of the character
float movementSpeed = 0.05f; // Adjust the movement speed as needed

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

    return true;
}

void initializeMaze() {
    maze = {
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
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    float newX, newY;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        newX = characterX - movementSpeed;
        newY = characterY;
        if (isPositionValid(newX, newY)) {
            characterX = newX;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        newX = characterX + movementSpeed;
        newY = characterY;
        if (isPositionValid(newX, newY)) {
            characterX = newX;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        newX = characterX;
        newY = characterY + movementSpeed;
        if (isPositionValid(newX, newY)) {
            characterY = newY;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        newX = characterX;
        newY = characterY - movementSpeed;
        if (isPositionValid(newX, newY)) {
            characterY = newY;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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


int main() {
    srand(time(NULL)); // Seed the random number generator

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maze Game", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    initializeMaze(); // Populate the maze data

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, MAZE_WIDTH, 0.0, MAZE_HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);
        updateGhostPositions(deltaTime); // Update positions with delta time

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawMaze();
        drawCharacter();
        for (Ghost &ghost : ghosts) {
            drawGhost(ghost);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
