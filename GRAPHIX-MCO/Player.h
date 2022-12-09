#pragma once
class Player {
	private:
		Model player;

	public:
		Player() {

		}

		Player(glm::mat4 identityMatrix) {
			player = Model("SubLow0Smooth", 0);
			player.setPos(0, 0, -10.f);
			player.setRot(0, 0, 0, 90.f);
			player.setScale(1.f, 1.f, 1.f);
			player.initTransformationMatrix(identityMatrix);
		}

		Model getPlayer() {
			return player;
		}

		void movePlayer(float pos_x, float pos_y, float pos_z) {
			player.setPos(pos_x, pos_y, pos_z);
		}

		void rotPlayer(float rot_x, float rot_y, float rot_z, float theta) {
			player.setRot(rot_x, rot_y, rot_z, theta);
		}
};
