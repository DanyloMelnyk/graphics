//
// Created by danylo on 5/31/22.
//

#include "CrossSection.h"

#include <iostream>
#include <fstream>
#include <cmath>

CrossSection::CrossSection(const string &filename) {
    ifstream file(filename);

    for (int i = 0; i < 4; ++i) {
        file >> vertices[i].x >> vertices[i].z;
        vertices[i].y = 0;
        vertices[i].w = 1;
    }
}

glm::vec4 *CrossSection::getVert(glm::vec3 norm, glm::vec3 pos, double scale) {
//    norm = glm::normalize(norm);

    auto res = new glm::vec4[4];

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
//    trans = glm::translate(trans, pos);

    trans[3][0] = pos.x;
    trans[3][1] = pos.y;
    trans[3][2] = pos.z;

    auto axis = glm::cross(glm::vec3(0, 1, 0), norm);
    float len = glm::length(axis);
    if (len != 0)
        trans = glm::rotate(trans, asin(len), axis);

    for (int i = 0; i < 4; i++) {
        res[i] = trans * vertices[i];
    }

    return res;
}


