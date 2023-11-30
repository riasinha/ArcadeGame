#include "dependencies/include/GLFW/glfw3.h"
#include <iostream>
#include <vector>

// Define the maze grid size based on your image (this should match your maze's dimensions)
const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 10;

// This is a placeholder for where you would load your image and process it to a maze
// The actual implementation of image loading and processing is not provided
std::vector<std::vector<int> > maze(MAZE_HEIGHT, std::vector<int>(MAZE_WIDTH));

void initializeMaze() {
    int rawMaze[MAZE_HEIGHT][MAZE_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            maze[y][x] = rawMaze[y][x];
        }
    }
}

void drawMaze() {
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (maze[y][x] == 1) {
                // Draw a wall
                glColor3f(0.0f, 0.0f, 0.0f); // Black color for walls
            } else {
                // Draw a path
                glColor3f(1.0f, 1.0f, 1.0f); // White color for paths
            }

            // Draw a quad at the (x, y) position
            glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x + 1, y);
                glVertex2i(x + 1, y + 1);
                glVertex2i(x, y + 1);
            glEnd();
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple Maze Renderer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initializeMaze();

    // Set up an orthogonal projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset the projection to the identity matrix
    glOrtho(0.0, double(MAZE_WIDTH), double(MAZE_HEIGHT), 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawMaze();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

