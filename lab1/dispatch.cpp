#include "dispatch.h"

void mouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (MODE == 0) {
            addVertex(x, y);
        } else if (MODE == 1) {
            moveVertex(x, y);
        }
    }
}

void moveVertex(int x, int y) {
    primitives[primitives.size() - 1].moveVertex(x, y);

    glutPostRedisplay();
}

void addVertex(int x, int y) {
    std::cout << "Add vertex [" << x << "; " << y << "]" << std::endl;
    primitives[primitives.size() - 1].addVertex(x, y);

    glutPostRedisplay();
}

void keyPressedEvent(unsigned char key, int x, int y) {
    for (auto command : commands) {
        if (command.getKey() == key) {
            command.action();
        }
    }
}

void keyUpEvent(unsigned char key, int x, int y) {
    if (key == 'h') {
        showHelp(false);
    }
}

void clear() {
    primitives.clear();
    primitives.emplace_back();

    glutPostRedisplay();
}

void newPrimitive() {
    primitives.emplace_back();

    glutPostRedisplay();
}

void resizeEvent(int width, int height) {
    WIDTH = width;
    HEIGHT = height;

    glViewport(0, 0, width, height);
}

void menuEvent(int value) {
    for (auto command : commands) {
        if (command.getKey() == value) {
            command.action();
        }
    }
}

void delLastVertex() {
    if (!primitives.empty()){
        primitives[primitives.size() - 1].deleteLastVertex();

        glutPostRedisplay();
    }
}

void delLastPrimitive() {
    primitives.pop_back();

    if (primitives.empty()) {
        primitives.emplace_back();
    }

    glutPostRedisplay();
}

void changeColor(double red, double green, double blue) {
    primitives[primitives.size() - 1].r = red;
    primitives[primitives.size() - 1].g = green;
    primitives[primitives.size() - 1].b = blue;

    glutPostRedisplay();
}

void setMode(int mode) {
    auto prev = MODE;
    MODE = mode;

    if (prev != mode) {
        glutPostRedisplay();
    }
}

void showHelp(bool show) {
    auto prev = SHOW_HELP;
    SHOW_HELP = show;
    if (prev != show) {
        glutPostRedisplay();
    }
}

void setModeAdd() {
    setMode(0);
}

void setModeMove() {
    setMode(1);
}

void changeColorToRed() {
    changeColor(1, 0, 0);
}

void changeColorToGreen() {
    changeColor(0, 1, 0);
}

void changeColorToBlue() {
    changeColor(0, 0, 1);
}

void showHelpInfo() {
    showHelp(true);
}