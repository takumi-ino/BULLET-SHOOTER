#pragma once
#include "../EnemyZakoBase.h"

namespace inl {

	class EnemyZakoDome final : public EnemyZakoBase
	{
	public:

		EnemyZakoDome() {}
		~EnemyZakoDome() override {}


		EnemyZakoDome(
			const EnemyZakoInfo& info,
			const Shared<Player>& player,
			const Shared<dxe::Camera>& camera,
			const Shared<Collision>& collision
		);

		// ������---------------------------------------------------------------------------------
		void InitBulletFactoryInstance() override;
		void SetMeshInfo() override;
	};
}