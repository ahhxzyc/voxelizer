#pragma once

#include "Types.h"
#include "Utils.h"

#include <cmath>
using namespace std;

class Trackball {
private:
    float radius = 1.f;
    const float PI = acosf(-1.f);
    Vec2f prev;
    Vec2f cur;

public:
    Quat getRotation() {
        // Return 0 retation if prev == cur
        if (almostEqual(prev[0], cur[0]) &&
                almostEqual(prev[1], cur[1])) {
            return Quat();
        }
        // Find the angle between two vectors using dot product
        Vec3f prev3 = toVectorOnBall(prev);
        Vec3f cur3 = toVectorOnBall(cur);
        float angle = acosf(dot(prev3, cur3) / (prev3.length() * cur3.length())) * 180.f / PI;
        // Return the quaternion representing the rotation
        return Quat::fromAxisAndAngle(cross(prev3, cur3), angle);
        return Quat()
    }
    Quat getReverseRotation() {
        Quat q = getRotation();
        q.setVector(-q.vector());
        return q;
    }
    Vec3f toVectorOnBall(QVector2D v) {
        Vec3f retVector;
        float arc = v.length();
        float a = arc / radius;     // angle between retVector and vertical direction(the direction that points outward through the screen)
        float b = atan2f(v[1], v[0]);
        retVector[0] = radius * sinf(a) * cosf(b);
        retVector[1] = radius * sinf(a) * sinf(b);
        retVector[2] = radius * cosf(a);
        return retVector;
    }
    void update(QVector2D pos) {
        cur = pos;
    }
    void set() {
        prev = cur;
    }
    
};