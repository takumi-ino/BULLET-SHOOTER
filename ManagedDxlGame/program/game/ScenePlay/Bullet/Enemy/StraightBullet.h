#pragma once
#include "EnemyBullet.h"

class BulletFactory;

class StraightBullet : public EnemyBullet
{
public:

	enum class USER {

		None,
		ZakoBox,
		ZakoDome,
		ZakoCylinder
	};


	StraightBullet();
	~StraightBullet() override {}

	void Render(const Shared<dxe::Camera> mainCamera) override;
};