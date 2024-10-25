#pragma once
#include "../EnemyZakoBase.h"

namespace inl {

	class EnemyZakoBox final : public EnemyZakoBase
	{
	public:

		EnemyZakoBox() {}
		~EnemyZakoBox() override {}


		EnemyZakoBox(const EnemyZakoInfo& info);

		// èâä˙âª---------------------------------------------------------------------------------
		void InitBulletFactoryInstance() override;
		void SetMeshInfo() override;
	};
}