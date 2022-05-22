#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/freeglut.h>
#include "Primitive.h"
#include "dispatch.h"
#include "../lib/Command.h"

#include "Shader.h"
#include "Grid.h"
#include "State.h"

using namespace std;

int MODE = 0;
bool SHOW_HELP = false;

State state;

string help;
vector<Command> commands;

vector<Octagon> primitives;

void initHelp(const vector<Command> &commands);

void renderString(float x, float y, void *font, const char *string) {
    glColor3f(0, 1, 1);
    glRasterPos2f(x, y);

    glutBitmapString(font, reinterpret_cast<const unsigned char *>(string));
}

void renderScene(int shader) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

//    if (MODE == 0) {
//        renderString(-0.9, -0.9, GLUT_BITMAP_9_BY_15, "Mode: add vertices");
//    } else if (MODE == 1) {
//        renderString(-0.9, -0.9, GLUT_BITMAP_9_BY_15, "Mode: move vertices");
//    }

    if (SHOW_HELP) {
        renderString(-0.9, 0.9, GLUT_BITMAP_9_BY_15, help.data());
    }

    for (int i = 0; i < primitives.size(); i++) {
        primitives[i].render(shader);
    }

//    glutSwapBuffers();
}

void initMenu(vector<Command> &commands) {
//    glutCreateMenu(menuEvent);

    for (const auto &command: commands) {
        if (command.getMenuLabel() != nullptr) {
//            cout << "Label: " << command.getMenuLabel() << endl;
            glutAddMenuEntry(command.getMenuLabel(), command.getKey());
        }
    }

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initCommands() {
//    commands.emplace_back(GLUT_LEFT_BUTTON, nullptr,
//                          "dep. on mode (add vertex to current primitive/move closest vertex\nof current primitive to pointer position",
//                          nullptr);
//    commands.emplace_back(GLUT_RIGHT_BUTTON, nullptr, "open menu", nullptr);
//    commands.emplace_back('h', nullptr, "show help", showHelpInfo);
//
//    commands.emplace_back('a', "Mode: Add vertices", "set mode to add vertex to current primitive", setModeAdd);
//    commands.emplace_back('m', "Mode: Move vertices", "set mode to move closest vertex", setModeMove);
//    commands.emplace_back(' ', "New primitive", "create new primitive", newPrimitive);
//    commands.emplace_back('r', "Change color to red", "set color of current primitive red", changeColorToRed);
//    commands.emplace_back('g', "Change color to green", "set color of current primitive green", changeColorToGreen);
//    commands.emplace_back('b', "Change color to blue", "set color of current primitive blue", changeColorToBlue);
////    commands.emplace_back('p', "Delete last vertex", "delete last vertex", delLastVertex);
//    commands.emplace_back('d', "Delete current primitive", "delete current primitive", delLastPrimitive);


    commands.emplace_back('C', "Clear screen", "clearColors screen", []() { state.clear(); });
    commands.emplace_back('R', "Real|Rasterized", "real/rasterized mode", []() {
        state.showRasterized = !state.showRasterized;
        state.showReal = !state.showReal;
    });

    commands.emplace_back(GLFW_KEY_UP, "Move Up last octagon", "move up last octagon", []() {
        state.moveLast(vec2(0, 0.2));
    });
    commands.emplace_back(GLFW_KEY_DOWN, "Move Down last octagon", "move down last octagon", []() {
        state.moveLast(vec2(0, -0.2));
    });
    commands.emplace_back(GLFW_KEY_RIGHT, "Move Right last octagon", "move right last octagon", []() {
        state.moveLast(vec2(0.2, 0));
    });
    commands.emplace_back(GLFW_KEY_LEFT, "Move Left last octagon", "move left last octagon", []() {
        state.moveLast(vec2(-0.2, 0.0));
    });

    commands.emplace_back('Q', "Rotate counterclockwise last octagon", "rotate counterclockwise last octagon", []() {
        state.rotateLast(-0.2);
    });

    commands.emplace_back('E', "Rotate clockwise last octagon", "rotate clockwise last octagon", []() {
        state.rotateLast(0.2);
    });

    commands.emplace_back('=', "Inc size of last octagon", "inc size of  last octagon", []() {
        state.changeLastSize(1.1);
    });

    commands.emplace_back('-', "Dec size of last octagon", "dec size of  last octagon", []() {
        state.changeLastSize(0.9);
    });


    commands.emplace_back('I', "Inc/dec red", "inc/dec red", [](int mod) {
        if (mod & GLFW_MOD_CONTROL) {
            state.changeLastColor(vec3(0.6, 1, 1));
        } else {
            state.changeLastColor(vec3(1.4, 1, 1));
        }
    });

    commands.emplace_back('O', "Inc/dec green", "inc/dec green", [](int mod) {
        if (mod & GLFW_MOD_CONTROL) {
            state.changeLastColor(vec3(1, 0.6, 1));
        } else {
            state.changeLastColor(vec3(1, 1.4, 1));
        }
    });

    commands.emplace_back('P', "Inc/dec blue", "inc/dec blue", [](int mod) {
        if (mod & GLFW_MOD_CONTROL) {
            state.changeLastColor(vec3(1, 1, 0.6));
        } else {
            state.changeLastColor(vec3(1, 1, 1.4));
        }
    });
}

void initHelp(const vector<Command> &commands) {
    ostringstream helpStream;

    for (const auto &command: commands) {
        helpStream << command.getHelpText();
    }

    help = helpStream.str();
}

void GLAPIENTRY
MessageCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message,
                const void *userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

int main(int argc, char **argv) {
    if (!glfwInit()) {
        cerr << "Can't initialize GLFW!" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state.window = glfwCreateWindow(state.WIDTH, state.HEIGHT, "Lab 2", nullptr, nullptr);
    if (state.window == nullptr) {
        glfwTerminate();
        cerr << "Can't create window!" << endl;
        return -1;
    }

    glfwMakeContextCurrent(state.window);

    glfwSetWindowAspectRatio(state.window, 1, 1);

    glfwSetKeyCallback(state.window, keyEvent);
    glfwSetMouseButtonCallback(state.window, mouseEvent);
    glfwSetScrollCallback(state.window, scrollEvent);


    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions

    glfwSetFramebufferSizeCallback(state.window, resizeEvent);

    glfwGetFramebufferSize(state.window, &state.WIDTH, &state.HEIGHT);
    glViewport(0, 0, state.WIDTH, state.HEIGHT);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    state.shader = LoadShader("/home/danylo/CLionProjects/graphics/shader.vert", "/home/danylo/CLionProjects/graphics/shader.frag");

    initCommands();

    initHelp(commands);
    cout << "Help:\n" << help << "\n" << endl;


//    Grid grid = Grid(100);
    Grid grid = Grid(50); // FIXME

    state.grid = grid;

    long frame = 0;

    while (!glfwWindowShouldClose(state.window)) {
        cout << "Frame: " << frame++ << endl;

        glfwPollEvents();

        state.render();

//        cout << "\n\n";
    }

    glfwDestroyWindow(state.window);
    glfwTerminate();
}
