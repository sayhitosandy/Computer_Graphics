#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utils.h"
#include "gl_utils.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tvp.hpp"


GLFWwindow * setupGlfw(int width, int height, const std::string &title);

//Globals
GLuint program;
GLint vVertex_attrib, vColor_attrib;
//Uniform variabled for Model, View, and Projection matrices to be passed to the vertex shader.
GLint vModel_uniform, vView_uniform, vProjection_uniform;
int screen_width = 800, screen_height = 600;


GLuint cube_VAO; //Vertex array object for cube
GLuint torus_VAO; //Vertex array object for torus

void setupModelTransformation()
{
    //Modelling transformations (Model -> World coordinates)
    glm::mat4 model = getModelMatrix(); //Identity matrix -- do nothing
    //Pass on the modelling matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
}

void setupViewTransformation()
{
    //Viewing transformations (World -> Camera coordinates
    glm::mat4 view = getViewMatrix();


    //Pass on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if(vView_uniform == -1){
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(view));
}


void setupProjectionTransformation()
{
    //Projection transformation (Orthographic projection)
    float aspect = (float)screen_width/(float)screen_height;

    glm::mat4 projection  = getProjectionMatrix(aspect,screen_height,screen_width);

    //Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if(vProjection_uniform == -1){
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}



void init_resources(void)
{
    //Enable certain OpenGL states
    glEnable(GL_DEPTH_TEST); //Enable Z-buffer
    glEnable(GL_MULTISAMPLE); //Draw smoothed polygons

    //Create program
    program = createProgram("./Resources/vshader.vs", "./Resources/fshader.fs");

    //Create cube VAO
    createCubeObject(cube_VAO);
    createTorusObject(torus_VAO);

    //Setup Transformations
    setupModelTransformation();
    setupViewTransformation();
    setupProjectionTransformation();
}

int main(int argc, char* argv[])
{
    using namespace std;



    int screenWidth = 800,screenHeight = 600;
    auto window = setupGlfw(screenWidth,screenHeight, "MainWindow");
    glfwSetKeyCallback(window,[](GLFWwindow *window, int key, int scancode, int action, int mods){
        if(action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
        }
    });


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize OpenGL context" << endl;
        return -1;
    }
    init_resources();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        /* Clear the background as white */
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //TODO: Uncomment line below to display torus, and comment out display of cube.
        drawCube(cube_VAO, program);
        //drawTorus(torus_VAO,program);
        glfwSwapBuffers(window);
    }

    {
        glDeleteProgram(program);
        glfwTerminate();
    }
    return EXIT_SUCCESS;
}


GLFWwindow * setupGlfw(int width, int height, const std::string &title) {
    glfwInit();

    glfwSetTime(0);

    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
    //
    // Create a GLFWwindow object
    GLFWwindow *window = glfwCreateWindow(width,height,title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    return window;
}
