#include "dispatch.h"

void addVertex(int x, int y);

void moveVertex(int x, int y);

void newPrimitive();

void clear();

void changeColor(double red, double green, double blue);

void setMode(int mode);

void showHelp(bool show);

void delLastPrimitive();

void delLastVertex();

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
    if (key == ' ') {
        newPrimitive();
    } else if (key == 'c') {
        clear();
    } else if (key == 'm') {
        setMode(1);
    } else if (key == 'a') {
        setMode(0);
    } else if (key == 'r') {
        changeColor(1, 0, 0);
    } else if (key == 'g') {
        changeColor(0, 1, 0);
    } else if (key == 'b') {
        changeColor(0, 0, 1);
    } else if (key == 'h') {
        showHelp(true);
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
    if (value == ' ') {
        newPrimitive();
    } else if (value == 'c') {
        clear();
    } else if (value == 'd') {
        delLastPrimitive();
    } else if (value == 'p') {
        delLastVertex();
    } else if (value == 'm') {
        MODE = 1;
        glutPostRedisplay();
    } else if (value == 'a') {
        MODE = 0;
        glutPostRedisplay();
    } else if (value == 'r') {
        changeColor(1, 0, 0);
    } else if (value == 'g') {
        changeColor(0, 1, 0);
    } else if (value == 'b') {
        changeColor(0, 0, 1);
    }
}

void delLastVertex() {
    primitives[primitives.size() - 1].deleteLastVertex();

    glutPostRedisplay();
}

void delLastPrimitive() {
    primitives.pop_back();
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

        std::cout << "R" << prev << " " << show << std::endl;
        glutPostRedisplay();
    }
}
