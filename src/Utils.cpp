#include "Utils.h"
#include <algorithm>
using namespace std;

const float EPS = 1e-10;

bool almostEqual(float a, float b) {
    return fabs(a - b) < EPS;
}


Vec3f maxv(Vec3f a, Vec3f b) {
    for (int i = 0; i < 3; i ++ )
        a[i] = max(a[i], b[i]);
    return a;
}
Vec3f minv(Vec3f a, Vec3f b) {
    for (int i = 0; i < 3; i ++ )
        a[i] = min(a[i], b[i]);
    return a;
}