#pragma once

#include "GLCommon.h"

#include <vector>
using namespace std;

class PointSet {
public:
    PointSet(vector<float> points);
    ~PointSet();

    // bind this point set
    void bind();
    // draw the points in a given size
    void draw(int size);

private:
    // size of the point set
    int mSize;
    // OpenGL stuff
    GLuint mVao;
    GLuint mVbo;
};