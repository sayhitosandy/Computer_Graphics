#include <imgui_impl_glfw_gl3.h>
#include "Game.hpp"
#include "utitliy.hpp"
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using std::cout;
using std::endl;

GLuint VAO_Player, VBO_Player, EBO_Player, VAO_Wall, VBO_Wall, EBO_Wall, VAO_Ball, VBO_Ball, EBO_Ball;

GLfloat vertices_Player[] = {           //Rectangular player using 2 triangles having 2 vertices common
        0.25f, -0.75f, 0.0f,    // Top Right
        0.25f, -0.8f, 0.0f,     // Bottom Right
        -0.25f, -0.8f, 0.0f,    // Bottom Left
        -0.25f, -0.75f, 0.0f    // Top Left
};

int stop = 0;
int total_score = 0;

unsigned int indices_Player[] = {       //indices for the vertices of the rectangular player
        0, 1, 2,
        0, 2, 3
};

GLfloat vertices_Wall[] = {         //3 Rectangular walls using 6 triangles
        1.0f, 1.0f, 0.0f,       // Top right
        1.0f, -1.0f, 0.0f,      // Bottom Right
        0.9f, -1.0f, 0.0f,      // Bottom Right little left
        0.9f, 0.9f, 0.0f,       // Top right little left
        -0.9f, 0.9f, 0.0f,      // Top left little right
        -0.9f, -1.0f, 0.0f,     // Bottom left little right
        -1.0f, -1.0f, 0.0f,     // Bottom Left
        -1.0f, 1.0f, 0.0f       // Top left
};

unsigned int indices_Wall[] = {     //indices for the vertices of the 3 rectangular walls
        0, 1, 3,
        1, 2, 3,
        0, 3, 4,
        0, 4, 7,
        4, 5, 6,
        4, 6, 7
};

GLfloat vertices_Ball[45] = {};
unsigned int indices_Ball[15] = {};

GLfloat xSpeed = 0.01f, ySpeed = 0.01f;
GLfloat ballXVelocity = 0.01f, ballYVelocity = 0.01f;
GLfloat ballRadius = 0.05f;

Game::Game(int width,int height,const std::string & title):width(width),height(height) {
    {
        glfwInit();
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

    // Create a GLFWwindow object
    mainWindow = glfwCreateWindow(width,height,title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(mainWindow,this);

    if (mainWindow == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(mainWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize OpenGL context" << endl;
        exit(-1);
    }

    ImGui_ImplGlfwGL3_Init(mainWindow,false);

    //setup matrix
    {
        projectionMatrix = glm::ortho(0.0f,width*1.0f,0.0f,height*1.0f,-1.0f,1.0f);
    }

    glViewport(0,0, width, height);

    //compile shader
    {
        auto vs = fileToCharArr ("resources/sprite.vert");
        auto fs = fileToCharArr ("resources/sprite.frag");
        spriteProgram = makeProgram(compileShader(shDf(GL_VERTEX_SHADER,&vs[0],vs.size())),compileShader(shDf(GL_FRAGMENT_SHADER,&fs[0],fs.size())));

    }
}

//create ball using triangle_fan
void createBall(GLfloat centre_x, GLfloat centre_y, GLfloat radius) {           //No. of triangles used to create a ball = 15
    vertices_Ball[0] = centre_x;
    vertices_Ball[1] = centre_y;
    vertices_Ball[2] = (GLfloat)0.0f;

    GLfloat angle;

    for (int i=3; i<45; i+=3) {
        angle = (i/3)*2.5f*M_PI/15;
        vertices_Ball[i] = (GLfloat) (centre_x + (cos(angle)*radius));
        vertices_Ball[i+1] = (GLfloat) (centre_y + (sin(angle)*radius));
        vertices_Ball[i+2] = (GLfloat) 0.0f;
    }
}

void Game::run() {

    glGenVertexArrays(1, &VAO_Player);      //generate
    glGenBuffers(1, &VBO_Player);
    glGenBuffers(1, &EBO_Player);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Player);      //bind
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Player), vertices_Player, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Player);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_Player), indices_Player, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO_Player);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Player);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Player);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);       //unbind

    glGenVertexArrays(1, &VAO_Wall);        //generate
    glGenBuffers(1, &VBO_Wall);
    glGenBuffers(1, &EBO_Wall);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Wall);        //bind
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Wall), vertices_Wall, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Wall);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_Wall), indices_Wall, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO_Wall);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Wall);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Wall);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);       //unbind

    createBall(0.0f, -0.7f, ballRadius);         //create the ball
    for (int i=0; i<15; i++) {
        indices_Ball[i] = i;
    }

    glGenVertexArrays(1, &VAO_Ball);      //generate
    glGenBuffers(1, &VBO_Ball);
    glGenBuffers(1, &EBO_Ball);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Ball);      //bind
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Ball), vertices_Ball, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Ball);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_Ball), indices_Ball, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO_Ball);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Ball);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Ball);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);       //unbind

    double last = 0;
    glfwSetTime(last);
    double delta = 0.0f;
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(mainWindow)) {

        double curr = glfwGetTime();
        delta = curr-last;
        last = curr;
        //input polling
        glfwPollEvents();
        //update objects
        update(delta);

        //draw them
        glUseProgram(spriteProgram);
        draw();

        glfwSwapBuffers(mainWindow);

    }
}

Game::~Game() {
    glfwTerminate();
}

void Game::update(double delta) {
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
    }

    if (glfwGetKey(mainWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
        stop = (stop == 0)? 1 : 0;
    }
    if (stop == 0) {

        if (glfwGetKey(mainWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {        //key controller for left arrow key and limits
            if (vertices_Player[9] >= -0.9) {
                for (int i = 0; i <= 3; i++) {
                    vertices_Player[i * 3] -= (float) delta;
                }
            }
        }

        if (glfwGetKey(mainWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {        //key controller for right arrow key and limits
            if (vertices_Player[3] <= 0.9) {
                for (int i = 0; i <= 3; i++) {
                    vertices_Player[i * 3] += (float) delta;
                }
            }
        }


        {
            ballXVelocity += xSpeed;
            ballYVelocity += ySpeed;
            createBall(ballXVelocity, ballYVelocity, ballRadius);
            if (!(ballXVelocity + ballRadius >= -0.8f && ballXVelocity + ballRadius <= 0.9f)) {
                xSpeed = -xSpeed;
            }
            if (!(ballYVelocity + ballRadius >= -0.9f && ballYVelocity + ballRadius <= 0.9f)) {
                ySpeed = -ySpeed;
            }
            if (ballXVelocity + ballRadius >= vertices_Player[6] && ballXVelocity + ballRadius <= vertices_Player[3]) {
                if (ballYVelocity - ballRadius <= -0.75f && ballYVelocity + ballRadius >= -0.8f) {
                    ySpeed = -ySpeed;
                    total_score++;
                } else if (ballYVelocity - ballRadius >= -0.75f && ballYVelocity + ballRadius <= -0.8f) {
                    ySpeed = -ySpeed;
                }
            }
            if (!(ballYVelocity+ballRadius >= -0.9f)) {
                total_score = 0;
            }
        }

        //update buffers for player
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Player);      //bind
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Player), vertices_Player, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Player);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_Player), indices_Player, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO_Player);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Player);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Player);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);       //unbind

        //update buffers for ball
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Ball);      //bind
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Ball), vertices_Ball, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Ball);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_Ball), indices_Ball, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO_Ball);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Ball);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Ball);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);       //unbind
    }
}

void Game::draw() {

    glClearColor(1.0f,1.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO_Player);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);        //draw the rectangular player using 2 triangles
    glBindVertexArray(0);

    glBindVertexArray(VAO_Wall);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);       //draw the walls using 6 triangles
    glBindVertexArray(0);

    glBindVertexArray(VAO_Ball);                                //draw the ball using 15 triangles
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    showScore(total_score);

}


void Game::showScore(int score) {
    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetNextWindowSize(ImVec2(100, 10), ImGuiSetCond_FirstUseEver);

    ImGui::SetNextWindowPos(ImVec2(490,0));
    bool show_another_window = true;
    ImGui::Begin("Score", &show_another_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

    ImGui::Text("Score %d    ",score);

    ImGui::End();
    ImGui::Render();
}