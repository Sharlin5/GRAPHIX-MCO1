#pragma once
class Light {
private:
	float lightPos;
	float specStr, specPhong;
	float ambientStr;
	glm::vec3 ambientColor, lightColor;

public:
	Light() {

	}
};
