#include "dispatch.h"


void keyEvent(unsigned char key, int action, int mods) {
    if (mods == SPECIAL_KEY) {

    } else if (mods == GLUT_ACTIVE_CTRL) {
        key += 'a' - 1;
    } else {
        mods |= 64;
    }


    if (action == GLFW_PRESS) {
        cout << "Key pressed: " << Command::keyRepr(key, mods) << " (" << key << ": " << (int) key << ", mods: " << mods << ")" << endl;

        for (auto command: state.commands) {
            if ((int) command.getKey() == key) {
                command.action(mods);
            }
        }
    } else if (action == GLFW_RELEASE) {

    }
}

void mouseEvent(int button, int action, int x, int y) {
    if (action == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            cout << "Mouse click " << x << "; " << y << endl;

            state.leftClick(x, y);
        }
    }
}

void scrollEvent(int button, int dir, int x, int y) {
    cout << "Scroll event " << dir << endl;

    state.scroll(dir);
}

void resizeEvent(int width, int height) {
    glutReshapeWindow(width, width);

    state.WIDTH = width;
    state.HEIGHT = width;

    glViewport(0, 0, width, width);
}