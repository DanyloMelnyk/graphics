//
// Created by danylo on 4/5/22.
//

#ifndef GRAPHICS_COMMAND_H
#define GRAPHICS_COMMAND_H

#include <iostream>
#include <sstream>
#include <GL/freeglut.h>
#include "../lab2/dispatch.h"

using namespace std;

class Command {
    int key;

    bool showInMenu;
    string menuLabel;

    bool showInHelp;
    string helpText;
    int mod;

    void (*callback)() = nullptr;

public:
    Command(int key, const char *menuDescription, const char *helpDescription, void (*callback)(), int mod) :
            callback(callback), mod(mod), key(key) {
        setMenuLabel(key, mod, menuDescription);

        setHelpDescription(key, mod, helpDescription);
    }

    Command(int key, const char *menuDescription, const char *helpDescription, void (*callback)()) :
            callback(callback), mod(64), key(key) {
        setMenuLabel(key, mod, menuDescription);

        setHelpDescription(key, mod, helpDescription);
    }

    static string keyRepr(int key, int mod) {
        if (mod == SPECIAL_KEY) {
            if (key == GLUT_LEFT_BUTTON) {
                return "Left mouse";
            } else if (key == GLUT_RIGHT_BUTTON) {
                return "Right mouse";
            } else if (key == GLUT_KEY_UP) {
                return "Arrow up";
            } else if (key == GLUT_KEY_DOWN) {
                return "Arrow down";
            } else if (key == GLUT_KEY_RIGHT) {
                return "Arrow right";
            } else if (key == GLUT_KEY_LEFT) {
                return "Arrow left";
            } else {
                return "<unknown special key>";
            }
        }

        if (mod == GLUT_ACTIVE_CTRL) {
            return "CTRL + " + keyRepr(key, 0);
        }

        if (key == ' ') {
            return "Space";
        } else {
            return string(1, (char)(toupper(key)));
        }
    }

    void setMenuLabel(int key, int mod, const char *menuDescription) {
        if (menuDescription == nullptr) {
            showInMenu = false;
        } else {
            showInMenu = true;

            ostringstream menuLabelStream;

            menuLabelStream << menuDescription << " (";

            menuLabelStream << keyRepr(key, mod);

            menuLabelStream << ")";

            menuLabel = menuLabelStream.str();
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
            return "";
        }
    }

    void setHelpDescription(int key, int mod, const char *helpDescription) {
        if (helpDescription == nullptr) {
            showInHelp = false;
        } else {
            showInHelp = true;
            ostringstream helpTextStream;

            helpTextStream << keyRepr(key, mod) << " - " << helpDescription << ";" << endl;

            helpText = helpTextStream.str();
        }
    }

    int getKey() const {
        return key;
    }

    void action(int mods) {

        if (mods & mod) {
            cout << "Click: " << helpText << " mods: " << mods << endl;

            callback();
        }
    }

    static string initHelp(const vector<Command> &commands) {
        ostringstream helpStream;

        for (const auto &command: commands) {
            helpStream << command.getHelpText();
        }

        return helpStream.str();
    }
};

#endif //GRAPHICS_COMMAND_H
