#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tvp.hpp"


//TODO implement this functions for part 2
glm::mat4 myLookAt(const glm::vec3 eye, const glm::vec3 center, const glm::vec3 up)
{
    //TODO:your look at definition
    //e = eye, g = center - eye, t = up
    glm::vec3 u, v, w;
    w = glm::normalize(eye - center);
    u = glm::normalize(glm::cross(glm::normalize(up), w));
    v = glm::cross(w, u);

    glm::mat4 rot;  //column major format
    rot[0][0] = u.x;
    rot[0][1] = v.x;
    rot[0][2] = w.x;
    rot[1][0] = u.y;
    rot[1][1] = v.y;
    rot[1][2] = w.y;
    rot[2][0] = u.z;
    rot[2][1] = v.z;
    rot[2][2] = w.z;

    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(-eye.x, -eye.y, -eye.z));

    glm::mat4 la = rot * trans;

    return la;
}

glm::mat4 getViewMatrix(){

    //TODO call your lookAt function here
    //Camera at (0, 0, 100) looking down the negative Z-axis in a right handed coordinate system
//    return  glm::lookAt(glm::vec3(50.0,50.0,50.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    //Orthogonal Projections:
    //Top:
//    return  myLookAt(glm::vec3(0.0,100.0,0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    //Front:
    return  myLookAt(glm::vec3(0.0,0.0,100.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    //Side:
//    return  myLookAt(glm::vec3(100.0,0.0,0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    //Isometric:
//    return  myLookAt(glm::vec3(50.0,50.0,50.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

//    return myLookAt(glm::vec3(50.0, 0.0, 100.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}


//TODO change projection matrix here
glm::mat4 getProjectionMatrix(float aspect, float height, float width) {
    //Orthogonal:
    float view_height = 100.0f;
    glm::mat4 projection = glm::ortho(-view_height*aspect/2.0f, view_height*aspect/2.0f, -view_height/2.0f, view_height/2.0f, 0.1f, 1000.0f);

    //Prespective:
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.f);

    //Two-point
//    glm::mat4 rot, rot2;
//    GLfloat q = 3.0f;
//    rot = glm::rotate(rot, glm::radians(-q), glm::vec3(0.0f, 1.0f, 0.0f));
//    rot2 = glm::rotate(rot2, glm::radians(q), glm::vec3(0.0f, 1.0f, 0.0f));
//    projection = rot * projection * rot2;

//    //Three-point
//    //Rat's View
//    glm::mat4 rot3, rot4;
//    GLfloat j = 3.0f;
//    rot3 = glm::rotate(rot3, glm::radians(-j), glm::vec3(1.0f, 0.0f, 0.0f));
//    rot4 = glm::rotate(rot4, glm::radians(j), glm::vec3(1.0f, 0.0f, 0.0f));
//    projection = rot3 * projection * rot4;

    //Bird's View
//    glm::mat4 rot3, rot4;
//    GLfloat j = 3.0f;
//    rot3 = glm::rotate(rot3, glm::radians(-j), glm::vec3(-1.0f, 0.0f, 0.0f));
//    rot4 = glm::rotate(rot4, glm::radians(j), glm::vec3(-1.0f, 0.0f, 0.0f));
//    projection = rot3 * projection * rot4;

    return projection;


}
