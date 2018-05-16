/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/
#include <iostream>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>




#include "shader_utils.h"
#include "gl_utils.h"

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;

//Globals
GLuint program;
GLint vVertex_attrib, vColor_attrib, vNormal_attrib;
GLint vModel_uniform, vView_uniform, vProjection_uniform, vLightColor_uniform, vLightPosition_uniform, vViewPos_uniform;
int screen_width = 800, screen_height = 600;
double oldX, oldY, currentX, currentY;
bool isDragging=false;

GLuint cube_VAO, sphere_VAO; //Vertex array object for cube

glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations


GLFWwindow * setupGlfw(int width, int height, const std::string &title);



void setupModelTransformation()
{
	//Modelling transformations (Model -> World coordinates)
	modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

	//Pass on the modelling matrix to the vertex shader
	glUseProgram(program);
	vModel_uniform = glGetUniformLocation(program, "vModel");
	if(vModel_uniform == -1){
		fprintf(stderr, "Could not bind location: vModel\n");
		exit(0);
	}
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}

void setupViewTransformation()
{
	//Viewing transformations (World -> Camera coordinates
	viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//Camera at (0, 0, 20) looking down the negative Z-axis in a right handed coordinate system

	//Pass on the viewing matrix to the vertex shader
	glUseProgram(program);
	vView_uniform = glGetUniformLocation(program, "vView");
	if(vView_uniform == -1){
		fprintf(stderr, "Could not bind location: vView\n");
		exit(0);
	}
	glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));

    vViewPos_uniform = glGetUniformLocation(program, "viewPos");
    if (vViewPos_uniform == -1) {
        fprintf(stderr, "Could not bind location: viewPos");
        exit(0);
    }
    glUniform3f(vViewPos_uniform, 40.0, -40.0, 40.0);
}

void setupProjectionTransformation()
{
	//Projection transformation (Orthographic projection)
	float aspect = (float)screen_width/(float)screen_height;
	float view_height = 100.0f;
	//glm::mat4 projection = glm::ortho(-view_height*aspect/2.0f, view_height*aspect/2.0f, -view_height/2.0f, view_height/2.0f, 0.1f, 1000.0f);
	glm::mat4 projectionT = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

	//Pass on the projection matrix to the vertex shader
	glUseProgram(program);
	vProjection_uniform = glGetUniformLocation(program, "vProjection");
	if(vProjection_uniform == -1){
		fprintf(stderr, "Could not bind location: vProjection\n");
		exit(0);
	}
	glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

void createCubeObject()
{
	glUseProgram(program);

	//Bind shader variables
	vVertex_attrib = glGetAttribLocation(program, "vVertex");
	if(vVertex_attrib == -1) {
		fprintf(stderr, "Could not bind location: vVertex\n");
		exit(0);
	}
	vColor_attrib = glGetAttribLocation(program, "vColor");
	if(vColor_attrib == -1) {
		fprintf(stderr, "Could not bind location: vColor\n");
		exit(0);
	}

	//Cube data
	GLfloat cube_vertices[] = {10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10, //Front
							   10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10}; //Back
	GLushort cube_indices[] = {0, 2, 3, 0, 1, 2, //Front
							   4, 7, 6, 4, 6, 5, //Back
							   5, 2, 1, 5, 6, 2, //Left
							   4, 3, 7, 4, 0, 3, //Right
							   1, 0, 4, 1, 4, 5, //Top
							   2, 7, 3, 2, 6, 7}; //Bottom
	GLfloat cube_colors[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1}; //Unique face colors

	//Generate VAO object
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	//Create VBOs for the VAO
	//Position information (data + format)
	int nVertices = 6*2*3; //(6 faces) * (2 triangles each) * (3 vertices each)
	GLfloat *expanded_vertices = new GLfloat[nVertices*3];
	for(int i=0; i<nVertices; i++) {
		expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
		expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
		expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
	}
	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vVertex_attrib);
	glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	delete []expanded_vertices;

	//Color - one for each face
	GLfloat *expanded_colors = new GLfloat[nVertices*3];
	for(int i=0; i<nVertices; i++) {
		int color_index = i / 6;
		expanded_colors[i*3] = cube_colors[color_index*3];
		expanded_colors[i*3+1] = cube_colors[color_index*3+1];
		expanded_colors[i*3+2] = cube_colors[color_index*3+2];
	}
	GLuint color_VBO;
	glGenBuffers(1, &color_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vColor_attrib);
	glVertexAttribPointer(vColor_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	delete []expanded_colors;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void createSphereObject() {
    glUseProgram(program);

    //Bind shader variables
    vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }
    vColor_attrib = glGetAttribLocation(program, "vColor");
    if(vColor_attrib == -1) {
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }
    vNormal_attrib = glGetAttribLocation(program, "vNormal");
    if (vNormal_attrib == -1) {
        fprintf(stderr, "Could not bind location: vNormal\n");
        exit(0);
    }

    int xs = 100, ys = 100;
    GLfloat radius = 12.0f;

    GLfloat sphere_vertices[xs*ys*3];
    GLuint sphere_indices[xs*ys*6];
    GLint sphere_colors[xs*ys*3];

    int k = 0;
    for (int i=0; i<xs; i++) {
        for (int j=0; j<ys; j++) {
            GLfloat theta, phi;
            phi = ((GLfloat)i/xs) * (glm::pi<float>());
            theta = ((GLfloat)j/ys) * (2*glm::pi<float>());

            GLfloat x, y, z;
            x = radius * cos(theta) * sin(phi);
            y = radius * sin(theta) * sin(phi);
            z = radius * cos(phi);

            sphere_vertices[k++] = x;
            sphere_vertices[k++] = y;
            sphere_vertices[k++] = z;
        }
    }

    int t = 0, g = 0;
    for (int i=0; i<xs; i++) {
        for (int j=0; j<ys; j++) {
            sphere_indices[t] = g;
            sphere_indices[t+1] = (g+xs)%(xs*ys);
            sphere_indices[t+2] = (g+xs+1)%(xs*ys);
            sphere_indices[t+3] = g;
            sphere_indices[t+4] = (g+1)%(xs*ys);
            sphere_indices[t+5] = (g+xs+1)%(xs*ys);
            t+=6;
            g++;
        }
    }

    t = 0;
    for (int i=0; i<xs; i++) {
        for (int j=0; j<ys; j++) {
            sphere_colors[t++] = 0;
            sphere_colors[t++] = 0;
            sphere_colors[t++] = 1;
        }
    }

    //Generate VAO object
    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    //Create VBOs for the VAO
    //Position information (data + format)
    int nVertices = xs*ys*6;
    GLfloat *expanded_vertices = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        expanded_vertices[i*3] = sphere_vertices[sphere_indices[i]*3];
        expanded_vertices[i*3 + 1] = sphere_vertices[sphere_indices[i]*3+1];
        expanded_vertices[i*3 + 2] = sphere_vertices[sphere_indices[i]*3+2];
    }
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vNormal_attrib);
    glVertexAttribPointer(vNormal_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    delete []expanded_vertices;

    //Color - one for each face
    GLfloat *expanded_colors = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        int color_index = i / 6;
        expanded_colors[i*3] = sphere_colors[color_index*3];
        expanded_colors[i*3+1] = sphere_colors[color_index*3+1];
        expanded_colors[i*3+2] = sphere_colors[color_index*3+2];
    }
    GLuint color_VBO;
    glGenBuffers(1, &color_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vColor_attrib);
    glVertexAttribPointer(vColor_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_colors;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.

}


int init_resources()
{
	//Enable certain OpenGL states
	glEnable(GL_DEPTH_TEST); //Enable Z-buffer
	glEnable(GL_MULTISAMPLE); //Draw smoothed polygons

	//Create program
	program = createProgram("./Resources/vshader.vs", "./Resources/fshader.fs");

	//Create cube VAO
    //createCubeObject();
    createSphereObject();

	//Setup Transformations
	setupModelTransformation();
	setupViewTransformation();
	setupProjectionTransformation();

    glUseProgram(program);
    vLightPosition_uniform = glGetUniformLocation(program, "vLightPosition");
    if(vLightPosition_uniform == -1){
        fprintf(stderr, "Could not bind location: vLightPosition\n");
        exit(0);
    }
    glUniform3f(vLightPosition_uniform, 11.0f, 11.0f, 11.0f);

    glUseProgram(program);
    vLightColor_uniform = glGetUniformLocation(program, "vLightColor");
    if(vLightColor_uniform == -1){
        fprintf(stderr, "Could not bind location: vLightColor\n");
        exit(0);
    }
    glUniform3f(vLightColor_uniform, 1.0f, 1.0f, 1.0f);

	return 1;
}





glm::vec3 getTrackBallVector(double x, double y)
{


	glm::vec3 p = glm::vec3(2.0*x/screen_width - 1.0, 2.0*y/screen_height - 1.0, 0.0); //Normalize to [-1, +1]
	p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

	float mag2 = p.x*p.x + p.y*p.y;
	if(mag2 <= 1.0f)
		p.z = sqrtf(1.0f - mag2);
	else
		p = glm::normalize(p); //Nearest point, close to the sides of the trackball
	return p;
}




int main(int argc, char* argv[]) {


	auto window = setupGlfw(screen_width,screen_height, "Assignment3");
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


    oldX = oldY = currentX = currentY = 0.0;

	if (1 == init_resources())
	{





        glm::mat4 hold = viewT;



        int prevLeftButtonState = GLFW_RELEASE;

		while (!glfwWindowShouldClose(window)) {

			glfwPollEvents();



            int leftButtonState = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT);
            double x,y;
            glfwGetCursorPos(window,&x,&y);
            if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_RELEASE){
                isDragging = true;

                currentX = oldX = x;
                currentY = oldY = y;

            }
            else if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_PRESS){
                    currentX = x;
                    currentY = y;

            }
            else if(leftButtonState == GLFW_RELEASE && prevLeftButtonState == GLFW_PRESS){
                isDragging = false;
            }


            prevLeftButtonState = leftButtonState;
            if(isDragging && (currentX !=oldX || currentY != oldY))
            {
                glm::vec3 va = getTrackBallVector(oldX, oldY);
                glm::vec3 vb = getTrackBallVector(currentX, currentY);

                float angle = acos(std::min(1.0f, glm::dot(va,vb)));
                glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
                glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
                glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
                modelT = glm::rotate(modelT, angle, axis_in_object_coord);
                glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
//                glUniform3fv(vViewPos_uniform, 1, glm::value_ptr(camera2object*glm::vec4(0,0,1,1)));

                oldX = currentX;
                oldY = currentY;
            }


            /* Clear the background as white */
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(program);
			glBindVertexArray(sphere_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 100*100*6);
			glBindVertexArray(0);

			glfwSwapBuffers(window);
		}

        glDeleteProgram(program);
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
        glfwWindowHint(GLFW_SAMPLES,4);
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

