//
// Created by danylo on 5/31/22.
//

#ifndef GRAPHICS1_CROSSSECTION_H
#define GRAPHICS1_CROSSSECTION_H

#include <vector>
#include <glm/glm.hpp>
#include <string>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class CrossSection {
public:
//    glm::mat4 vertices;
    glm::vec4 vertices[4];

    CrossSection(const string &filename);

    glm::vec4 *getVert(glm::vec3 norm, glm::vec3 pos, double scale);


};


#endif //GRAPHICS1_CROSSSECTION_H
