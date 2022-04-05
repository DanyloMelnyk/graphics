#ifndef GRAPHICS_PRIMITIVE_H
#define GRAPHICS_PRIMITIVE_H

#include "Vertex.h"
#include <GL/freeglut.h>
#include <vector>
#include <iostream>

extern int WIDTH, HEIGHT;

struct Primitive {
    std::vector<Vertex> vertices;
    double r, g, b;

    Primitive() {
        vertices = std::vector<Vertex>();

        r = 1;
        g = 1;
        b = 1;
    }

    void render(bool current) {
        std::cout << "Start primitive render!" << std::endl;

        if (current) {
            glBegin(GL_POLYGON);
            glColor4d(r * 0.3, g * 0.3, b * 0.3, 0.001);

            for (auto v: vertices) {
//                glVertex2d(v.x + 0.005, v.y + 0.005);
                glVertex2d(v.x, v.y);
            }

            glEnd();
        }

        glBegin(GL_LINE_LOOP);
        glColor3d(r, g, b);
        float lineWidth[2];
        glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);

        std::cout << lineWidth[1] << std::endl;

        for (auto v: vertices) {
            glVertex2d(v.x, v.y);
        }

        glEnd();

        std::cout << "End primitive render!" << std::endl;
    }

    void addVertex(double x, double y) {
        double x_ = 2 * x / WIDTH - 1; // 2 * (x - 0.5 WIDTH) / W = (2x - W)/W = 2x/W - 1
        double y_ = 1 - 2 * y / HEIGHT; // 2 * (0.5 H - y) / H = (H - 2y) / H = 1 - 2y/H

        vertices.emplace_back(x_, y_);
    }

    int closestPointIndex(double x, double y, double maxDist) {
        Vertex pointer = Vertex(2 * x / WIDTH - 1, 1 - 2 * y / HEIGHT);

        double minDist = maxDist;
        int closestVertex = -1;

        for (int i = 0; i < vertices.size(); ++i) {
            if ((vertices[i] - pointer).len() < minDist) {
                minDist = (vertices[i] - pointer).len();

                closestVertex = i;
            }
        }

        return closestVertex;
    }

    void moveVertex(double x, double y) {
        Vertex pointer = Vertex(2 * x / WIDTH - 1, 1 - 2 * y / HEIGHT);

        int closestVertex = closestPointIndex(x, y, 0.5);

        if (closestVertex != -1) {
            std::cout << "Move " << closestVertex << " to " << pointer.x << "; " << pointer.y << "]" << std::endl;
            vertices[closestVertex] = pointer;
        }
    }

    void deleteLastVertex() {
        vertices.pop_back();
    }
};

#endif //GRAPHICS_PRIMITIVE_H
