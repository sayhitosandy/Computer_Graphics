#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>




#include "../Src/camera.h"
#include "../Src/renderengine.h"
#include "../Src/world.h"
#include "../Src/material.h"
#include "../Src/object.h"
#include "../Src/sphere.h"
#include "../Src/lightsource.h"
#include "../Src/pointlightsource.h"
#include "../Src/utility.hpp"
#include "../Src/triangle.h"
#include "../Src/cylinder.h"

//Globals
GLFWwindow * mainWindow = nullptr;
GLuint program;
GLuint quadVao,quadVbo;
int screen_width = 800, screen_height = 600;
float quadVertices[] = {-1, -1,
                        1, -1,
                        1, 1,
                        -1, -1,
                        1, 1,
                        -1, 1};
GLuint texImage;
GLint uniform_texImage;

Camera *camera;
RenderEngine *engine;

int init_resources()
{

    auto vert(fileToCharArr("./Resources/vshader.vert"));
    auto frag(fileToCharArr("./Resources/fshader.frag"));



	//Create program
    program = makeProgram(compileShader(shDf(GL_VERTEX_SHADER,&vert[0],vert.size())),compileShader(shDf(GL_FRAGMENT_SHADER,&frag[0],frag.size())),true);



	//Initialize raytracer objects
	Vector3D camera_position(0, 0, 10);
	Vector3D camera_target(0, 0, 0); //Looking down -Z axis
	Vector3D camera_up(0, 1, 0);
	float camera_fovy =  45;
	camera = new Camera(camera_position, camera_target, camera_up, camera_fovy, screen_width, screen_height);
	//Create a world
	World *world = new World;
	world->setAmbient(Color(1));
	world->setBackground(Color(0.1, 0.3, 0.6));
	
	Material *m = new Material(world);
	m->color = Color(0.1, 0.7, 0.0);
	m->ka = 0.15;
    m->kd = 0.5;
//    m->ks = 0.5;
    m->n = 128;
	Object *sphere = new Sphere(Vector3D(2, 0, -10), 2, m);
	world->addObject(sphere);

    Material *m2 = new Material(world);
    m2->ka = 0.15;
    m2->kd = 0.9;
//    m2->ks = 0.5;
    m2->n = 128;
    m2->color = Color(1.0, 0.3, 0.0);
    Object *triangle = new Triangle(Vector3D(-10, -10, -15), Vector3D(10, -10, -20), Vector3D(0, 10, -20), m2);
    world->addObject(triangle);

//    Object *cylinder = new Cylinder(Vector3D(2, 0, -10), 1, m);
//    world->addObject(cylinder);

	LightSource *light = new PointLightSource(world, Vector3D(5, 10, 10), Color(1, 1, 1));
	world->addLight(light);

	engine = new RenderEngine(world, camera);

    glGenVertexArrays(1, &quadVao);





    glGenBuffers(1, &quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    glBindVertexArray(quadVao);
    glBindBuffer(GL_ARRAY_BUFFER,quadVbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2* sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);



    //Initialise texture
	glGenTextures(1, &texImage);
	glBindTexture(GL_TEXTURE_2D, texImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	uniform_texImage = glGetUniformLocation(program, "texImage");
	if(uniform_texImage == -1)
	{
		fprintf(stderr, "Could not bind uniform: texImage\n");
		return 0;
	}
	return 1;
}



int main() {
    using namespace std;



    mainWindow = setupGlfw(screen_width,screen_height,"Lumina");


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize OpenGL context" << endl;
        exit(-1);
    }



	if (1 != init_resources()) {
        exit(-1);
	}

    double last = 0,accumulator = 0;
    glfwSetTime(last);
    double delta = 0.0f;
    glfwSwapInterval(1);

    bool done = false;
    while (!glfwWindowShouldClose(mainWindow)) {


        double curr = glfwGetTime();
        delta = curr-last;
        last = curr;
        glfwPollEvents();



        if(glfwGetKey(mainWindow,GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
        }





        if(done)
            continue;
        if(engine->renderLoop())
        {
            done = true;
            cout << "Rendering complete" << endl;
        }

        //Update texture on GPU
        glBindTexture(GL_TEXTURE_2D, texImage);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screen_width, screen_height, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());



        glClearColor(0.0, 0.0, 0.0, 0.0);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texImage);
        glUniform1i(uniform_texImage, 0);


        glBindVertexArray(quadVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(mainWindow);

    }


    glDeleteVertexArrays(1,&quadVao);
    glDeleteBuffers(1,&quadVbo);

    glDeleteProgram(program);
    glDeleteTextures(1, &texImage);
	return EXIT_SUCCESS;
}
