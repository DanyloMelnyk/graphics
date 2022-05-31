//
// Created by danylo on 5/31/22.
//

#include <GL/glew.h>

#include "Material.h"

Material::Material(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular, float shininess) :
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

void Material::setUniform(GLuint shader) {
    glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, &ambient[0]);
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, &diffuse[0]);
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, &specular[0]);

    glUniform1f(glGetUniformLocation(shader, "material.shininess"), shininess);
}
