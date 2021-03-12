#pragma once

// #include <GL/glew.h>
#include "GLCommon.h"
#include "Types.h"
#include <string>
using namespace std;

class Shader {
public:
    enum ShaderType {
        VERTEX = 0,
        FRAGMENT,
        GEOMETRY
    };

public:
    Shader(const string &filepath, ShaderType type);
    ~Shader();

    // load the shader source from file
    string loadFromFile(const string &filepath);
    // compile the shader
    void compile();
    // destroy the shader
    void destroy();
    // get the shader id
    unsigned int id();

private:
    // type of the shader
    ShaderType mType;
    // ID of the shader
    unsigned int mId;
    
};


class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(const ShaderProgram &p) = default;
    ~ShaderProgram() = default;

    // get the program ID
    unsigned int id();
    // use the program
    void use();
    // create a program with given vertex and fragment shaders
    static ShaderProgram createProgram(
            const string &vertexShaderSourcePath,
            const string &fragmentShaderSourcePath,
            const string &geometryShaderSourcePath = string()
    );
    // set uniforms
    void setMat4(const string &name, Mat mat);
    void setFloat(const string &name, float f);

private:
    // ID of the shader program
    unsigned int mId;
};