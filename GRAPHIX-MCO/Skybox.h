#pragma once
class Skybox {
	private:
		unsigned int VAO, VBO, EBO, txt;
		
	public:
		Skybox() {
			/*
                7--------6
                /|       /|
                4--------5 |
                | |      | |
                | 3------ | -2
                |/       |/
                0--------1
            */
            //Vertices for the cube
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
			
			// Generate VAO, VBO, and EBO
			glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

			// Bind VAO
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

			// Bind VBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);

			// Initialize Skybox Textures
            std::string facesSkybox[]{
                "Skybox/ocean_rt.png",
                "Skybox/ocean_lf.png",
                "Skybox/ocean_tp.png",
                "Skybox/ocean_bt.png",
                "Skybox/ocean_ft.png",
                "Skybox/ocean_bk.png"
            };
			
			glGenTextures(1, &txt);
            glBindTexture(GL_TEXTURE_CUBE_MAP, txt);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			for (unsigned int i = 0; i < 6; i++) {
                int w, h, color_channels;

                stbi_set_flip_vertically_on_load(false);

                unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &color_channels, 4);

                if (data)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            }

            stbi_set_flip_vertically_on_load(true);
		}
		
		void draw(GLuint shaderProgram, Camera camera) {
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            glUseProgram(shaderProgram);

            glm::mat4 sky_view = glm::mat4(1.0f);
            sky_view = glm::mat4(glm::mat3(camera.getViewMatrix()));

            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

            glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, txt);
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        void deleteData() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }
};
