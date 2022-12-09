#pragma once
// Camera Object
class Camera {
	protected:
		glm::vec3 cameraPos, worldUp, cameraCenter, F, R, U;
		glm::mat4 viewMatrix, projectionMatrix;

	public:
		Camera() {
			cameraPos = worldUp = cameraCenter = glm::vec3(0, 0, 0);
			viewMatrix = projectionMatrix = glm::mat4(0);
		}

		glm::vec3 getCameraPos() {
			return cameraPos;
		}

		glm::vec3 getWorldUp() {
			return worldUp;
		}

		glm::vec3 getCameraCenter() {
			return cameraCenter;
		}

		glm::vec3 getCameraF() {
			return F;
		}

		glm::vec3 getCameraR() {
			return R;
		}

		glm::vec3 getCameraU() {
			return U;
		}

		glm::mat4 getViewMatrix() {
			return viewMatrix;
		}

		glm::mat4 getProjectionMatrix() {
			return projectionMatrix;
		}

		void setCameraPos(glm::vec3 cameraPos) {
			this->cameraPos = cameraPos;
		}

		void setWorldUp(glm::vec3 worldUp) {
			this->worldUp = worldUp;
		}

		void setCameraCenter(glm::vec3 cameraCenter) {
			this->cameraCenter = cameraCenter;
		}

		void setCameraFRU(glm::vec3 F, glm::vec3 R, glm::vec3 U) {
			this->F = F;
			this->R = R;
			this->U = U;
		}

		void setViewMatrix() {
			this->viewMatrix = glm::lookAt(cameraPos, cameraPos + F, U);
		}

		void setProjectionMatrix(glm::mat4 projectionMatrix) {
			this->projectionMatrix = projectionMatrix;
		}
};

class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 worldUp, glm::mat4 orthoMatrix) {
			this->cameraPos = cameraPos;
			this->cameraCenter = cameraCenter;
			this->worldUp = worldUp;

			F = cameraCenter - cameraPos;
			F = glm::normalize(F);
			R = glm::cross(F, worldUp);
			U = glm::cross(R, F);

			viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);
			projectionMatrix = orthoMatrix;
		}
		
		OrthographicCamera() {
			cameraPos = worldUp = cameraCenter = glm::vec3(0, 0, 0);
			viewMatrix = projectionMatrix = glm::mat4(0);
		}
};

class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 worldUp, glm::mat4 perspectiveMatrix) {
			this->cameraPos = cameraPos;
			this->cameraCenter = cameraCenter;
			this->worldUp = worldUp;

			F = cameraCenter - cameraPos;
			F = glm::normalize(F);
			R = glm::cross(F, worldUp);
			U = glm::cross(R, F);

			viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);
			projectionMatrix = perspectiveMatrix;
		}

		PerspectiveCamera() {
			cameraPos = worldUp = cameraCenter = glm::vec3(0, 0, 0);
			viewMatrix = projectionMatrix = glm::mat4(0);
		}
};