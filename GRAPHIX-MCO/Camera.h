#pragma once
// Camera Object
class Camera {
private:
	glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 10.f);
	glm::vec3 WorldUp = glm::vec3(0, 1.5f, 0);
	glm::vec3 cameraCenter = glm::vec3(0, 0.0f, 0);
	glm::mat4 viewMatrix;

public:
	Camera() {

	}

	glm::mat4 getView() {
		return viewMatrix;
	}
};
