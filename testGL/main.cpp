#include "game.h"

#include <GLUT/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

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
    readHighScores(); // Read the high scores at the start

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}