#include <iostream>
#include <vector>

#include <GL/freeglut.h>
#include "Primitive.h"
#include "dispatch.h"
#include "../lib/Command.h"

using namespace std;

int HEIGHT = 600, WIDTH = 960;
int MODE = 0;
bool SHOW_HELP = false;

string help;
vector<Command> commands;

vector<Primitive> primitives;

void initHelp(const vector<Command> &commands);

void renderString(float x, float y, void *font, const char *string) {
    glColor3f(0, 1, 1);
    glRasterPos2f(x, y);

    glutBitmapString(font, reinterpret_cast<const unsigned char *>(string));
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (MODE == 0) {
        renderString(-0.9, -0.9, GLUT_BITMAP_9_BY_15, "Mode: add vertices");
    } else if (MODE == 1) {
        renderString(-0.9, -0.9, GLUT_BITMAP_9_BY_15, "Mode: move vertices");
    }

    if (SHOW_HELP) {
        renderString(-0.9, 0.9, GLUT_BITMAP_9_BY_15, help.data());
    }

    for (int i = 0; i < primitives.size(); i++) {
        primitives[i].render(i == primitives.size() - 1);
    }

    glutSwapBuffers();
}

void initMenu(vector<Command> &commands) {
    glutCreateMenu(menuEvent);

    for (const auto &command: commands) {
        if (command.getMenuLabel() != nullptr) {
//            cout << "Label: " << command.getMenuLabel() << endl;
            glutAddMenuEntry(command.getMenuLabel(), command.getKey());
        }
    }

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initCommands() {
    commands.emplace_back(GLUT_LEFT_BUTTON, nullptr,
                          "dep. on mode (add vertex to current primitive/move closest vertex\nof current primitive to pointer position",
                          nullptr);
    commands.emplace_back(GLUT_RIGHT_BUTTON, nullptr, "open menu", nullptr);
    commands.emplace_back('h', nullptr, "show help", showHelpInfo);

    commands.emplace_back('a', "Mode: Add vertices", "set mode to add vertex to current primitive", setModeAdd);
    commands.emplace_back('m', "Mode: Move vertices", "set mode to move closest vertex", setModeMove);
    commands.emplace_back(' ', "New primitive", "create new primitive", newPrimitive);
    commands.emplace_back('r', "Change color to red", "set color of current primitive red", changeColorToRed);
    commands.emplace_back('g', "Change color to green", "set color of current primitive green", changeColorToGreen);
    commands.emplace_back('b', "Change color to blue", "set color of current primitive blue", changeColorToBlue);
    commands.emplace_back('p', "Delete last vertex", "delete last vertex", delLastVertex);
    commands.emplace_back('d', "Delete current primitive", "delete current primitive", delLastPrimitive);
    commands.emplace_back('c', "Clear screen", "clearColors screen", clear);
}

void initHelp(const vector<Command> &commands) {
    ostringstream helpStream;

    for (const auto &command: commands) {
        helpStream << command.getHelpText();
    }

    help = helpStream.str();
}

int main(int argc, char **argv) {
    initCommands();

    initHelp(commands);
    cout << "Help:\n" << help << "\n" << endl;

    primitives.emplace_back();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Lab 1");

    initMenu(commands);

    glutDisplayFunc(renderScene);
    glutMouseFunc(mouseEvent);
    glutKeyboardFunc(keyPressedEvent);
    glutKeyboardUpFunc(keyUpEvent);
    glutReshapeFunc(resizeEvent);

    glutMainLoop();

    return 0;
}
