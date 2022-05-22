#include "dispatch.h"

void keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        cout << (char) key << endl;

        for (auto command: commands) {
            if ((int)command.getKey() == key) {
                command.action(mods);
            }
        }
    } else if (action == GLFW_RELEASE) {
        if (key == 'H') {
//            showHelp(false);
        }
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouseEvent(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            // in px from upper left corner

            cout << "Click " << xpos << "; " << ypos << endl;

            state.leftClick(xpos, ypos);
            //        if (MODE == 0) {
//            addVertex(x, y);
//        } else if (MODE == 1) {
//            moveVertex(x, y);
//        }
        }
    }
}

void scrollEvent(GLFWwindow *window, double xOffset, double yOffset) {
    cout << "Scroll event " << xOffset << "; " << yOffset << endl;

    state.scroll(yOffset);
}




//void moveVertex(int x, int y) {
//    primitives[primitives.size() - 1].moveVertex(x, y);
//
//    glutPostRedisplay();
//}

//void addVertex(int x, int y) {
//    std::cout << "Add vertex [" << x << "; " << y << "]" << std::endl;
//    primitives[primitives.size() - 1].addVertex(x, y);
//
//    glutPostRedisplay();
//}

void clear() {
    state.clear();
}
//
//void newPrimitive() {
//    primitives.emplace_back(0, 0, 1);
//
//    glutPostRedisplay();
//}

void resizeEvent(GLFWwindow *window, int width, int height) {
    state.WIDTH = width;
    state.HEIGHT = height;

    glViewport(0, 0, width, height);
}

//void menuEvent(int value) {
//    for (auto command: commands) {
//        if (command.getKey() == value) {
//            command.action(0);
//        }
//    }
//}

//void delLastVertex() {
//    if (!primitives.empty()){
//        primitives[primitives.size() - 1].deleteLastVertex();
//
//        glutPostRedisplay();
//    }
//}
//
//void delLastPrimitive() {
//    primitives.pop_back();
//
//    if (primitives.empty()) {
//        primitives.emplace_back(0, 0, 1);
//    }
//
//    glutPostRedisplay();
//}
//
//void changeColor(double red, double green, double blue) {
//    primitives[primitives.size() - 1].r = red;
//    primitives[primitives.size() - 1].g = green;
//    primitives[primitives.size() - 1].b = blue;
//
//    glutPostRedisplay();
//}

//void setMode(int mode) {
//    auto prev = MODE;
//    MODE = mode;
//
//    if (prev != mode) {
//        glutPostRedisplay();
//    }
//}
//
//void showHelp(bool show) {
//    auto prev = SHOW_HELP;
//    SHOW_HELP = show;
//    if (prev != show) {
//        glutPostRedisplay();
//    }
//}
//
//void setModeAdd() {
//    setMode(0);
//}
//
//void setModeMove() {
//    setMode(1);
//}
//
//void changeColorToRed() {
//    changeColor(1, 0, 0);
//}
//
//void changeColorToGreen() {
//    changeColor(0, 1, 0);
//}
//
//void changeColorToBlue() {
//    changeColor(0, 0, 1);
//}
//
//void showHelpInfo() {
//    showHelp(true);
//}