//
// Created by danylo on 5/8/22.
//

#ifndef GRAPHICS1_GRID_H
#define GRAPHICS1_GRID_H

#include <GL/glew.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>

using namespace std;
using glm::vec3;
using glm::ivec2;
using glm::vec2;
using glm::ivec3;

struct Grid {


    int rows, cols;

    vec3 *colors;
    vec3 *colorsBuffer;

    // (x, y) (r, g, b)
    GLfloat *vertices;

    GLuint *indices;

    GLuint VBO, VAO, EBO;
    int pixels;

    bool dirty = true;

    bool debug = false;

    Grid() {}

    Grid(int rows) : rows(rows), cols(rows) {
        pixels = rows * cols;

        cout << "Grid " << rows << "x" << cols << endl;


        // 1 піксель = 2 трикутники = 4 вершини
        vertices = new GLfloat[5 * 4 * pixels];
        cout << "0" << endl;

        setPixelCoords();

        cout << "1" << endl;

        colors = new vec3[pixels];
        colorsBuffer = new vec3[pixels];
        clearColors();

        cout << "2" << endl;

        indices = new GLuint[3 * 2 * pixels];
        setIndices();

        cout << "3" << endl;

        bindBuffers();

        cout << "Init Error: " << glGetError() << endl;

    }

    void setIndices() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                int offset = (row * cols + col) * 3 * 2;

                // 0 1
                // 2 3

                // 0 - 1 - 3
                // 0 - 2 - 3

                int v_0 = (row * cols + col) * 4;
                indices[offset] = v_0;
                indices[offset + 1] = v_0 + 1;
                indices[offset + 2] = v_0 + 3;

                indices[offset + 3] = v_0;
                indices[offset + 4] = v_0 + 2;
                indices[offset + 5] = v_0 + 3;
            }
        }
    }


    void bindBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 5 * 4 * pixels * sizeof(float), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * pixels * sizeof(float), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0);
        // Unbind VAO
        // (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    }

    void render(int shader) {
        if (dirty) {
            if (debug)
                cout << "Dirty!" << endl;

            update();

//            bindBuffers();
            dirty = false;
        }

        glUseProgram(shader);

        GLenum error = glGetError();
        if (error != 0)
            cout << "glUseProgram Error: " << error << endl;

        GLint transformLoc = glGetUniformLocation(shader, "transform");
        glm::mat4 trans = glm::mat4(1.0f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, pixels * 3 * 2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        error = glGetError();
        if (error != 0) {
            cout << "Error: " << error << endl;
            throw 3;
        }
    }

    void set(double x, double y, vec3 color) {
        // x in [-1; 1]
        // y in [-1; 1]
        setI(round((x + 1.0) / 2.0 * rows), round((y + 1.0) / 2.0 * cols), color);
    }

    bool setI(int x, int y, vec3 color) {
        if (x < 0 || x >= cols) {
//            cout << "Bad x!" << endl;
//            cout << "(" << x << "; " << y << ")" << endl;
            return false;
        }
        if (y < 0 || y >= rows) {
//            cout << "Bad y!" << endl;

//            cout << "(" << x << "; " << y << ")" << endl;

            return false;
        }

        dirty = true;

        colorsBuffer[y * cols + x] = color;

        return true;
    }

    void clearColors() {
//        vec3 dark(0, 0, 0);
//        vec3 light(0.2, 0.2, 0.2);
//        for (int row = 0; row < rows; row++) {
//            for (int col = 0; col < cols; col++) {
//                if ((col + row) % 2 == 0) {
//                    colors[cols * row + col] = dark;
//                    colorsBuffer[cols * row + col] = dark;
//                } else {
//                    colors[cols * row + col] = light;
//                    colorsBuffer[cols * row + col] = light;
//                }
//            }
//        }



        vec3 white(1, 1, 1);
        vec3 dark(0.2, 0.2, 0.2);
        vec3 black(0, 0, 0);
        clearColors(colors, black, white);
        clearColors(colorsBuffer, black, black);
    }

    void clearColors(vec3 *buffer, vec3 c1, vec3 c2) {
//        vec3 dark(0, 0, 0);
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if ((col + row) % 2 != 0) {
                    buffer[cols * row + col] = c1;
                } else {
                    buffer[cols * row + col] = c2;
                }
            }
        }

        dirty = true;
    }

    void setPixelCoords() {
        GLfloat *offset = vertices;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                double x = (2.0 * col / cols) - 1.0;
                double y = (2.0 * row / rows) - 1.0; // pixel upper left coord

                for (int vert = 0; vert < 4; vert++) {
                    // 0 1
                    // 2 3

                    offset[0] = (vert % 2 == 0) ? x : x + 2.0 / cols; // X
                    offset[1] = (vert / 2 == 0) ? y : y + 2.0 / cols; // Y

                    offset += 5;
                }
            }
        }
    }

    void update() {
        GLfloat *offset = vertices;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                vec3 c = colors[cols * row + col];

                for (int vert = 0; vert < 4; vert++) {
                    offset[2] = c.r; // R
                    offset[3] = c.g; // G
                    offset[4] = c.b; // B

                    offset += 5;
                }
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 5 * 4 * pixels * sizeof(float), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        dirty = false;
    }

    pair<ivec2, ivec2> drawLine1(int x1, int y1, int x2, int y2, vec3 color) {
        const int deltaX = abs(x2 - x1);
        const int deltaY = abs(y2 - y1);
        const int signX = x1 < x2 ? 1 : -1;
        const int signY = y1 < y2 ? 1 : -1;
        int error = deltaX - deltaY;

        ivec2 firstPixel = ivec2(-1, -1);
        ivec2 lastPixel = ivec2(-1, -1);

        bool isFirstPixelSet = false;

        if (setI(x2, y2, color)) {
            firstPixel = ivec2(x2, y2);
            lastPixel = firstPixel;

            isFirstPixelSet = true;
        }

        while (x1 != x2 || y1 != y2) {
//            setI(x1, y1, color);
            if (setI(x1, y1, color)) {
                lastPixel = ivec2(x1, y1);

                if (!isFirstPixelSet) {
                    firstPixel = lastPixel;

                    isFirstPixelSet = true;
                }
            }

            int error2 = error * 2;
            if (error2 > -deltaY) {
                error -= deltaY;
                x1 += signX;
            }
            if (error2 < deltaX) {
                error += deltaX;
                y1 += signY;
            }
        }

        return make_pair(firstPixel, lastPixel);

    }

    pair<vec2, vec2> drawLine(double xa, double ya, double xb, double yb, vec3 color) {
//        cout << "Line: (" << xa << "; " << ya << ") - (" << xb << "; " << yb << ")" << endl;

        xa = (xa + 1.0) / 2.0 * rows;
        xb = (xb + 1.0) / 2.0 * rows;

        ya = (ya + 1.0) / 2.0 * cols;
        yb = (yb + 1.0) / 2.0 * cols;

        dirty = true;

        pair<ivec2, ivec2> drawnPixels = drawLine1(round(xa), round(ya), round(xb), round(yb), color);

//        vec2 firstPixel = vec2(2.0 * drawnPixels.first.x / rows - 1, 2.0 * drawnPixels.first.y / cols - 1);
        vec2 firstPixel = vec2(drawnPixels.first.x, drawnPixels.first.y);
//        vec2 lastPixel = vec2(2.0 * drawnPixels.second.x / rows - 1, 2.0 * drawnPixels.second.y / cols - 1);
        vec2 lastPixel = vec2(drawnPixels.second.x, drawnPixels.second.y);

        if (drawnPixels.first.x == -1)
            firstPixel = vec2(-1, -1);

        if (drawnPixels.second.x == -1)
            lastPixel = vec2(-1, -1);

        return make_pair(firstPixel, lastPixel);
    }

    void fill(int x, int y, vec3 color) {
        bool *visited = new bool[rows * cols];
        for (int i = 0; i < rows * cols; i++) {
            visited[i] = colorsBuffer[i] == color;
        }

        stack<ivec2> p;

        int xI = x;//(x + 1.0) / 2.0 * cols;
        int yI = y;//(y + 1.0) / 2.0 * rows;

        if (xI >= cols) {
            xI = cols - 1;
        }

        if (xI < 0) {
            xI = 0;
        }

        if (yI >= rows) {
            yI = rows - 1;
        }

        if (yI < 0) {
            yI = 0;
        }

        p.emplace(xI, yI);
        auto t = p.top();

        cout << "\n\nTOP (" << p.top().x << " " << p.top().y << ")" << endl;

        while (!p.empty()) {
            ivec2 top = p.top();
            p.pop();

            if (top.x < 0 || top.y < 0 || top.x > cols - 1 || top.y > rows - 1) {
                cout << "Bad pixel! Out of frame" << endl;
                continue;
            }


            colorsBuffer[top.y * cols + top.x] = color;
            visited[top.y * cols + top.x] = true;


            if (top.y < rows - 1 && !visited[(top.y + 1) * cols + top.x]) {
                p.emplace(top.x, top.y + 1);
            }

            if (top.y > 0 && !visited[(top.y - 1) * cols + top.x]) {
                p.emplace(top.x, top.y - 1);
            }

            if (top.x < cols - 1 && !visited[top.y * cols + top.x + 1]) {
                p.emplace(top.x + 1, top.y);
            }

            if (top.x > 0 && !visited[top.y * cols + top.x - 1]) {
                p.emplace(top.x - 1, top.y);
            }
        }

        dirty = true;

//        colors[t.y * cols + t.x] = vec3(1, 0, 0);

        delete[] visited;
    }

    vec3 andVec(vec3 a, vec3 b) {
        if (a.r + a.g + a.b != 0 && b.r + b.g + b.b != 0) {
            return b;
        }

        return vec3(0.0, 0.0, 0.0);
    }

    vec3 nandVec(vec3 a, vec3 b) {
        if (!(a.r + a.g + a.b != 0 && b.r + b.g + b.b != 0)) {
            return b;
        }

        return vec3(0.0, 0.0, 0.0);
    }

    void andBuffer() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                colors[cols * row + col] = nandVec(colors[cols * row + col], colorsBuffer[cols * row + col]);
            }
        }


        vec3 white(1, 1, 1);
        vec3 dark(0.2, 0.2, 0.2);
        vec3 black(0, 0, 0);

        clearColors(colorsBuffer, black, black);
    }

};

#endif //GRAPHICS1_GRID_H
