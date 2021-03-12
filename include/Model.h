#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "Types.h"
#include "Triangle.h"

using namespace std;

class Model {
public:
    Model(const string &filepath) {
        loadModelFromFile(filepath);
    }
    ~Model() = default;

    //   project
    //      +---bin    // here goes exe
    //      +---res    // here goes the models
    void loadModelFromFile(const string &filepath);

public:
    vector<Vec3f>   mVertices;
    vector<Triangle> mTriangles;
};