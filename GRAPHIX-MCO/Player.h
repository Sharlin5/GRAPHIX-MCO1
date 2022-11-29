#pragma once
class Player {
	private:
		Model player;

	public:
		Player(glm::mat4 identityMatrix) {
			player = Model("SubLow0Smooth", 0);
			player.setPos(0, 0, -10.f);
			player.setRot(0, 0, 0, 90.f);
			player.setScale(100.f, 100.f, 100.f);
			player.initTransformationMatrix(identityMatrix);
		}

		Model getPlayer() {
			return player;
		}
};
