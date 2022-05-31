//
// Created by danylo on 5/31/22.
//

#ifndef GRAPHICS1_TRACE_H
#define GRAPHICS1_TRACE_H

#include <vector>
#include <glm/glm.hpp>
#include <string>

using namespace std;
using glm::vec4;

class Trace {
public:
    vector<vec4> trace;

    Trace(const string &filename);
};


#endif //GRAPHICS1_TRACE_H
