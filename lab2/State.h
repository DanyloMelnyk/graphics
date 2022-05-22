//
// Created by danylo on 5/17/22.
//

#ifndef GRAPHICS1_STATE_H
#define GRAPHICS1_STATE_H

#include <GL/gl.h>
#include "Grid.h"
#include "Primitive.h"
#include <glm/glm.hpp>

using namespace glm;

struct State {
    int HEIGHT = 600, WIDTH = 600;

    GLuint shader;

    Grid grid;

    vector<Octagon> octagons;
    GLFWwindow *window;

    double prevX, prevY;
    bool isCompleted = true;

    bool showRasterized = false;
    bool showReal = true;

    double scrol = 0.0;

    void render() {
        if (abs(scrol) > 1.0) {
            resizeGrid(scrol);
            scrol = 0;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        grid.clearColors();
        if (showRasterized) {
            for (auto octagon: octagons) {
                octagon.render(grid);
            }
        }

        grid.render(shader);


        if (showReal) {
            for (auto octagon: octagons) {
                octagon.render(shader);
            }
        }

        glfwSwapBuffers(window);
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
//            l=0.5;
            octagons.emplace_back(prevX, prevY, l, vec3(0.6, 0.7, 0.8));

            isCompleted = true;
        }
    }

    void clear() {
        octagons.clear();
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
    }

    void resizeGrid(int d) {
        cout << "D" << d << endl;
        int oldSize = grid.rows;
        int newSize = oldSize + d;
        cout << "New size: " << newSize << endl;

        this->grid = Grid(newSize);
    }

    void scroll(double d) {
        scrol += d / 2;
    }

    void rotateLast(double d) {
        if (!octagons.empty()) {
            octagons.back().angle += d;
        }
    }

    void changeLastSize(double d) {
        if (!octagons.empty()) {
            octagons.back().size *= d;
        }
    }

    void changeLastColor(vec3 d) {
        if (!octagons.empty()) {
            octagons.back().modifyColor(d);
        }
    }
};

#endif //GRAPHICS1_STATE_H
