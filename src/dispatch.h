#ifndef GRAPHICS_DISPATCH_H
#define GRAPHICS_DISPATCH_H

#include <iostream>

#include <GL/freeglut.h>
#include "Primitive.h"
#include "Command.h"

extern std::vector<Primitive> primitives;
extern int WIDTH, HEIGHT;
extern int MODE;
extern std::vector<Command> commands;
extern bool SHOW_HELP;

void mouseEvent(int button, int state, int x, int y);

void keyPressedEvent(unsigned char key, int x, int y);

void keyUpEvent(unsigned char key, int x, int y);

void resizeEvent(int width, int height);

void menuEvent(int value);

void addVertex(int x, int y);

void moveVertex(int x, int y);

void newPrimitive();

void clear();

void changeColor(double red, double green, double blue);

void setMode(int mode);

void showHelp(bool show);

void delLastPrimitive();

void delLastVertex();

void setModeAdd();

void setModeMove();

void changeColorToRed();

void changeColorToGreen();

void changeColorToBlue();

void showHelpInfo();

#endif //GRAPHICS_DISPATCH_H
