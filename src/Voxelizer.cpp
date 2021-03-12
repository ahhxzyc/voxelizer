#include "Types.h"
#include "Utils.h"
#include "Voxelizer.h"
#include "Model.h"
#include "Triangle.h"

#include <algorithm>
using namespace std;

Voxelizer::Voxelizer(Model *pModel, float resolution) :
                m_pModel(pModel), mResolution(resolution) {
    // specify bounds
    for (const Triangle &tri : pModel->mTriangles) {
        for (int i = 0; i < 3; i ++ ) mMaxVertex = maxv(mMaxVertex, tri.get(i));
        for (int i = 0; i < 3; i ++ ) mMinVertex = minv(mMinVertex, tri.get(i));
    }
    mMaxVertex += Vec3f(mResolution) * 0.5f;
    mMinVertex -= Vec3f(mResolution) * 0.5f;
    // size
    for (int i = 0; i < 3; i ++ ) {
        mSize[i] = mMaxVertex[i] - mMinVertex[i];
        mSizeDiscrete[i] = (int) ceil(mSize[i] / mResolution);
    }
    mVoxels.resize(
        mSizeDiscrete[0] * 
        mSizeDiscrete[1] * 
        mSizeDiscrete[2]
    );
    // voxelize all triangles in the model
    for (Triangle tri : m_pModel->mTriangles) {
        mDir = Z;       voxelize(tri);
        mDir = X;       voxelize(tri);
        mDir = Y;       voxelize(tri);

    }
    cout << "generated " << mCnt << " voxels.\n";
}

void Voxelizer::voxelize(const Triangle &tri) {
    // reorder dimensions
    Vec3f t[3];
    for (int i = 0; i < 3; i ++ )
        t[i] = reorder(tri.get(i));
    // sort according to y
    if (t[0].y < t[1].y) swap(t[0], t[1]);
    if (t[1].y < t[2].y) swap(t[1], t[2]);
    if (t[0].y < t[1].y) swap(t[0], t[1]);
    // split
    float z[3] = { t[0].z, t[1].z, t[2].z };
    Vec2f v[3] = {
        Vec2f(t[0].x, t[0].y),
        Vec2f(t[1].x, t[1].y),
        Vec2f(t[2].x, t[2].y)
    };
    // discretize
    Vec2i w[3] = {
        getDiscrete(v[0]),
        getDiscrete(v[1]),
        getDiscrete(v[2])
    };
    // slope and x, z for the algorithm
    float m02, m01, m12;
    float x0, x1;
    float z0, z1;
    float mz02, mz01, mz12;
    if (w[0].y != w[2].y) {
        m02 = (float) (w[2].x - w[0].x) / (w[2].y - w[0].y);
        mz02 = (float) (z[2] - z[0]) / (w[2].y - w[0].y);
    }
    // upper part
    if (w[0].y == w[1].y) {
        voxelizeScanline(w[0], w[1], z[0], z[1]);
    } else {
        m01 = (float) (w[1].x - w[0].x) / (w[1].y - w[0].y);
        mz01 = (float) (z[1] - z[0]) / (w[1].y - w[0].y);
        x0 = x1 = w[0].x;
        z0 = z1 = z[0];
        for (int y = w[0].y; y >= w[1].y;
                y -- , x0 -= m02, x1 -= m01, z0 -= mz02, z1 -= mz01) {
            voxelizeScanline(
                Vec2i((int)x0, y),
                Vec2i((int)x1, y),
                z0, z1
            );
        }
    }
    // lower part
    if (w[1].y == w[2].y) {
        voxelizeScanline(w[1], w[2], z[1], z[2]);
    } else {
        m12 = (float) (w[1].x - w[2].x) / (w[1].y - w[2].y);
        mz12 = (float) (z[1] - z[2]) / (w[1].y - w[2].y);
        x0 = x1 = w[2].x;
        z0 = z1 = z[2];
        for (int y = w[2].y; y <= w[1].y;
                y ++ , x0 += m02, x1 += m12, z0 += mz02, z1 += mz12) {
            voxelizeScanline(
                Vec2i((int)x0, y),
                Vec2i((int)x1, y),
                z0, z1
            );
        }
    }
}


Vec3f Voxelizer::reorder(const Vec3f &v) {
    Vec3f s(0);
    switch (mDir) {
        case X:     s = Vec3f(v[1], v[2], v[0]); break;
        case Y:     s = Vec3f(v[0], v[2], v[1]); break;
        case Z:     s = v; break;
        default:    break;
    }
    return s;
}

Vec2i Voxelizer::getDiscrete(Vec2f v) {
    float m[2] = {};
    switch (mDir) {
        case X:
            m[0] = mMinVertex[1];
            m[1] = mMinVertex[2];
            break;
        case Y:
            m[0] = mMinVertex[0];
            m[1] = mMinVertex[2];
            break;
        case Z:
            m[0] = mMinVertex[0];
            m[1] = mMinVertex[1];
            break;
        default:
            break;
    }
    return Vec2i((v[0] - m[0]) / mResolution,
            (v[1] - m[1]) / mResolution);
}

int Voxelizer::getIndex(Vec2i xy, float w) {
    int idx = 0;
    int z = 0;
    switch (mDir) {
        case X:
            z = (w - mMinVertex.x) / mResolution;
            idx = z + xy.x * mSizeDiscrete[0] +
                    xy.y * mSizeDiscrete[0] * mSizeDiscrete[1];
            break;
        case Y:
            z = (w - mMinVertex.y) / mResolution;
            idx = xy.x + z * mSizeDiscrete[0] +
                    xy.y * mSizeDiscrete[0] * mSizeDiscrete[1];
            break;
        case Z:
            z = (w - mMinVertex.z) / mResolution;
            idx = xy.x + xy.y * mSizeDiscrete[0] +
                    z * mSizeDiscrete[0] * mSizeDiscrete[1];
            break;
        default:
            break;
    }
    return idx;
}

void Voxelizer::voxelizeScanline(Vec2i a, Vec2i b, float fa, float fb) {
    if (a.x > b.x) {
        swap(a, b);
        swap(fa, fb);
    }
    int s = b.x - a.x;
    float step = s ? (fb - fa) / s : 0;
    float w = fa;
    for (int x = a.x; x <= b.x; x ++ , w += step) {
        int y = b.y;
        int idx = getIndex(Vec2i(x, y), w);
        if (mVoxels[idx] == 0) {
            mVoxels[idx] = 1;
            mCnt  ++ ;
        }
    }
}

vector<float> Voxelizer::getPoints() {
    vector<float> ps;
    for (int i = 0; i < mVoxels.size(); i ++ ) {
        if (mVoxels[i]) {
            int xy = mSizeDiscrete[0] * mSizeDiscrete[1];
            int x = i % xy % mSizeDiscrete[0];
            int y = i % xy / mSizeDiscrete[0];
            int z = i / xy;
            ps.push_back(mMinVertex.x + x * mResolution);
            ps.push_back(mMinVertex.y + y * mResolution);
            ps.push_back(mMinVertex.z + z * mResolution);
        }
    }
    return ps;
}