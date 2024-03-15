#pragma once
#include "../EnemyZakoBase.h"

namespace inl {

	class EnemyZakoCylinder final : public EnemyZakoBase
	{
	public:

		EnemyZakoCylinder() {}
		~EnemyZakoCylinder() override {}

		EnemyZakoCylinder(
			const EnemyZakoInfo& info,
			const Shared<Player>& player,
			const Shared<dxe::Camera>& camera,
			const Shared<Collision>& collision
		);

		// èâä˙âª---------------------------------------------------------------------------------
		void InitBulletFactoryInstance() override;
		void SetMeshInfo() override;
	};
}