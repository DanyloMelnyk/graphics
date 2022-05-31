//
// Created by danylo on 5/31/22.
//

#include "Trace.h"

#include <iostream>
#include <fstream>

Trace::Trace(const string &filename) {
    ifstream file(filename);

    int n;
    file >> n;
    trace.resize(n);

    for (int i = 0; i < n; ++i) {
        file >> trace[i].x >> trace[i].y >> trace[i].z >> trace[i].w;
    }
}
