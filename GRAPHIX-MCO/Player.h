#pragma once
class Player {
private:
	float pos_x, pos_y, pos_z; //position
	float rot_x, rot_y, rot_z, theta; // rotation
	float scale_x, scale_y, scale_z; // scale
	Model player;

public:
	Player() {
		pos_x = pos_y = pos_z = 0.0;
		rot_x = rot_y = rot_z = theta = 0.0;
		scale_x = scale_y = scale_z = 0.0;
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
