#pragma once

#include "Types.h"
// #include "Intersection.h"
// #include "Ray.h"
#include <array>
using namespace std;

class Model;

class Triangle {
public:
    Triangle(Model *pModel, const array<int, 3> &vertexIdx);

    // intersection test with a ray
    // bool intersect(Ray ray, Intersection &inter) const;

    // return a vertex
    Vec3f get(int i) const {
        return p[i];
    }

public:
    // pointer to the model
    Model *m_pModel;
    // vertices of the triangle
    Vec3f p[3];
};