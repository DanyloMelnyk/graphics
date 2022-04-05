#ifndef GRAPHICS_DISPATCH_H
#define GRAPHICS_DISPATCH_H

#include <iostream>

#include <GL/freeglut.h>
#include "Primitive.h"

extern std::vector<Primitive> primitives;
extern int WIDTH, HEIGHT;
extern int MODE;
extern bool SHOW_HELP;

void mouseEvent(int button, int state, int x, int y);

void keyPressedEvent(unsigned char key, int x, int y);

void keyUpEvent(unsigned char key, int x, int y);

void resizeEvent(int width, int height);

void menuEvent(int value);

#endif //GRAPHICS_DISPATCH_H
