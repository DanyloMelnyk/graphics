#include <iostream>
#include <vector>

#include <GL/freeglut.h>
#include "Primitive.h"
#include "dispatch.h"

static const char *const HELP = "Left mouse - dep. on mode (add vertex to current primitive/move closest vertex\n"
                                "of current primitive to pointer position;\n"
                                "Right mouse - open menu;\n"
                                "Space - start new primitive;\n"
                                "H - show help;\n"
                                "A - set mode to add vertex to current primitive;\n"
                                "M - set mode to move closest vertex;\n"
                                "R - set color of current primitive red;\n"
                                "G - set color of current primitive green;\n"
                                "B - set color of current primitive blue;\n"
                                "C - clear screen;";
using namespace std;

int HEIGHT = 600, WIDTH = 800;
int MODE = 0;
bool SHOW_HELP = false;

vector<Primitive> primitives;

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
        renderString(-0.9, 0.9, GLUT_BITMAP_9_BY_15, HELP);
    }

    for (int i = 0; i < primitives.size(); i++) {
        primitives[i].render(i == primitives.size() - 1);
    }

    glutSwapBuffers();
}

void initMenu() {
    glutCreateMenu(menuEvent);

    glutAddMenuEntry("Mode: Add vertices", 'a');
    glutAddMenuEntry("Mode: Move vertices", 'm');
    glutAddMenuEntry("New primitive", ' ');
    glutAddMenuEntry("Change color to red", 'r');
    glutAddMenuEntry("Change color to green", 'g');
    glutAddMenuEntry("Change color to blue", 'b');
    glutAddMenuEntry("Clear", 'c');
    glutAddMenuEntry("Delete current primitive", 'd');
    glutAddMenuEntry("Delete last vertex", 'p');

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {

    cout << HELP << endl;

    primitives.emplace_back();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // GLUT_DEPTH |
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Lab 1");

    initMenu();

    glutDisplayFunc(renderScene);
    glutMouseFunc(mouseEvent);
    glutKeyboardFunc(keyPressedEvent);
    glutKeyboardUpFunc(keyUpEvent);
    glutReshapeFunc(resizeEvent);

    glutMainLoop();

    return 0;
}
