#pragma once

#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>


class Transform {

public:
    QVector3D translation;
    QQuaternion rotation;

public:
    Transform(): translation(), rotation() {}

    void translate(QVector3D v) {translation += v;}
    void rotate(QQuaternion q) {rotation = q * rotation;}
    QMatrix4x4 getMatrix() {
        QMatrix4x4 mat;
        mat.setToIdentity();
        mat.translate(translation);
        mat.rotate(rotation);
        return mat;
    }
};