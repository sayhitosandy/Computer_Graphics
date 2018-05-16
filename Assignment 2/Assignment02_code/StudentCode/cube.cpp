#include <glad/glad.h>
#include <vector>
#include <glm/gtc/constants.hpp>

#include "tvp.hpp"

void createCubeObject(GLuint &cubeVAO)
{
    //Cube data: [-10, 10]^3
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
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

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
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
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
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    delete []expanded_colors;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void drawCube(GLuint &cubeVAO, GLuint program)
{
    glUseProgram(program);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    glBindVertexArray(0);
}
