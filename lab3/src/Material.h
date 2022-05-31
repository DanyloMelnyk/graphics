//
// Created by danylo on 5/31/22.
//

#ifndef GRAPHICS1_MATERIAL_H
#define GRAPHICS1_MATERIAL_H

#include <glm/glm.hpp>
#include <GL/gl.h>

using glm::vec3;

class Material {
public:
    Material(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular, float shininess);

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    void setUniform(GLuint shader);
};

namespace {
    auto example = Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 32.0);
    auto emerald = Material(vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633),
                            0.6 * 128);
}


#endif //GRAPHICS1_MATERIAL_H
