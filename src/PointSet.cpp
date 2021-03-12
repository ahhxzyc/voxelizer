#include "PointSet.h"


PointSet::PointSet(vector<float> points) : mSize(points.size() / 3) {
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glBindVertexArray(mVao);
    // specify buffer data
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(float) * points.size(),
            points.data(),
            GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

PointSet::~PointSet() {
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
}


void PointSet::bind() {
    glBindVertexArray(mVao);
}

void PointSet::draw(int size) {
    glPointSize(size);
    glDrawArrays(GL_POINTS, 0, mSize);
}