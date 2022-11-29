#pragma once
class Shader
{
private:
	GLuint shaderProgram;

public:
	Shader(std::string shaderFile) {
		// Flips texture right side up
		stbi_set_flip_vertically_on_load(true);

		std::fstream vertSrc("Shader/" + shaderFile + ".vert");
		std::stringstream vertBuff;
		vertBuff << vertSrc.rdbuf();
		std::string vertString = vertBuff.str();
		const char* v = vertString.c_str();

		std::fstream fragSrc("Shader/" + shaderFile + ".frag");
		std::stringstream fragBuff;
		fragBuff << fragSrc.rdbuf();
		std::string fragString = fragBuff.str();
		const char* f = fragString.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &v, NULL);
		glCompileShader(vertexShader);

		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &f, NULL);
		glCompileShader(fragShader);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragShader);
	}

	GLuint getShader() {
		return shaderProgram;
	}
};

