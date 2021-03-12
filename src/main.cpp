// #define GLEW_STATIC
// #include <GL/glew.h>
#include "GLCommon.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "Model.h"
#include "PointSet.h"
#include "Voxelizer.h"
#include "Shader.h"
using namespace std;

int main(void)
{
    // init GLFW
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "ERROR: failed to init GLEW.\n";
        return -1;
    }

    // load model
    Model model("../res/head.obj");
    Voxelizer vox(&model, 0.05);
    
    vector<float> arr = vox.getPoints();
    for (float &f : arr)
        f /= 2.f;
    PointSet points(arr);

    ShaderProgram program = ShaderProgram::createProgram(
            "../src/shaders/basic.vert",
            "../src/shaders/basic.frag",
            "../src/shaders/basic.geom"
    );

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        Mat modelMat(1);
        modelMat = glm::rotate(
                modelMat,
                sinf(glfwGetTime()),
                Vec3f(0.0, 1, 0.0)
        );
        modelMat = glm::rotate(
                modelMat,
                cosf(glfwGetTime()),
                Vec3f(1, 0, 0.0)
        );
        program.setMat4("model", modelMat);
        program.setFloat("voxResolution", 0.05);
        points.bind();
        points.draw(2);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}