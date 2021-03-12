#include "Shader.h"

#include <fstream>
#include <iostream>
using namespace std;

Shader::Shader(const string &filepath, ShaderType type) : mType(type) {
    string s = loadFromFile(filepath);
    const char *cs = s.c_str();
    GLenum shaderTypeGL = 0;
    switch (type) {
        case VERTEX:    shaderTypeGL = GL_VERTEX_SHADER; break;
        case FRAGMENT:  shaderTypeGL = GL_FRAGMENT_SHADER; break;
        case GEOMETRY:  shaderTypeGL = GL_GEOMETRY_SHADER; break;
        default:        break;
    }
    mId = glCreateShader(shaderTypeGL);
    glShaderSource(mId, 1, &cs, 0);
}

Shader::~Shader() {
    // delete the shader
    destroy();
}


string Shader::loadFromFile(const string &filepath) {
    ifstream stream(filepath);
    if (!stream.is_open()) {
        cout << "ERROR: failed to find file " << filepath << " .\n";
        return string();
    }
    return string((istreambuf_iterator<char>(stream)),
            (istreambuf_iterator<char> ()));
}


void Shader::compile() {
    glCompileShader(mId);
    // grab compile info
    int success = 0;
    char log[200];
    glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
    if (!success) {
        cout << "ERROR: failed to compile shader.\n";
        glGetShaderInfoLog(mId, sizeof(log), 0, log);
        cout << log << endl;
    }
}


void Shader::destroy() {
    glDeleteShader(mId);
}

unsigned int Shader::id() {
    return mId;
}

ShaderProgram::ShaderProgram() {
    mId = glCreateProgram();
}

ShaderProgram ShaderProgram::createProgram(
        const string &vertexShaderSourcePath,
        const string &fragmentShaderSourcePath,
        const string &geometryShaderSourcePath
) {
    // geometry shader is optional
    bool hasgeom = !geometryShaderSourcePath.empty();
    // create shaders
    Shader vert(vertexShaderSourcePath, Shader::VERTEX);
    Shader frag(fragmentShaderSourcePath, Shader::FRAGMENT);
    Shader geom(geometryShaderSourcePath, Shader::GEOMETRY);
    vert.compile();
    frag.compile();
    geom.compile();
    ShaderProgram p;
    glAttachShader(p.id(), vert.id());
    glAttachShader(p.id(), frag.id());
    glAttachShader(p.id(), geom.id());
    glLinkProgram(p.id());

    // GLuint shaders[5];
    // GLsizei cnt;
    // glGetAttachedShaders(p.id(), 5, &cnt, shaders);
    // char source[500];
    // glGetShaderSource(shaders[0], 500, 0, source);

    // check for link error
    int success = 0;
    char log[512];
    glGetProgramiv(p.id(), GL_LINK_STATUS, &success);
    if (!success) {
        cout << "ERROR: failed to link program.\n";
        glGetProgramInfoLog(p.id(), sizeof(log), 0, log);
        cout << log << endl;
    }
    return p;
}

unsigned int ShaderProgram::id() {
    return mId;
}

void ShaderProgram::use() {
    glUseProgram(mId);
}


void ShaderProgram::setMat4(const string &name, Mat mat) {
    GLuint loc = glGetUniformLocation(mId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}


void ShaderProgram::setFloat(const string &name, float f) {
    GLuint loc = glGetUniformLocation(mId, name.c_str());
    glUniform1f(loc, f);
}