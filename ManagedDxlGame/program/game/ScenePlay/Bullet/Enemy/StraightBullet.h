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

	void CheckLifeTimeDistance(const Shared<StraightBullet>& straightBullet);

	void Render(Shared<dxe::Camera> _mainCamera) override;
	void Update(const float deltaTime) override;

public:

	Shared<dxe::Mesh> _mesh = nullptr;

};