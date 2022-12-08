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

// Global Variables
bool isFirst = false, isPerspective = true;
float screenHeight = 800.f, screenWidth = 800.f,
      lastX = screenWidth / 2.f, lastY = screenHeight / 2.f,
      modPos_x = 0.0, modPos_y = 0.0, modPos_z = 0.0,
      modRot_y = 0.0, pos_y = -10,
      yaw = -90.f, pitch = 0,
      lightIntensity = 0.0,
      deltaTime, lastFrame = 0.0;
float pan_x = 0.0, pan_y = 0.0;
int currIntensity = 1; // 1=low, 2=med, 3=high
glm::vec3 F, R, U, worldUp;

//insert cursor position values


void Key_Callback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void Mouse_Callback(GLFWwindow* window, double xPos, double yPos);


int main(void)
{
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

    // Initialize key callback and mouse input callback
    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    worldUp = glm::normalize(glm::vec3(0.f, 1.5f, 0.f));
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

        // Compute for the velocity needed for camera movement (More uniform for different systems)
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		
        //set cam
        if (isPerspective == true) {
            //pass perspective camera
            if (isFirst == true) {
                //pass 1st perspective
            }
            else {
                //pass 3rd perspective
            }
        }
        else {
            //pass orthographic camera
        }

        // Update Camera
        perspectiveCam.setCameraPos(glm::vec3(modPos_x, modPos_y, modPos_z));
        perspectiveCam.setCameraFRU(F, R, U);
        perspectiveCam.setViewMatrix();
        if (isPerspective) std::cout << "[Perspective] ";
        else std::cout << "[Orthographic] ";
        //std::cout << "CameraPos: " << perspectiveCam.getCameraPos().x << " " << perspectiveCam.getCameraPos().y << " " << perspectiveCam.getCameraPos().z << " | " << modPos_x << " " << modPos_y << " " << modPos_z << "\n";
        std::cout << "CameraPos: " << F.x << " "  << F.y << " " << F.z << " | " << R.x << " " << R.y << " " << R.z << " | " << U.x << " " << U.y << " " << U.z << "\n";

        // Render Skybox
        skybox.draw(skyboxShdr.getShader(), perspectiveCam);
        player.getPlayer().draw(obj5TxtShdr.getShader(), dirLight, perspectiveCam);

        //player.getPlayer().setPos(0, 0, 0);
        //added move function in player object
        // change to modPos_x, modPos_y, modPos_z 
        player.movePlayer(0, 0, 0);

        //draw all models
        /*
        for()
        enemies.draw()
        
        */

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

void Key_Callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    float cameraSpeed = 300.0f * deltaTime;     // Camera Speed/Velocity
    glm::vec3 direction;                        // Current Camera Location
    
    // condition 1st to 3rd person perspective
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        isFirst = !isFirst;
    } 

    // condition perspective to top view
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        isPerspective = !isPerspective;
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        //change intensity
        //set intensity value
        if (currIntensity < 3)
            currIntensity++;
        else
            currIntensity = 1;
    }

    // Perspective (Player) Movement
    if (!isPerspective) {
        if (key == GLFW_KEY_W) {
            // Forward
        }

        if (key == GLFW_KEY_S) {
            // Backward
        }

        if (key == GLFW_KEY_A) {
            // turn left
        }

        if (key == GLFW_KEY_D) {
            // turn right
        }

        // if pos += 0.5 > 0 do not register this.
        if (pos_y += 0.5 < 0) {
            if (key == GLFW_KEY_Q) {
                // Ascend(Depth)
                modPos_y += 0.5;
            }
        }

        if (key == GLFW_KEY_E) {
            // Descend(Depth)
            modPos_y += 0.5;
        }
    }
    else {
        //pan camera in top view
        if (key == GLFW_KEY_W) {
            // Forward
            pan_y += 0.5;
        }

        if (key == GLFW_KEY_S) {
            // Backward
            pan_y -= 0.5;
        }

        if (key == GLFW_KEY_A) {
            // left
            pan_x -= 0.5;
        }

        if (key == GLFW_KEY_D) {
            // right
            pan_y += 0.5;
        }
    }
}

// Perspective (3rd Person Camera) Movement
void Mouse_Callback(GLFWwindow* window, double xPos, double yPos) {
    glm::vec3 direction;

    if(isPerspective && !isFirst) {
        // Variables for current mouse location
        float xpos = static_cast<float>(xPos);
        float ypos = static_cast<float>(yPos);

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f; // Mouse sensitivity value
        xoffset *= sensitivity;         // Compute for mouse x-offset * mouse sensitivity
        yoffset *= sensitivity;         // Compute for mouse y-offset * mouse sensitivity

        yaw += xoffset;     // Adjust horizontal rotation using mouse offset value
        pitch += yoffset;   // Adjust vertical rotation using mouse offset value

        // Maintain Pitch Values to -90 > x < 90
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Compute for the new directional values (for rotation)
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        // Compute for the new camera position/angle
        F = glm::normalize(direction);
        R = glm::normalize(glm::cross(F, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        U = glm::normalize(glm::cross(R, F));
    }
}