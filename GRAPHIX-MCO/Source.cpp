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
PerspectiveCamera perspectiveCam;
OrthographicCamera orthoCam;
Player player;
std::vector<Model> enemies;

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
    glm::mat4 orthoMatrix = glm::ortho(screenWidth, screenHeight, -1.f, 1.f, -.75f, .75f);

    // Load Shaders
    Shader obj1TxtShdr = Shader("1txtModel");
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
    perspectiveCam = PerspectiveCamera(cameraPos, cameraCenter, worldUp, projectionMatrix); 
    F = cameraCenter - cameraPos;
    F = glm::normalize(F);
    R = glm::cross(F, worldUp);
    U = glm::cross(R, F);
    perspectiveCam.setCameraFRU(F, R, U);

    cameraPos = glm::vec3(0.f, 70.f, 15.f);
    cameraCenter = glm::vec3(0.f, 3.f, 0.f);
    glm::vec3 orthoworldUp = glm::normalize(glm::vec3(0.f, 90.f, 0.f));
    orthoCam = OrthographicCamera(cameraPos, cameraCenter, orthoworldUp, projectionMatrix);
    F = cameraCenter - cameraPos;
    F = glm::normalize(F);
    R = glm::cross(F, orthoworldUp);
    U = glm::cross(R, F);
    orthoCam.setCameraFRU(F, R, U);
    
    // TODO: Add Source Links
    // Load Player Model
    player = Player(identityMatrix);

    // Load Enemy Models

    /* Seaview Submarine by herminio (Free3D)
       Link: https://free3d.com/3d-model/seaview-submarine-78646.html */
    enemies.push_back(Model("Seaview_submarine", 1));

    /* 
    */
    enemies.push_back(Model("11098_submarine_v4", 2));

    /*
    */
    enemies.push_back(Model("Cat_Low", 3));

    /* Cyclops Subnautica by Naudaff3D (CGTrader)
    * Link: https://www.cgtrader.com/free-3d-models/vehicle/other/cyclops-subnautica */
    enemies.push_back(Model("Cyclops_Subnautica", 4));

    /* Submarine by ptrojan (TurboSquid)
       Link: https://www.turbosquid.com/3d-models/free-submarine-3d-model/708103 */
    enemies.push_back(Model("Submarine", 5));

    /* Hades Carrier by Crazycatdev (TurboSquid)
       Link: https://www.turbosquid.com/3d-models/hades-cruiser-obj-free/1133051 */
    enemies.push_back(Model("Hades_Carrier", 6));

    // Manually set each model's rot and scale values
    enemies[0].setScale(0.05f, 0.05f, 0.05f);       // Seaview_submarine
    enemies[1].setRot(-90.f, 0.f, 0.f, 90.f);       // 11090_submarine_v4
    enemies[1].setScale(0.003f, 0.003f, 0.003f);
    enemies[2].setScale(0.3f, 0.3f, 0.3f);          // Cat_Low
    float temp_x = enemies[2].getPosX();
    float temp_z = enemies[2].getPosZ();
    enemies[2].setPos(temp_x, -8.f, temp_z);// Cat_Low
    enemies[3].setScale(1.f, 1.f, 1.f);             // Cyclops_Subnautica
    enemies[3].setRot(0.f, 90.f, 0.f, 90.f);
    enemies[4].setScale(0.01f, 0.01f, 0.01f);       // Submarine
    enemies[4].setRot(0.f, 90.f, 0.f, 90.f);
    enemies[5].setScale(0.001f, 0.001f, 0.001f);    // Hades_Carrier

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute for the velocity needed for camera movement (More uniform for different systems)
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update Camera
        Camera currCam;
        if (isPerspective){
            currCam = perspectiveCam;
            currCam.setCameraFRU(F, R, U);
            currCam.setViewMatrix();
        }
        else {
            currCam = orthoCam;
            currCam.setOrthoView();
        }

        //std::cout << "CameraPos: " << currCam.getCameraPos().x << " " << currCam.getCameraPos().y << " " << currCam.getCameraPos().z << "\n";
        //std::cout << "CameraPos: " << F.x << " "  << F.y << " " << F.z << " | " << R.x << " " << R.y << " " << R.z << " | " << U.x << " " << U.y << " " << U.z << "\n";

        //if (isPerspective) {
        //    std::cout << "[Perspective] ";
        //    if (isFirst) std::cout << "[1P] ";
        //    else std::cout << "[3P] ";
        //}
        //else std::cout << "[Orthographic] ";        

        // Render Skybox
        skybox.draw(skyboxShdr.getShader(), currCam);

        // Render Player
        player.getPlayer().initTransformationMatrix(identityMatrix);
        //player.getPlayer().draw(obj5TxtShdr.getShader(), dirLight, currCam);
        
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].initTransformationMatrix(identityMatrix);
            

            switch (enemies[i].getID()) {
                case 1:
                    enemies[i].draw(obj2TxtShdr.getShader(), dirLight, currCam);
                    break;
                case 0:
                case 6:
                    enemies[i].draw(obj2TxtShdr.getShader(), dirLight, currCam);
                    break;
                default:
                    enemies[i].draw(obj2TxtShdr.getShader(), dirLight, currCam);
            }            
        }

        //std::cout << "[Player " << player.getPlayer().getID() << " Location] " << player.getPlayer().getPosX() << ' ' << player.getPlayer().getPosY() << ' ' << player.getPlayer().getPosZ() << " | " << player.getPlayer().getRotX() << ' ' << player.getPlayer().getRotY() << ' ' << player.getPlayer().getRotZ() << " | " << player.getPlayer().getScaleX() << ' ' << player.getPlayer().getScaleY() << ' ' << player.getPlayer().getScaleZ() << '\n';

        //player.getPlayer().setPos(0, 0, 0);
        //added move function in player object
        // change to modPos_x, modPos_y, modPos_z 
        //player.movePlayer(0, 0, 0);

        //update light intensity
        if (currIntensity == 1) {
            lightIntensity = 0.0;
        } else if (currIntensity == 2) {
            lightIntensity = 1.0;
        } else if (currIntensity == 3) {
            lightIntensity = 1.5;
        }

        pointLight.setIntensity(lightIntensity);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    skybox.deleteData();
    player.getPlayer().deleteData();
    for (Model model : enemies)
        model.deleteData();

    glfwTerminate();
    return 0;
}

void Key_Callback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    float cameraSpeed = 300.0f * deltaTime;     // Camera Speed/Velocity
    
    // condition 1st to 3rd person perspective
    if (isPerspective && key == GLFW_KEY_1 && action == GLFW_PRESS) {
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
    if (isPerspective) {
        if (key == GLFW_KEY_W) {
            // Forward
            perspectiveCam.setCameraPos(perspectiveCam.getCameraPos() + cameraSpeed * perspectiveCam.getCameraF());
            player.getPlayer().setPos(perspectiveCam.getCameraPos().x, perspectiveCam.getCameraPos().y, perspectiveCam.getCameraPos().z);
            //perspectiveCam.setViewMatrix();
            std::cout << perspectiveCam.getCameraPos().x << ' ' << perspectiveCam.getCameraPos().y << ' ' << perspectiveCam.getCameraPos().z << "\n";
        }

        if (key == GLFW_KEY_S) {
            // Backward
            perspectiveCam.setCameraPos(perspectiveCam.getCameraPos() - cameraSpeed * perspectiveCam.getCameraF());
            player.getPlayer().setPos(perspectiveCam.getCameraPos().x, perspectiveCam.getCameraPos().y, perspectiveCam.getCameraPos().z);
            //perspectiveCam.setViewMatrix();
            std::cout << perspectiveCam.getCameraPos().x << ' ' << perspectiveCam.getCameraPos().y << ' ' << perspectiveCam.getCameraPos().z << "\n";
        }

        if (key == GLFW_KEY_A) {
            // turn left
            perspectiveCam.setCameraPos(perspectiveCam.getCameraPos() - cameraSpeed * glm::normalize(glm::cross(perspectiveCam.getCameraF(), perspectiveCam.getCameraU())));
            player.getPlayer().setPos(perspectiveCam.getCameraPos().x, perspectiveCam.getCameraPos().y, perspectiveCam.getCameraPos().z);
            //perspectiveCam.setViewMatrix();
            std::cout << perspectiveCam.getCameraPos().x << ' ' << perspectiveCam.getCameraPos().y << ' ' << perspectiveCam.getCameraPos().z << "\n";
        }

        if (key == GLFW_KEY_D) {
            // turn right
            perspectiveCam.setCameraPos(perspectiveCam.getCameraPos() + cameraSpeed * glm::normalize(glm::cross(perspectiveCam.getCameraF(), perspectiveCam.getCameraU())));
            player.getPlayer().setPos(perspectiveCam.getCameraPos().x, perspectiveCam.getCameraPos().y, perspectiveCam.getCameraPos().z);
            //perspectiveCam.setViewMatrix();
            std::cout << perspectiveCam.getCameraPos().x << ' ' << perspectiveCam.getCameraPos().y << ' ' << perspectiveCam.getCameraPos().z << "\n";
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
            //pan_y += 0.5;
            orthoCam.setCameraFRU(glm::vec3(F.x + 0.5f, F.y, F.z), R, U);
            orthoCam.setOrthoView();
        }

        if (key == GLFW_KEY_S) {
            // Backward
            pan_y -= 0.5;
            orthoCam.setCameraFRU(glm::vec3(F.x - 0.5f, F.y, F.z), R, U);
            orthoCam.setCameraFRU(F, R, U);
            orthoCam.setOrthoView();
        }

        if (key == GLFW_KEY_A) {
            // left
            pan_x -= 0.5;
            orthoCam.setCameraFRU(glm::vec3(F.x, F.y, F.z - 0.5f), R, U);
            orthoCam.setCameraFRU(F, R, U);
            orthoCam.setOrthoView();
        }

        if (key == GLFW_KEY_D) {
            // right
            //pan_x += 0.5;
            orthoCam.setCameraPos(glm::vec3(orthoCam.getCameraPos().x + 0.5, orthoCam.getCameraPos().y, orthoCam.getCameraPos().z));
            orthoCam.setCameraFRU(F, R, U);
            orthoCam.setOrthoView();
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