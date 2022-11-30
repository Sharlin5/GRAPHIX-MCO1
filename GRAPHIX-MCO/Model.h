#pragma once
class Model {
	private:
		int id = -1, txtCount = -1;
		std::vector<int> img_w, img_h, color_channels;
		float pos_x, pos_y, pos_z; //position
		float rot_x, rot_y, rot_z, theta; // rotation
		float scale_x, scale_y, scale_z; // scale
		std::string warning, error;

		// object data
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		tinyobj::attrib_t attributes;
		std::vector<GLfloat> fullVertexData;
		GLuint VAO, VBO, EBO;
		std::vector<GLuint> mesh_indices, txt;
		std::vector<glm::vec3> tangents, bitangents;
		std::vector<unsigned char*> tex_bytes;
		glm::mat4 transformationMatrix;
	
	public:
		Model() {
			pos_x = pos_y = pos_z = rot_x = rot_y = rot_z = theta = scale_x = scale_y = scale_z = theta = 0;
			warning = error = "";
		}

		Model(std::string objName, int id) {
			this->id = id;
			pos_x = pos_y = pos_z = rot_x = rot_y = rot_z = theta = scale_x = scale_y = scale_z = 0.0;
			
			std::string objPath = "3D/";
			if (id == 0) { 
				objPath += "Player/"; 
			}
			else {
				objPath += "Enemy/" + std::to_string(id) + "/";
			}
			objPath += objName + ".obj";
			bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, objPath.c_str());
			std::cout << success << " " << objPath << "\n";

			//Get tangents and bitangents
			for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
				tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
				tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
				tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

				glm::vec3 v1 = glm::vec3(
					attributes.vertices[vData1.vertex_index * 3],
					attributes.vertices[vData1.vertex_index * 3 + 1],
					attributes.vertices[vData1.vertex_index * 3 + 2]);

				glm::vec3 v2 = glm::vec3(
					attributes.vertices[vData2.vertex_index * 3],
					attributes.vertices[vData2.vertex_index * 3 + 1],
					attributes.vertices[vData2.vertex_index * 3 + 2]);

				glm::vec3 v3 = glm::vec3(
					attributes.vertices[vData3.vertex_index * 3],
					attributes.vertices[vData3.vertex_index * 3 + 1],
					attributes.vertices[vData3.vertex_index * 3 + 2]);

				glm::vec2 uv1 = glm::vec2(
					attributes.texcoords[vData1.texcoord_index * 2],
					attributes.texcoords[vData1.texcoord_index * 2 + 1]);

				glm::vec2 uv2 = glm::vec2(
					attributes.texcoords[vData2.texcoord_index * 2],
					attributes.texcoords[vData2.texcoord_index * 2 + 1]);

				glm::vec2 uv3 = glm::vec2(
					attributes.texcoords[vData3.texcoord_index * 2],
					attributes.texcoords[vData3.texcoord_index * 2 + 1]);

				glm::vec3 deltaPos1 = v2 - v1;
				glm::vec3 deltaPos2 = v3 - v1;

				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;

				float r = 1.f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;


				tangents.push_back(tangent);
				tangents.push_back(tangent);
				tangents.push_back(tangent);

				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);
			}
			
			for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
				tinyobj::index_t vData = shapes[0].mesh.indices[i];

				int vertexIndex = vData.vertex_index * 3;
				int normalIndex = vData.normal_index * 3;
				int uvIndex = vData.texcoord_index * 2;

				// Set X, Y, Z vertex values
				fullVertexData.push_back(attributes.vertices[vertexIndex]);
				fullVertexData.push_back(attributes.vertices[vertexIndex + 1]);
				fullVertexData.push_back(attributes.vertices[vertexIndex + 2]);

				// Set X, Y, Z normals
				fullVertexData.push_back(attributes.normals[normalIndex]);
				fullVertexData.push_back(attributes.normals[normalIndex + 1]);
				fullVertexData.push_back(attributes.normals[normalIndex + 2]);

				// Set U, V coordinate values
				fullVertexData.push_back(attributes.texcoords[uvIndex]);
				fullVertexData.push_back(attributes.texcoords[uvIndex + 1]);

				// Set Tangent values
				fullVertexData.push_back(tangents[i].x);
				fullVertexData.push_back(tangents[i].y);
				fullVertexData.push_back(tangents[i].z);

				// Set Bitangent values
				fullVertexData.push_back(bitangents[i].x);
				fullVertexData.push_back(bitangents[i].y);
				fullVertexData.push_back(bitangents[i].z);
			}

			//Generate and Assign ID to VAO
			glGenVertexArrays(1, &VAO);
			//Generate and Assign ID to VBO
			glGenBuffers(1, &VBO);

			// Bind VAO and VBO
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// Assign Mesh Data to VBO
			glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);
			GLintptr normPtr = 3 * sizeof(GLfloat);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)normPtr);
			GLintptr uvPtr = 6 * sizeof(GLfloat);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);
			GLintptr tangentPtr = 8 * sizeof(float);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)tangentPtr);
			GLintptr bitangentPtr = 11 * sizeof(float);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)bitangentPtr);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			stbi_set_flip_vertically_on_load(true);

			// Initialize Texture Count
			switch (id) {
				case 1:	// Player Sub Texture Count
				case 7:
					txtCount = 5;
					break;
				case 2:	// Enemy Sub Texture Count
					txtCount = 2;
					break;
				case 3:
				case 4:
				case 5:
				case 6:
					txtCount = 0;
			}

			if (txtCount > 0) {
				tex_bytes = std::vector<unsigned char*>(txtCount, 0);
				img_w = std::vector<int>(txtCount, 0);
				img_h = std::vector<int>(txtCount, 0);
				color_channels = std::vector<int>(txtCount, 0);
				txt = std::vector<GLuint>(txtCount, 0);

				// Initialize Textures
				switch (id) {
					case 1:
						tex_bytes[0] = stbi_load("3D/Player/SubLow0Smooth_DefaultMaterial_AO.png", &img_w[0], &img_h[0], &color_channels[0], 4);
						tex_bytes[1] = stbi_load("3D/Player/SubLow0Smooth_DefaultMaterial_BaseColor.png", &img_w[1], &img_h[1], &color_channels[1], 4);
						tex_bytes[2] = stbi_load("3D/Player/SubLow0Smooth_DefaultMaterial_Metallic.png", &img_w[2], &img_h[2], &color_channels[2], 4);
						tex_bytes[3] = stbi_load("3D/Player/SubLow0Smooth_DefaultMaterial_RoughnessAlt.png", &img_w[3], &img_h[3], &color_channels[3], 4);
						tex_bytes[4] = stbi_load("3D/Player/SubLow0Smooth_DefaultMaterial_Normal.png", &img_w[4], &img_h[4], &color_channels[4], 4);
						break;
					case 2:
						tex_bytes[0] = stbi_load("3D/Enemy/1/fna1.jpg", &img_w[0], &img_h[0], &color_channels[0], 0);
						tex_bytes[1] = stbi_load("3D/Enemy/1/fna1b.jpg", &img_w[1], &img_h[1], &color_channels[1], 0);
						break;
					case 7:
						tex_bytes[0] = stbi_load("3D/Enemy/6/Hades_albedo.png", &img_w[0], &img_h[0], &color_channels[0], 4);
						tex_bytes[1] = stbi_load("3D/Enemy/6/Hades_ao.png", &img_w[1], &img_h[1], &color_channels[1], 4);
						tex_bytes[2] = stbi_load("3D/Enemy/6/Hades_metallic.png", &img_w[2], &img_h[2], &color_channels[2], 4);
						tex_bytes[3] = stbi_load("3D/Enemy/6/Hades_roughness.png", &img_w[3], &img_h[3], &color_channels[3], 4);
						tex_bytes[4] = stbi_load("3D/Enemy/6/Hades_normal.png", &img_w[4], &img_h[4], &color_channels[4], 4);
				}

				switch (id) {
					case 1:
					case 7:
						for (int i = 0; i < txtCount; i++) {
							glGenTextures(1, &txt[i]);
							glActiveTexture(GL_TEXTURE0 + i);
							glBindTexture(GL_TEXTURE_2D, txt[i]);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_w[i], img_h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes[i]);
							glGenerateMipmap(GL_TEXTURE_2D);

							stbi_image_free(tex_bytes[i]);
						}
						break;
					case 2:
						for (int i = 0; i < txtCount; i++) {
							glGenTextures(1, &txt[i]);
							glActiveTexture(GL_TEXTURE0 + i);
							glBindTexture(GL_TEXTURE_2D, txt[i]);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w[i], img_h[i], 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes[i]);
							glGenerateMipmap(GL_TEXTURE_2D);

							stbi_image_free(tex_bytes[i]);
						}
				}
			}

			glEnable(GL_DEPTH_TEST);
		}

		void initTransformationMatrix(glm::mat4 identityMatrix) {
			transformationMatrix = glm::translate(identityMatrix, glm::vec3(pos_x, pos_y, pos_z));
			transformationMatrix = glm::scale(transformationMatrix, glm::vec3(scale_x, scale_y, scale_z));
			transformationMatrix = glm::rotate(transformationMatrix, glm::radians(theta), glm::normalize(glm::vec3(rot_x, rot_y, rot_z)));
		}

		void draw(GLuint shaderProgram, Light light, Camera cam) {
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);

			glUseProgram(shaderProgram);

			// Link Light Variables to Shader
			light.linkShader(shaderProgram, cam);

			GLuint texAddress;

			if (txtCount > 0) {
				switch (id) {
					case 1:
					case 7:
						for (int i = 0; i < txtCount - 1; i++) {
							texAddress = glGetUniformLocation(shaderProgram, "tex" + i);
							glActiveTexture(GL_TEXTURE0 + i);
							glBindTexture(GL_TEXTURE_2D, txt[i]);
							glUniform1i(texAddress, i);
						}

						texAddress = glGetUniformLocation(shaderProgram, "norm0");
						glActiveTexture(GL_TEXTURE4);
						glBindTexture(GL_TEXTURE_2D, txt[4]);
						glUniform1i(texAddress, 4);
						break;
					case 2:
						for (int i = 0; i < txtCount; i++) {
							texAddress = glGetUniformLocation(shaderProgram, "tex" + i);
							glActiveTexture(GL_TEXTURE0 + i);
							glBindTexture(GL_TEXTURE_2D, txt[i]);
							glUniform1i(texAddress, i);
						}
				}
			}

			// Initialize projection, view, and transformation locations
			unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));
			unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
			unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
			glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);
			
			
			std::cout << pos_x << " " << pos_y << " " << pos_z << " | " << rot_x << " " << rot_y << " " << rot_z << " | " << scale_x << " " << scale_y << " " << scale_z << "\n";
		}

		void deleteData() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

		void setPos(float pos_x, float pos_y, float pos_z) {
			this->pos_x = pos_x;
			this->pos_y = pos_y;
			this->pos_z = pos_z;
		}

		void setRot(float rot_x, float rot_y, float rot_z, float theta) {
			this->rot_x = rot_x;
			this->rot_y = rot_y;
			this->rot_z = rot_z;
			this->theta = theta;
		}

		void setScale(float scale_x, float scale_y, float scale_z) {
			this->scale_x = scale_x;
			this->scale_y = scale_y;
			this->scale_z = scale_z;
		}

};
