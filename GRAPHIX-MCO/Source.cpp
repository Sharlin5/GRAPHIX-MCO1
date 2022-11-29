// Initialize C++ libraries
#include <string>
#include <iostream>

// Initialize GLAD and GLFW libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Initialize GLM libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Initialize object loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// Initialize image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Prerequisite Libraries
#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Model.h"
#include "Skybox.h"
#include "Player.h"

int main(void)
{
    float screenHeight, screenWidth;
    screenHeight = screenWidth = 800.f;

    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(screenHeight, screenWidth, "GRAPHIX MCO1: Gregorio, Tang", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context the current context
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    gladLoadGL();
    glViewport(0, 0, screenWidth, screenHeight);

    // Declare Identity, Projection, and Orthographic Matrices to be used
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), screenHeight / screenWidth, 0.1f, 100.f);
    glm::mat4 orthoMatrix = glm::ortho(-5.0f, 5.0f, -6.0f, 6.0f, -15.0f, 15.0f);

    // Load Shaders
    Shader obj2TxtShdr = Shader("2txtModel");
    Shader obj5TxtShdr = Shader("5txtModel");
    Shader skyboxShdr = Shader("skybox");

    // Initialize Skybox
    Skybox skybox = Skybox();

    // Initialize Point Light and Directional Light
    DirLight dirLight = DirLight(glm::vec3(10.f, 10.f, 10.f), 1);
    PointLight pointLight = PointLight(glm::vec3(-10.f, -10.f, -10.f), 2);

    // Initialize Ortographic and Perspective Cameras// Initialize Camera
    glm::vec3 cameraPos = glm::vec3(0.f, 5.f, 10.0f);
    glm::vec3 cameraCenter = glm::vec3(0.f, 5.f, 0.f);
    glm::vec3 worldUp = glm::normalize(glm::vec3(0.f, 1.5f, 0.f));
    OrthographicCamera orthoCam = OrthographicCamera(cameraPos, cameraCenter, worldUp, orthoMatrix);
    PerspectiveCamera perspectiveCam = PerspectiveCamera(cameraPos, cameraCenter, worldUp, projectionMatrix);

    
    // TODO: Add Source Links
    // Load Player Model
    Player player = (identityMatrix);
    // Load Enemy Models
    //std::vector<Model> enemies;
    //enemies.push_back(Model("Seaview_submarine", 1));
    //enemies.push_back(Model("11098_submarine_v4", 2));
    //enemies.push_back(Model("submarine", 3));
    //enemies.push_back(Model("Cyclops_Subnautica", 4));
    //enemies.push_back(Model("Submarine", 5));
    //enemies.push_back(Model("Hades_Carrier", 6));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        // Render Skybox
        skybox.draw(skyboxShdr.getShader(), perspectiveCam);
        player.getPlayer().draw(obj5TxtShdr.getShader(), dirLight, perspectiveCam);
        player.getPlayer().setPos(0, 0, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    skybox.deleteData();
    player.getPlayer().deleteData();
    //for (Model model : enemies)
    //    model.deleteData();

    glfwTerminate();
    return 0;
}