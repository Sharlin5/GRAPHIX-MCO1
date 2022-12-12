#pragma once
class Player {
	private:
		Model player;

	public:
		Player() {

		}

		Player(glm::mat4 identityMatrix) {
			player = Model("Odyssey_OBJ", 0);
			player.setPos(-10.f, 0.f, 0.f);
			player.setRot(0, 1.f, 0, 90.f);
			player.setScale(0.05f, 0.05f, 0.05f);
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
