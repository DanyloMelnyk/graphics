#ifndef GRAPHICS_DISPATCH_H
#define GRAPHICS_DISPATCH_H

#include <iostream>

#include "Primitive.h"
#include <GL/freeglut.h>
#include "../lib/Command.h"
#include "State.h"

extern int MODE;
extern std::vector<Command> commands;
extern bool SHOW_HELP;

extern State state;

void keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);

void resizeEvent(GLFWwindow *window, int width, int height);

void mouseEvent(GLFWwindow *window, int button, int action, int mods);

void scrollEvent(GLFWwindow *window, double xOffset, double yOffset);

void menuEvent(int value);
//
//void addVertex(int x, int y);
//
//void moveVertex(int x, int y);
//
//void newPrimitive();
//
void clear();
//
//void changeColor(double red, double green, double blue);
//
//void setMode(int mode);
//
//void showHelp(bool show);
//
//void delLastPrimitive();
//
//void delLastVertex();
//
//void setModeAdd();
//
//void setModeMove();
//
//void changeColorToRed();
//
//void changeColorToGreen();
//
//void changeColorToBlue();
//
//void showHelpInfo();


#endif //GRAPHICS_DISPATCH_H
