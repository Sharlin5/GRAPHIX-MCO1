#pragma once
//for shader creation
class Shader {
private:
	GLuint shaderProgram;
public:
	Shader(std::string vert, std::string frag) {
        std::fstream vertSrc(vert);
        std::stringstream vertBuff;
        vertBuff << vertSrc.rdbuf();
        std::string vertString = vertBuff.str();
        const char* v = vertString.c_str();

        std::fstream fragSrc(frag);
        std::stringstream fragBuff;
        fragBuff << fragSrc.rdbuf();
        std::string fragString = fragBuff.str();
        const char* f = fragString.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &v, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &f, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
	}

    GLuint getShader() {
        return shaderProgram;
    }

};