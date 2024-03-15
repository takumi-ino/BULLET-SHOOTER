#pragma once
#include "EnemyBullet.h"


class HomingBullet : public EnemyBullet
{
public:

	enum class USER {

		None,
		ZakoBox,
		ZakoDome,
		ZakoCylinder
	};


	HomingBullet();
	~HomingBullet() override {}

	void Render(const Shared<dxe::Camera> mainCamera) override;
};