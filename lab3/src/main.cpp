#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <GL/freeglut.h>

#define DEBUG

#include "GlUtills.h"

#include "dispatch.h"

#include "State.h"

using namespace std;

State state;

void initCommands() {
    state.commands.emplace_back(GLUT_KEY_UP, nullptr, "Move Up last octagon", []() {
        state.moveTarget(vec2(0, 0.5));
    }, SPECIAL_KEY);

    state.commands.emplace_back(GLUT_KEY_DOWN, nullptr, "Move Down last octagon", []() {
        state.moveTarget(vec2(0, -0.5));
    }, SPECIAL_KEY);
    state.commands.emplace_back(GLUT_KEY_RIGHT, nullptr, "Move Right last octagon", []() {
        state.moveTarget(vec2(0.5, 0));
    }, SPECIAL_KEY);
    state.commands.emplace_back(GLUT_KEY_LEFT, nullptr, "Move Left last octagon", []() {
        state.moveTarget(vec2(-0.5, 0.0));
    }, SPECIAL_KEY);

    state.commands.emplace_back('w', "Move forward", "move forward",
                                []() {
                                    state.moveCamera(vec3(0, 0, 1));
                                });
    state.commands.emplace_back('a', "Move backward", "move backward",
                                []() {
                                    state.moveCamera(vec3(-1, 0, 0));
                                });
    state.commands.emplace_back('s', "Move left", "move left",
                                []() {
                                    state.moveCamera(vec3(0, 0, -1));
                                });
    state.commands.emplace_back('d', "Move right", "move right",
                                []() {
                                    state.moveCamera(vec3(1, 0, 0));
                                });
    state.commands.emplace_back(' ', "Move up", "move up",
                                []() {
                                    state.moveCamera(vec3(0, 1, 0));
                                });
    state.commands.emplace_back(112, "Move down", "move down",
                                []() {
                                    state.moveCamera(vec3(0, -1, 0));
                                }, 16);


    state.commands.emplace_back('m', "Toggle norm smoothing", "toggle norm smoothing",
                                []() {
                                    state.toggleSmoothing();
                                });

    state.commands.emplace_back('n', "Toggle display norm", "toggle display norm",
                                []() {
                                    state.toggleNorm();
                                });

    state.commands.emplace_back('t', "Toggle textures", "toggle textures",
                                []() {
                                    state.toggleTextures();
                                });

    state.commands.emplace_back('p', "Toggle projection perspective/ortho", "toggle projection perspective/ortho",
                                []() {
                                    state.toggleProjections();
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
    initGl(argc, argv, state.WIDTH, state.HEIGHT, "Lab 3");
    glEnable(GL_DEPTH_TEST);

    state.primitiveShader = loadShaders("/home/danylo/CLionProjects/graphics/lab3/shaders/lab3.vert",
                                        "/home/danylo/CLionProjects/graphics/lab3/shaders/lab3.frag");
    state.lightShader = loadShaders("/home/danylo/CLionProjects/graphics/lab3/shaders/lab3.vert",
                                    "/home/danylo/CLionProjects/graphics/lab3/shaders/lightsource.frag");

    state.vectorShader = loadShaders("/home/danylo/CLionProjects/graphics/lab3/shaders/vector.vert",
                                    "/home/danylo/CLionProjects/graphics/lab3/shaders/vector.frag");


    initCommands();

    state.help = Command::initHelp(state.commands);

    cout << "\n-----------------------------------------------------------------------" << endl;
    cout << "Help:\n" << state.help << "\n" << endl;
    cout << "\n-----------------------------------------------------------------------" << endl;

    initMenu(state.commands);

    state.init();

    glutDisplayFunc([]() { state.render(); });
//    glutMouseFunc(mouseEvent);
    glutKeyboardFunc([](unsigned char key, int x, int y) { keyEvent(key, GLFW_PRESS, glutGetModifiers()); });
    glutKeyboardUpFunc([](unsigned char key, int x, int y) { keyEvent(key, GLFW_RELEASE, glutGetModifiers()); });
    glutSpecialFunc([](int key, int x, int y) { keyEvent(key, GLFW_PRESS, glutGetModifiers() | SPECIAL_KEY); });
//    glutMouseWheelFunc(scrollEvent);

    glutReshapeFunc(resizeEvent);

    glutMainLoop();
}
