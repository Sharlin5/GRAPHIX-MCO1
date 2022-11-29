#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//include classes
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"
#include "Player.h"

float screenHeight = 800, screenWidth = 800;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenHeight, screenWidth, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // load player model
   // Player player = Player();
    // load enemy models
  //  Model model1 = Model();
   // Model model2 = Model();
   // Model model3 = Model();
   // Model model4 = Model();
   // Model model5 = Model();
   // Model model6 = Model();

    //Shader skyboxShader = Shader("Shader/main.vert", "Shader/main.frag");

    std::fstream skybox_vertSrc("Shaders/skybox.vert");
    std::stringstream skybox_vertBuff;
    skybox_vertBuff << skybox_vertSrc.rdbuf();
    std::string skybox_vertString = skybox_vertBuff.str();
    const char* skybox_v = skybox_vertString.c_str();

    //Fragment shader
    std::fstream skybox_fragSrc("Shaders/skybox.frag");
    std::stringstream skybox_fragBuff;
    skybox_fragBuff << skybox_fragSrc.rdbuf();
    std::string skybox_fragString = skybox_fragBuff.str();
    const char* skybox_f = skybox_fragString.c_str();


    //Instantiate and compile vertex shader
    GLuint skybox_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skybox_vertexShader, 1, &skybox_v, NULL);
    glCompileShader(skybox_vertexShader);

    //Instantiate and compile fragment shader
    GLuint skybox_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skybox_fragShader, 1, &skybox_f, NULL);
    glCompileShader(skybox_fragShader);

    //create shader program and attach both vertex and fragment shader to it
    GLuint skybox_shaderProg = glCreateProgram();
    glAttachShader(skybox_shaderProg, skybox_vertexShader);
    glAttachShader(skybox_shaderProg, skybox_fragShader);
    glLinkProgram(skybox_shaderProg);

    //Skybox skybox = Skybox("Shader/skybox.vert", "Shader/skybox.frag");

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //drawmodels
        //skybox.drawSkybox(screenHeight, screenWidth);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}