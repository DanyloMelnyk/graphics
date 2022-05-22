#ifndef GRAPHICS_PRIMITIVE_H
#define GRAPHICS_PRIMITIVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

//#include "SOIL/SOIL.h"
#include "Grid.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

struct Octagon {
private:
    vec3 color;

public:

    double x, y;
    double size;

    float angle = 0;

    GLfloat vertices[9 * 5] = {
            // (x, y) POS || (r, g, b) COLOR
            0.0, 0.0, 1.0, 1.0, 1.0,
            0.0, 1.0, 0.8, 0.9, 1.0,
            0.7, 0.7, 0.8, 0.9, 1.0,
            1.0, 0.0, 1.0, 1.0, 1.0,
            0.7, -0.7, 1.0, 1.0, 1.0,
            0.0, -1.0, 1.0, 1.0, 1.0,
            -0.7, -0.7, 1.0, 1.0, 1.0,
            -1.0, 0.0, 1.0, 1.0, 1.0,
            -0.7, 0.7, 1.0, 1.0, 1.0,
    };

    GLuint indices[3 * 8] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            0, 8, 1
    };

    GLuint VBO, VAO, EBO;

    Octagon(double x, double y, double size, vec3 color) : x(x), y(y), size(size), color(color) {

        cout << "New octagon " << x << " " << y << " with size " << size << endl;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0);
        // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    }

    void render(GLuint shader) {

        GLint transformLoc = glGetUniformLocation(shader, "transform");
        mat4 trans = mat4(1.0f);

        trans = rotate(trans, angle, vec3(0, 0, 1.0f));

        trans = scale(trans, vec3(size, size, size));

        trans[3][0] = x;
        trans[3][1] = y;

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(trans));

        glBindVertexArray(VAO);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 3 * 8, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        /*
         * 8 трикутників:
         *
         * t = size * cos(45)
         *
         * центр
         * (x, y) - (x, y + size) - (x + t, y + t)
         *
         *
         * (x, y) - (x + t, y + t) - (x + size, y)
         *
         *
         * (x, y) - (x + size, y) - (x + t, y - t)
         *
         *
         * (x, y) - (x + t, y - t) - (x, y - size)
         *
         *
         * (x, y) - (x, y - size) - (x - t, y - t)
         *
         *
         * (x, y) - (x - t, y - t) - (x - size, y)
         *
         *
         * (x, y) - (x - size, y) - (x - t, y + t)
         *
         *
         * (x, y) - (x - t, y + t) - (x, y + size)
         *
         */
    }


    void printM(mat4 mat4) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cout << mat4[i][j] << " ";
            }

            cout << endl;
        }
    }

    void render(Grid grid, bool fill = true) {
        mat4 trans = mat4(1.0f);

        trans = scale(trans, vec3(size, size, 0.0f));

        trans = rotate(trans, angle, vec3(0, 0, 1));

        trans[3][0] = x;
        trans[3][1] = y;

        // line starts from point[1] (point 0 is center)

        vec4 t = trans * vec4(vertices[8 * 5], vertices[8 * 5 + 1], 0, 1);

        vec2 lastPixel = t;

        double xAcc = 0;
        double yAcc = 0;

        int count = 0;

        for (int i = 1; i <= 8; i++) {
            vec4 pixel = vec4(vertices[5 * i], vertices[5 * i + 1], 0, 1);
            pixel = trans * pixel;

            pair<vec2, vec2> drawn = grid.drawLine(lastPixel.x, lastPixel.y, pixel.x, pixel.y, color);
            lastPixel = pixel;

            vec2 t = drawn.first;
            if (t.x != -1) {
                xAcc += t.x;
                yAcc += t.y;

                count++;
            }

            t = drawn.second;
            if (t.x != -1) {
                xAcc += t.x;
                yAcc += t.y;

                count++;
            }
        }

        cout << "Accumulated " << "(" << xAcc / count << " " << yAcc / count << ")" << endl;
        cout << "Count: " << count << endl;

        if (fill && count > 1) {
            grid.fill(xAcc / count, yAcc / count, color);
        }

//        for (int i = 1; i <= 8; i++) {
//            vec4 pixel = vec4(vertices[5 * i], vertices[5 * i + 1], 0, 1);
//            pixel = trans * pixel;
//
//            grid.drawLine(lastPixel.x, lastPixel.y, pixel.x, pixel.y, vec3(1,1,0));
//            lastPixel = pixel;
//        }

        grid.andBuffer();
    }

    void modifyColor(vec3 d) {
        color = color * d;

        if (color.r > 1.0) {
            color.r = 1.0;
        }

        if (color.r < 0) {
            color.r = 0;
        }
        if (color.g > 1.0) {
            color.g = 1.0;
        }

        if (color.g < 0) {
            color.g = 0;
        }
        if (color.b > 1.0) {
            color.b = 1.0;
        }

        if (color.b < 0) {
            color.b = 0;
        }

        for (int i = 0; i < 9; i++) {
            vertices[i * 5 + 2] = color.r;
            vertices[i * 5 + 3] = color.g;
            vertices[i * 5 + 4] = color.b;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

};

#endif //GRAPHICS_PRIMITIVE_H
