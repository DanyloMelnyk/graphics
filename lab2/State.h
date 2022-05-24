//
// Created by danylo on 5/17/22.
//

#ifndef GRAPHICS1_STATE_H
#define GRAPHICS1_STATE_H

#include <GL/gl.h>
#include "Grid.h"
#include "Octagon.h"
#include <glm/glm.hpp>
#include "Command.h"

using namespace glm;

struct State {
    int HEIGHT = 600, WIDTH = 600;

    GLuint primitiveShader;
    GLuint gridShader;

    Grid grid;

    vector<Octagon> octagons;
    vector<Command> commands;

    double prevX, prevY;
    bool isCompleted = true;

    bool showRasterized = false;
    bool showReal = true;

    bool fill = true;

    double scrol = 0.0;

    unsigned int logicOp = GL_COPY;

    string help;

    void render() {
        if (abs(scrol) > 1.0) {
            resizeGrid(scrol);
            scrol = 0;
        }

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLogicOp(logicOp);

        glEnable(GL_COLOR_LOGIC_OP);

        if (showRasterized) {
            for (auto octagon: octagons) {
                grid.clearColors();
                octagon.render(grid, fill);

                grid.render(gridShader);
            }
        }

//        grid.render(primitiveShader);

        if (showReal) {
            for (auto octagon: octagons) {
                octagon.render(primitiveShader);
            }
        }

        glDisable(GL_COLOR_LOGIC_OP);

        glutSwapBuffers();
    }

    void leftClick(double x, double y) {
//        x = 2.0 * x / WIDTH -1.0;
//        y = 2.0 * x/ HEIGHT - 1.0;

        x = 2 * x / WIDTH - 1; // 2 * (x - 0.5 WIDTH) / W = (2x - W)/W = 2x/W - 1
        y = 1 - 2 * y / HEIGHT; // 2 * (0.5 H - y) / H = (H - 2y) / H = 1 - 2y/H

        if (isCompleted) {
            prevX = x;
            prevY = y;

            isCompleted = false;
        } else {
            double l = sqrt(pow(prevX - x, 2) + pow(prevY - y, 2));
            octagons.emplace_back(prevX, prevY, l, vec3(0.6, 0.7, 0.8));

            isCompleted = true;
        }

        glutPostRedisplay();
    }

    void clear() {
        octagons.clear();

        glutPostRedisplay();
    }

    void moveLast(vec2 d) {
        if (!octagons.empty()) {
            Octagon &last = octagons.back();
            if (last.x + d.x < 1.1 && last.x + d.x > -1.1) {
                last.x += d.x;
            }

            if (last.y + d.y < 1.1 && last.y + d.y > -1.1) {
                last.y += d.y;
            }
        }

        glutPostRedisplay();
    }

    void resizeGrid(int d) {
        cout << "D" << d << endl;
        int oldSize = grid.rows;
        int newSize = oldSize + d;
        cout << "New size: " << newSize << endl;

        this->grid = Grid(newSize);
        glutPostRedisplay();
    }

    void scroll(double d) {
        scrol += 1.5 * d;
        glutPostRedisplay();

    }

    void rotateLast(double d) {
        if (!octagons.empty()) {
            octagons.back().angle += d;

            glutPostRedisplay();
        }
    }

    void changeLastSize(double d) {
        if (!octagons.empty()) {
            octagons.back().size *= d;

            glutPostRedisplay();
        }
    }

    void changeLastColor(vec3 d) {
        if (!octagons.empty()) {
            octagons.back().modifyColor(d);

            glutPostRedisplay();
        }
    }

    void setLogicOp(unsigned int op) {
        if (op == GL_COPY) {
            cout << "No logic op" << endl;
        } else if (op == GL_AND) {
            cout << "Set logic op to AND" << endl;
        } else if (op == GL_NAND) {
            cout << "Set logic op to NAND" << endl;
        } else {
            cout << "Wrong op!" << endl;
            return;
        }

        logicOp = op;
        glutPostRedisplay();
    }

    void setRasterized(bool b) {
        showRasterized = b;
        showReal = !b;

        glutPostRedisplay();
    }

    void setFill(bool b) {
        fill = b;

        glutPostRedisplay();
    }
};

#endif //GRAPHICS1_STATE_H
