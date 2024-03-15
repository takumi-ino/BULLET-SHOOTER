#pragma once
#include "../EnemyZakoBase.h"


class EnemyZakoBox final : public EnemyZakoBase
{
public:

	EnemyZakoBox() {}
	~EnemyZakoBox() override {}


	EnemyZakoBox(
		const EnemyZakoInfo& info,
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision
	);

	// ������---------------------------------------------------------------------------------
	void InitBulletFactoryInstance() override;
	void SetMeshInfo() override;
};