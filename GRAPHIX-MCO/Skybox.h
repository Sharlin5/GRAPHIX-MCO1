#pragma once
class Skybox {
private:
    GLuint skybox_shaderProgram;
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    unsigned int skyboxTex;
public:
	Skybox(std::string vert, std::string frag) {
        float skyboxVertices[]{
            -1.f, -1.f, 1.f, //0
            1.f, -1.f, 1.f,  //1
            1.f, -1.f, -1.f, //2
            -1.f, -1.f, -1.f,//3
            -1.f, 1.f, 1.f,  //4
            1.f, 1.f, 1.f,   //5
            1.f, 1.f, -1.f,  //6
            -1.f, 1.f, -1.f  //7
        };

        //Skybox Indices
        unsigned int skyboxIndices[]{
            1,2,6,
            6,5,1,

            0,4,7,
            7,3,0,

            4,5,6,
            6,7,4,

            0,3,2,
            2,1,0,

            0,1,5,
            5,4,0,

            3,7,6,
            6,2,3
        };

        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        std::string facesSkybox[]{
            "Skybox/rainbow_rt.png",
            "Skybox/rainbow_lf.png",
            "Skybox/rainbow_up.png",
            "Skybox/rainbow_dn.png",
            "Skybox/rainbow_ft.png",
            "Skybox/rainbow_bk.png"
        };

        glGenTextures(1, &skyboxTex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        //Prevents Pixelating
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //Prevents tiling
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        for (unsigned int i = 0; i < 6; i++) {
            int w, h, skyCChannel;
            stbi_set_flip_vertically_on_load(false);

            unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    w,
                    h,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
                );
                stbi_image_free(data);
            }
        }
	}

    void drawSkybox(float height, float width) {
        glm::mat4 projection = glm::perspective(glm::radians(60.f), height / width, 0.1f, 100.f);

        float cpos_x = 0.0f;
        float cpos_z = 10.0f;
        glm::vec3 F, R, U;
        glm::vec3 cameraPos;

        //Instantiate eye variable
        cameraPos = glm::vec3(cpos_x, 0, 10.f);
        //camera position matrix
        glm::mat4 cameraPositionMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.0f);

        //World's up direction
        //normally just 1 in Y
        glm::vec3 WorldUp = glm::vec3(0, 1.0f, 0);
        //Camera's center
        glm::vec3 Center = glm::vec3(0, 0.0f, 0);

        //apply Center - eye 
        F = glm::vec3(Center - cameraPos);
        //normalize forward vector
        F = glm::normalize(F);
        //Get the Right
        R = glm::normalize(glm::cross(F, WorldUp));
        //Get the Up
        U = glm::normalize(glm::cross(R, F));

        //Construct the Camera Orientation Matrix
        glm::mat4 cameraOrientation = glm::mat4(1.f);

        //Manually assign the matrix
        //Matrix[col][row]
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;

        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;

        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;


        //Camera view matrix
        glm::mat4 viewMatrix = cameraOrientation * cameraPositionMatrix;

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        //Load Skybox
        glUseProgram(skybox_shaderProgram);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int sky_projLoc = glGetUniformLocation(skybox_shaderProgram, "projection");
        glUniformMatrix4fv(sky_projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int sky_viewLoc = glGetUniformLocation(skybox_shaderProgram, "view");
        glUniformMatrix4fv(sky_viewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
};
