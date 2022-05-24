#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <GL/freeglut.h>
#include "GlUtills.h"

#include "dispatch.h"

#include "Grid.h"
#include "State.h"

using namespace std;

State state;

void initCommands() {
    state.commands.emplace_back('c', "Clear screen", "clear screen", []() { state.clear(); });
    state.commands.emplace_back('r', "Real / Rasterized", "real/rasterized mode", []() {
        state.setRasterized(!state.showRasterized);
    });

    state.commands.emplace_back(GLUT_KEY_UP, nullptr, "Move Up last octagon", []() {
        state.moveLast(vec2(0, 0.2));
    }, SPECIAL_KEY);

    state.commands.emplace_back(GLUT_KEY_DOWN, nullptr, "Move Down last octagon", []() {
        state.moveLast(vec2(0, -0.2));
    }, SPECIAL_KEY);
    state.commands.emplace_back(GLUT_KEY_RIGHT, nullptr, "Move Right last octagon", []() {
        state.moveLast(vec2(0.2, 0));
    }, SPECIAL_KEY);
    state.commands.emplace_back(GLUT_KEY_LEFT, nullptr, "Move Left last octagon", []() {
        state.moveLast(vec2(-0.2, 0.0));
    }, SPECIAL_KEY);

    state.commands.emplace_back('q', "Rotate counterclockwise last octagon", "rotate counterclockwise last octagon",
                                []() {
                                    state.rotateLast(0.2);
                                });

    state.commands.emplace_back('e', "Rotate clockwise last octagon", "rotate clockwise last octagon", []() {
        state.rotateLast(-0.2);
    });

    state.commands.emplace_back('=', "Inc size of last octagon", "inc size of  last octagon", []() {
        state.changeLastSize(1.1);
    });

    state.commands.emplace_back('-', "Dec size of last octagon", "dec size of  last octagon", []() {
        state.changeLastSize(0.9);
    });

    state.commands.emplace_back('i', nullptr, "+ red", []() {
        state.changeLastColor(vec3(1.4, 1, 1));
    });

    state.commands.emplace_back('i', nullptr, "- red", []() {
        state.changeLastColor(vec3(0.6, 1, 1));
    }, GLUT_ACTIVE_CTRL);

    state.commands.emplace_back('o', nullptr, "+ green", []() {
        state.changeLastColor(vec3(1, 1.4, 1));
    });

    state.commands.emplace_back('o', nullptr, "- green", []() {
        state.changeLastColor(vec3(1, 0.6, 1));
    }, GLUT_ACTIVE_CTRL);

    state.commands.emplace_back('p', nullptr, "+ blue", []() {
        state.changeLastColor(vec3(1, 1, 1.4));
    });

    state.commands.emplace_back('p', nullptr, "- blue", []() {
        state.changeLastColor(vec3(1, 1, 0.6));
    }, GLUT_ACTIVE_CTRL);

    state.commands.emplace_back('a', "AND mix", "and op", []() {
        state.setLogicOp(GL_AND);
    });

    state.commands.emplace_back('n', "NAND mix", "nand op", []() {
        state.setLogicOp(GL_NAND);
    });

    state.commands.emplace_back('m', "No mix", "no op", []() {
        state.setLogicOp(GL_COPY);
    });

    state.commands.emplace_back('f', "Fill on/off", "fill on/off", []() {
        state.setFill(!state.fill);
    });
}

void initMenu(vector<Command> &commands) {
    glutCreateMenu([](int key) { keyEvent(key, GLFW_PRESS, 0); });

    for (const auto &command: commands) {
        if (command.getMenuLabel() != nullptr) {
            glutAddMenuEntry(command.getMenuLabel(), command.getKey());
        }
    }

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    initGl(argc, argv, state.WIDTH, state.HEIGHT, "Lab 2");

    state.primitiveShader = loadShaders("/home/danylo/CLionProjects/graphics/primitive.vert",
                                        "/home/danylo/CLionProjects/graphics/primitive.frag");
    state.gridShader = loadShaders("/home/danylo/CLionProjects/graphics/grid.vert",
                                   "/home/danylo/CLionProjects/graphics/grid.frag");

    initCommands();

    state.help = Command::initHelp(state.commands);

    cout << "\n-----------------------------------------------------------------------" << endl;
    cout << "Help:\n" << state.help << "\n" << endl;
    cout << "\n-----------------------------------------------------------------------" << endl;

    initMenu(state.commands);

    Grid grid = Grid(100);
    state.grid = grid;

    glutDisplayFunc([]() { state.render(); });
    glutMouseFunc(mouseEvent);
    glutKeyboardFunc([](unsigned char key, int x, int y) { keyEvent(key, GLFW_PRESS, glutGetModifiers()); });
    glutKeyboardUpFunc([](unsigned char key, int x, int y) { keyEvent(key, GLFW_RELEASE, glutGetModifiers()); });
    glutSpecialFunc([](int key, int x, int y) { keyEvent(key, GLFW_PRESS, glutGetModifiers() | SPECIAL_KEY); });
    glutMouseWheelFunc(scrollEvent);

    glutReshapeFunc(resizeEvent);
    
    glutMainLoop();
}
