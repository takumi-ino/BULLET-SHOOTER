#pragma once
#include "../EnemyZakoBase.h"

namespace inl {

	class EnemyZakoDome final : public EnemyZakoBase
	{
	public:

		EnemyZakoDome() {}
		~EnemyZakoDome() override {}


		EnemyZakoDome(const EnemyZakoInfo& info);

		// èâä˙âª---------------------------------------------------------------------------------
		void InitBulletFactoryInstance() override;
		void SetMeshInfo() override;
	};
}