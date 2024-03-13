#pragma once
#include "../EnemyZakoBase.h"


class EnemyZakoDome final : public EnemyZakoBase
{
public:

	EnemyZakoDome(){}

	EnemyZakoDome(
		const EnemyZakoInfo& info,
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision
	);

	// èâä˙âª---------------------------------------------------------------------------------
	void InitBulletFactoryInstance() override;
	void SetMeshInfo() override;
};