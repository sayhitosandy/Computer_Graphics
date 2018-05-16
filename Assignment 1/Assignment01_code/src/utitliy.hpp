
#ifndef ASSIGNMENT1_UTITLIY_HPP
#define ASSIGNMENT1_UTITLIY_HPP



#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>





struct shDf{
    shDf(GLenum type, const char *src,GLint len);
    GLenum type;
    const char * src;
    const GLint len;
};
std::vector<char> fileToCharArr(const std::string & filename,bool printFile = false);
GLuint compileShader(const shDf & sd);
GLuint makeProgram(GLuint vS, GLuint fS,bool deleteDetachShaders = true);




#endif //ASSIGNMENT1_UTITLIY_HPP
