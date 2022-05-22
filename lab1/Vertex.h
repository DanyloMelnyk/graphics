#ifndef GRAPHICS_VERTEX_H
#define GRAPHICS_VERTEX_H

#include <cmath>

struct Vertex {
    double x, y;

    Vertex(double x, double y) {
        this->x = x;
        this->y = y;
    }

    Vertex operator-(const Vertex &other) const {
        return {x - other.x, y - other.y};
    }

    double len() const {
        return sqrt(std::pow(x, 2) + std::pow(y, 2));
    }
};

#endif //GRAPHICS_VERTEX_H
