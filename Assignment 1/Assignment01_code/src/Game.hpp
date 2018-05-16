
#ifndef ASSIGNMENT1_GAME_HPP
#define ASSIGNMENT1_GAME_HPP



#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>


class GLFWwindow;

class Game {

public:
    Game(int width,int height,const std::string & title = "MainWindow");



    void run();
    ~Game();


    int height;
    int width;
private:

    GLFWwindow * mainWindow = nullptr;

    void update(double delta);
    void draw();

private:

    GLuint spriteProgram;

    glm::mat4 projectionMatrix;
    void showScore(int score);


};


#endif //ASSIGNMENT1_GAME_HPP
