#pragma once
// Class for creating Model
class Model {
private:
	float pos_x, pos_y, pos_z; //position
	float rot_x, rot_y, rot_z, theta; // rotation
	float scale_x, scale_y, scale_z; // scale
	// object data
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	tinyobj::attrib_t attributes;
	std::vector<GLfloat> fullVertexData;
	GLuint VAO, VBO;
	// matrix
	glm::mat4 identity_matrix;
	
public:
	Model() {
		//Initialization of all objects
		pos_x = pos_y = pos_z = 0.0;
		rot_x = rot_y = rot_z = theta = 0.0; 
		scale_x = scale_y = scale_z = 0.0;
		identity_matrix = glm::mat4(1.0f);
	}

	Model(std::string path) {
		//Initialization of all objects
		pos_x = pos_y = pos_z = 0.0;
		rot_x = rot_y = rot_z = theta = 0.0;
		scale_x = scale_y = scale_z = 0.0;
		identity_matrix = glm::mat4(1.0f);

		std::string warning, error;

		bool success = tinyobj::LoadObj(
			&attributes,
			&shapes,
			&materials,
			&warning,
			&error,
			path.c_str()
		);
		
		for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
			tinyobj::index_t vData = shapes[0].mesh.indices[i];

			int vertexIndex = vData.vertex_index * 3;

			int normalIndex = vData.normal_index * 3;

			int uvIndex = vData.texcoord_index * 2;

			// x
			fullVertexData.push_back(attributes.vertices[vertexIndex]);
			// y
			fullVertexData.push_back(attributes.vertices[vertexIndex + 1]);
			// z
			fullVertexData.push_back(attributes.vertices[vertexIndex + 2]);

			fullVertexData.push_back(attributes.normals[normalIndex]);

			fullVertexData.push_back(attributes.normals[normalIndex + 1]);

			fullVertexData.push_back(attributes.normals[normalIndex + 2]);

			// u
			fullVertexData.push_back(attributes.texcoords[uvIndex]);
			// v
			fullVertexData.push_back(attributes.texcoords[uvIndex + 1]);
		}

		//Generate and Assign ID to VAO
		glGenVertexArrays(1, &VAO);
		//Generate and Assign ID to VBO
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GL_FLOAT) * fullVertexData.size(),
			fullVertexData.data(),
			GL_STATIC_DRAW
		);

		glVertexAttribPointer(
			0, //Pos
			3, //xyz
			GL_FLOAT,
			GL_FALSE,
			14 * sizeof(GL_FLOAT),
			(void*)0
		);

		GLintptr normPtr = 3 * sizeof(float);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			14 * sizeof(GL_FLOAT),
			(void*)normPtr
		);

		GLintptr uvPtr = 6 * sizeof(float);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			14 * sizeof(GL_FLOAT),
			(void*)uvPtr
		);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void changePos(float x, float y, float z) {
		pos_x = x;
		pos_y = y;
		pos_z = z;
	}

	void changeRot(float x, float y, float z, float tempTheta) {
		rot_x = x;
		rot_y = y;
		rot_z = z;
		theta = tempTheta;
	}

	void changeScale(float x, float y, float z) {
		scale_x = x;
		scale_y = y;
		scale_z = z;
	}

};
