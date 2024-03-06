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

	void Render(Shared<dxe::Camera> _mainCamera) override;
};