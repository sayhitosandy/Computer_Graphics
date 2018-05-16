#include <glad/glad.h>
#include <cmath>
#include <vector>
#include <glm/gtc/constants.hpp>

//TODO:create and draw torus here
//refer cube.cpp for example
void createTorusObject(GLuint &torusVAO){
//    GLfloat x[30], y[30], z[30];
//    int k = 0;
//    GLfloat c = 1, a = 0.25;
//    for (int u = 0; u < 360; u+=12) {
//        for (int v = 0; v < 360; v+=12) {
//            x[k] = (GLfloat)((c + a*cos(2*M_PI*v*0.1f/360))*cos(2*M_PI*u*0.1f/360));
//            y[k] = (GLfloat)((c + a*cos(2*M_PI*v*0.1f/360))*sin(2*M_PI*u*0.1f/360));
//            z[k] = (GLfloat)(a*sin(2*M_PI*v*0.1f/360));
//            k++;
//        }
//    }
//
//    GLfloat vertices[30*3];
//    for (int i=0; i<90; i+=3) {
//        int j = i/3;
//        vertices[i] = x[j];
//        vertices[i+1] = y[j];
//        vertices[i+2] = z[j];
//    }
//
//    GLfloat indices[60];
//
//
//    GLuint vertex_VBO;
//    glGenBuffers(1, &vertex_VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
//    glBufferData(GL_ARRAY_BUFFER, 30*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void drawTorus(GLuint &torusVAO,GLuint program){
//    glUseProgram(program);
//    glBindVertexArray(torusVAO);
//    glDrawArrays(GL_TRIANGLES, 0, 30*3);
//    glBindVertexArray(0);

}
