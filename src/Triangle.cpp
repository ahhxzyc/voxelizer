
#include "Triangle.h"
#include "Model.h"
#include "Utils.h"


Triangle::Triangle(Model *pModel, const array<int, 3> &vertexIdx) : m_pModel(pModel) {
    for (int i = 0; i < 3; i ++ )
        p[i] = m_pModel->mVertices[vertexIdx[i]];
}


// bool Triangle::intersect(Ray ray, Intersection &inter) const {
//     Vec3f e1 = p[1] - p[0];
//     Vec3f e2 = p[2] - p[0];
//     Vec3f d = ray.mDirection;
//     float denominator = mixedProduct(d, e1, e2);
//     if (fabs(denominator) < 1e-10)
//         return false;
//     Vec3f s = ray.mOrigin - p[0];
//     float t = -mixedProduct(s, e1, e2) / denominator;
//     float b0 = mixedProduct(d, s, e2) / denominator;
//     float b1 = mixedProduct(d, e1, s) / denominator;
//     if (t > 0 && inside01(b0) && inside01(b1) && inside01(b0 + b1)) {
//         inter.t = t;
//         inter.mPoint = ray.mOrigin + t * d;
//         inter.mNormal = e1.cross(e2).normalized();
//         return true;
//     }
//     return false;
// }