#pragma once

#include "Types.h"

// see if two float's are almost equal
bool almostEqual(float a, float b);

// vector-wise max & min
Vec3f maxv(Vec3f a, Vec3f b);
Vec3f minv(Vec3f a, Vec3f b);