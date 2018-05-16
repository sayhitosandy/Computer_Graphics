
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//TODO implement these functions for part 1
glm::mat4 rotateAroundZ(){
    glm::mat4 m;
    m = glm::rotate(m,glm::radians(15.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    return m;

}
glm::mat4 scaleAlongY(){
    glm::mat4 m;
    m = glm::scale(m, glm::vec3(1.0f, 2.0f, 1.0f));
    return m;

}
glm::mat4 translateAlongXY(){
    glm::mat4 m;
    m =  glm::translate(m, glm::vec3(20.0f, 10.0f, 0.0f));
    return m;

}

glm::mat4 composeOrder1(){
    glm::mat4 m;
    m = glm::translate(m, glm::vec3(20.0f, 10.0f, 0.0f));
    m = glm::scale(m, glm::vec3(1.0f, 2.0f, 1.0f));
    m = glm::rotate(m,glm::radians(15.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    return m;
}
glm::mat4 composeOrder2(){
    glm::mat4 m;
    m = glm::rotate(m,glm::radians(15.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    m = glm::scale(m, glm::vec3(1.0f, 2.0f, 1.0f));
    m = glm::translate(m, glm::vec3(20.0f, 10.0f, 0.0f));
    return m;

}


glm::mat4 getModelMatrix()
{
    //Modelling transformations (Model -> World coordinates)
    glm::mat4 model = glm::mat4(1.0);

    //TODO: use the above functions here
//        model = rotateAroundZ();
//        model = scaleAlongY();
//        model = translateAlongXY();
//        model = composeOrder1();
//        model = composeOrder2();

    return model;

}
