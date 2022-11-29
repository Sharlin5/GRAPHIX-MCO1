#pragma once
class Light {
	protected:
		int intensity;
		float ambientStr, specStr, specPhong;
		glm::vec3 lightColor, lightPos, ambientColor;

	public:
		Light() {
			intensity = 0;
		
			ambientStr = 0.1f;
			specStr = 0.5f;
			specPhong = 16.f;

			lightColor = ambientColor = glm::vec3(1.f, 1.f, 1.f);
			lightPos = glm::vec3(0, 0, 0);
		}

		void linkShader(GLuint shaderProgram, Camera cam) {
			GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
			glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
			GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
			glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));
			GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
			glUniform1f(ambientStrAddress, ambientStr);
			GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
			glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));
			GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
			glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cam.getCameraPos()));
			GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
			glUniform1f(specStrAddress, specStr);
			GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
			glUniform1f(specPhongAddress, specPhong);
			GLuint intensityAddress = glGetUniformLocation(shaderProgram, "intensity");
			glUniform1f(intensityAddress, intensity);
		}

		int getIntensity() {
			return intensity;
		}

		float getAmbientStr() {
			return ambientStr;
		}

		float getSpecStr() {
			return specStr;
		}

		float getSpecPhong() {
			return specPhong;
		}

		glm::vec3 getLightColor() {
			return lightColor;
		}

		glm::vec3 getLightPos() {
			return lightPos;
		}

		glm::vec3 getAmbientColor() {
			return ambientColor;
		}

		void setIntensity(int intensity) {
			this->intensity = intensity;
		}

		void setAmbientStr(float ambientStr) {
			this->ambientStr = ambientStr;
		}

		void setSpecStr(float specStr) {
			this->specStr = specStr;
		}

		void setSpecPhong(float specPhong) {
			this->specPhong = specPhong;
		}

		void setLightColor(glm::vec3 lightColor) {
			this->lightColor = lightColor;
		}

		void setLightPos(glm::vec3 lightPos) {
			this->lightPos = lightPos;
		}

		void setAmbientColor(glm::vec3 ambientColor) {
			this->ambientColor = ambientColor;
		}
};


class DirLight : public Light {
	public:
		DirLight(glm::vec3 lightPos, int intensity) {
			this->intensity = intensity;

			ambientStr = 0.1f;
			specStr = 0.5f;
			specPhong = 16.f;

			lightColor = ambientColor = glm::vec3(1.f, 1.f, 1.f);
			this->lightPos = lightPos;
		}
};

class PointLight : public Light {
	public:
		PointLight(glm::vec3 lightPos, int intensity) {
			this->intensity = intensity;

			ambientStr = 0.1f;
			specStr = 0.5f;
			specPhong = 16.f;

			lightColor = ambientColor = glm::vec3(1.f, 1.f, 1.f);
			this->lightPos = lightPos;
		}
};