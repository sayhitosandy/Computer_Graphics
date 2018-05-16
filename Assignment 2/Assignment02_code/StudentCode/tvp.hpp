#ifndef ASSIGNMENT02_CODE_TRANSFORMATION_HPP
#define ASSIGNMENT02_CODE_TRANSFORMATION_HPP


#include <glm/glm.hpp>


glm::mat4 getModelMatrix();
glm::mat4 myLookAt(const glm::vec3 eye, const glm::vec3 center, const glm::vec3 up);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix(float aspect, float height, float width);

void createCubeObject(GLuint &cubeVAO);
void drawCube(GLuint &cubeVAO,GLuint program);

void createTorusObject(GLuint &torusVAO);
void drawTorus(GLuint &torusVAO,GLuint program);


#endif //ASSIGNMENT02_CODE_TRANSFORMATION_HPP
