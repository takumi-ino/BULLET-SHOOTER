#include "../../DxLibEngine.h"
#include "ShiningStar.h"
#include <random>


void ShiningStar::Render() {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> rateDistribution_x(0, (float)DXE_WINDOW_HEIGHT);
	std::uniform_real_distribution<float> rateDistribution_y(0, (float)DXE_WINDOW_WIDTH);
	std::uniform_real_distribution<float> rateDistribution_z(0,1.0f);

	float randomX = rateDistribution_x(gen);
	float randomY = rateDistribution_y(gen);
	float randomZ = rateDistribution_z(gen);

	S_ShiningStar pos[1000];

	for (int i = 0; i < 1000; i++) {

		pos[i].z -= 0.00001;
		if (pos[i].z <= 0.0f) { pos[i].z = 1.0f; }

		pos[i].x = star[i].x = randomX;
		pos[i].y = star[i].y = randomY;

		DrawCircleAA((pos[i].x / pos[i].z) * 2.0, (pos[i].y / pos[i].z) * 2.0, 1, 10, GetColor(255,255,0));
	}
}