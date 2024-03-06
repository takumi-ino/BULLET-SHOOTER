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

	void Render(Shared<dxe::Camera> _mainCamera) override;
};