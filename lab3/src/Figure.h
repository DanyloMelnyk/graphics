//
// Created by danylo on 5/31/22.
//

#ifndef GRAPHICS1_FIGURE_H
#define GRAPHICS1_FIGURE_H

#define DIR "/home/danylo/CLionProjects/graphics/lab3"

#include <GL/glew.h>
#include <GL/gl.h>

#include "CrossSection.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Trace.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL/SOIL.h>

#include <iostream>

using namespace std;

class Figure {
    // (x, y, z) (normX, normY, normZ) (textureX, textureY)

    glm::mat4 modelTransform;
    glm::mat3 normTransform;

    GLfloat *vertices;//[3 * 2 * 6 * 8];FIXME
    // (x, y, z)
    GLfloat *normVert;

    GLuint indices[3 * 2 * 6] = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            1, 5, 4,
            1, 4, 0,
            3, 7, 6,
            3, 6, 2,
            0, 4, 7,
            0, 7, 3,
            2, 6, 5,
            2, 5, 1
    };

    GLuint shader, vectorShader;
    GLuint VBO, VAO;
    GLuint VBO_VECTORS, VAO_VECTORS;
    GLuint texture;

    Camera &camera;
    Light &light;
    Material material = example;
    unsigned long verticesCount;
    unsigned long normsVerticesCount;
    bool normSmoothing;

    void loadBuffers();

    void loadTextures();

    Trace trace;
    CrossSection crossSection;

public:
    void setModelTransform(const mat4 &modelTransform);

    Figure(Trace &trace, CrossSection &crossSection, GLuint shader, GLuint vectorShader, Camera &camera,
           Light &light);

    void render();

    vector<vec3> calcVertPos();

    void prepareVerArray(unsigned long traceLen, const vector<vec3> &vertPos) const;

};


#endif //GRAPHICS1_FIGURE_H
