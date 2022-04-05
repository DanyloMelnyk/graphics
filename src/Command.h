//
// Created by danylo on 4/5/22.
//

#ifndef GRAPHICS_COMMAND_H
#define GRAPHICS_COMMAND_H

#include <iostream>
#include <sstream>
#include <GL/freeglut.h>

using namespace std;

class Command {
    char key;

    bool showInMenu;
    string menuLabel;

    bool showInHelp;
    string helpText;


    void (*callback)();

public:
    Command(char key, const char *menuDescription, const char *helpDescription, void (*callback)()) : key(key),
                                                                                                      callback(
                                                                                                              callback) {
        if (menuDescription == nullptr) {
            showInMenu = false;
        } else {
            showInMenu = true;

            ostringstream menuLabelStream;

            menuLabelStream << menuDescription << " (";

            if (key == ' ') {
                menuLabelStream << "Space";
            } else if (key == GLUT_LEFT_BUTTON) {
                menuLabelStream << "Left mouse";
            } else if (key == GLUT_RIGHT_BUTTON) {
                menuLabelStream << "Right mouse";
            } else {
                menuLabelStream << (char) toupper(key);
            }

            menuLabelStream << ")";

            menuLabel = menuLabelStream.str();
        }

        if (helpDescription == nullptr) {
            showInHelp = false;
        } else {
            showInHelp = true;
            ostringstream helpTextStream;
            if (key == ' ') {
                helpTextStream << "Space";
            } else if (key == GLUT_LEFT_BUTTON) {
                helpTextStream << "Left mouse";
            } else if (key == GLUT_RIGHT_BUTTON) {
                helpTextStream << "Right mouse";
            } else {
                helpTextStream << (char) toupper(key);
            }

            helpTextStream << " - " << helpDescription << ";" << endl;

            helpText = helpTextStream.str();
        }

    }

    const char *getMenuLabel() const {
        if (showInMenu) {
            return menuLabel.data();
        } else {
            return nullptr;
        }
    }

    const char *getHelpText() const {
        if (showInHelp) {
            return helpText.data();
        } else {
            return nullptr;
        }
    }

    char getKey() const {
        return key;
    }

    void action() {
        if (callback != nullptr) {
            callback();
        }
    }
};

#endif //GRAPHICS_COMMAND_H
