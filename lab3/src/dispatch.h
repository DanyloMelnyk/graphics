#ifndef GRAPHICS_DISPATCH_H
#define GRAPHICS_DISPATCH_H

#define GLFW_PRESS 1
#define GLFW_RELEASE 2
#define SPECIAL_KEY 0x0010

#include <iostream>

#include "State.h"


#include "Command.h"
#include <GL/freeglut.h>

extern State state;

void keyEvent(unsigned char key, int action, int mods);

void resizeEvent(int width, int height);

void mouseEvent(int button, int action, int x, int y);

void scrollEvent(int button, int dir, int x, int y);

#endif //GRAPHICS_DISPATCH_H
