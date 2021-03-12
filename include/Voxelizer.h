#pragma once

#include <vector>
#include <limits>
using namespace std;

class Model;
class Triangle;

struct Vec2i {
    int x, y;
    Vec2i(int a, int b) : x(a), y(b) {}
};

class Voxelizer {
private:
    enum ProjectionDir {
        X = 0,
        Y,
        Z
    };
public:
    Voxelizer(Model *pModel, float resolution);
    ~Voxelizer() = default;

    // voxelize a triangle
    void voxelize(const Triangle &tri);
    // voxelize a scanline
    void voxelizeScanline(Vec2i a, Vec2i b, float fa, float fb);

    // reorder dimensions of vector for the projection algorithm
    Vec3f reorder(const Vec3f &v);
    // convert to discrete coordinates
    Vec2i getDiscrete(Vec2f v);
    // convert to real coordinates
    int getIndex(Vec2i xy, float z);
    // get points at the center of voxels
    vector<float> getPoints();


private:
    // num of voxels generated
    int mCnt = 0;
    // the model to be voxelized
    Model *m_pModel;
    // 1 where there is a voxel, 0 otherwise
    vector<int> mVoxels;
    // 2 vertices specify the area to be voxelized
    Vec3f mMinVertex = Vec3f(numeric_limits<float>::max());
    Vec3f mMaxVertex = Vec3f(numeric_limits<float>::min());
    // size of a single voxel
    float mResolution;
    // size of the voxelization boundary
    float mSize[3];
    int mSizeDiscrete[3];
    // current projection direction for the algorithm
    ProjectionDir mDir;
};