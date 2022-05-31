//
// Created by danylo on 5/17/22.
//

#ifndef GRAPHICS1_STATE_H
#define GRAPHICS1_STATE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Command.h"
#include "Camera.h"
#include "dispatch.h"
#include "Figure.h"
#include "Light.h"

using namespace glm;

struct State {
    int HEIGHT = 600, WIDTH = 600;

    GLuint primitiveShader;
    GLuint lightShader;
    GLuint vectorShader;

    Camera camera;
    Light light;

    vector<Command> commands;

    double prevX, prevY;
    bool isCompleted = true;

    bool showReal = true;

    bool fill = true;

    double scrol = 0.0;

    unsigned int logicOp = GL_COPY;

    string help;

    vector<Figure> figures;

    mat4 perspectiveProjection = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);

    void init() {
        cout << "Start state init" << endl;

        camera.setProjection(perspectiveProjection);

        light.init(lightShader);

        CrossSection section("/home/danylo/CLionProjects/graphics/lab3/СrossSection0.txt");
        CrossSection section1("/home/danylo/CLionProjects/graphics/lab3/СrossSection.txt");
        Trace trace("/home/danylo/CLionProjects/graphics/lab3/Trace0.txt");
        Trace trace1("/home/danylo/CLionProjects/graphics/lab3/Trace1.txt");
        Trace trace2("/home/danylo/CLionProjects/graphics/lab3/Trace3.txt");

        figures.emplace_back(trace, section, primitiveShader, vectorShader, camera, light);
        figures.emplace_back(trace1, section1, primitiveShader, vectorShader, camera, light);
        figures.emplace_back(trace2, section1, primitiveShader, vectorShader, camera, light);
    }

    void render() {
        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        light.render(camera);

        for (auto fig: figures) {
            fig.render();
        }

        cout << "End render" << endl;

        glutSwapBuffers();
    }

    void clear() {
        figures.clear();

        glutPostRedisplay();
    }

    void toggleSmoothing() {
        camera.toggleNormSmoothing();

        glutPostRedisplay();
    }

    void toggleTextures() {
        camera.toggleTextures();

        glutPostRedisplay();
    }

    void moveCamera(vec3 d) {
        camera.changePos(d);

        glutPostRedisplay();
    }

    void moveTarget(vec2 d) {
        camera.moveTarget(vec3(d.x, d.y, 0));

        glutPostRedisplay();
    }

    void toggleNorm() {
        camera.toggleNorm();

        glutPostRedisplay();
    }

    void toggleProjections() {
        camera.perspective = !camera.perspective;

        glutPostRedisplay();
    }
};

#endif //GRAPHICS1_STATE_H
