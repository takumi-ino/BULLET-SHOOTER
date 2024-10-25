#pragma once
#include "../EnemyZakoBase.h"

namespace inl {

	class EnemyZakoCylinder final : public EnemyZakoBase
	{
	public:

		EnemyZakoCylinder() {}
		~EnemyZakoCylinder() override {}

		EnemyZakoCylinder(const EnemyZakoInfo& info);

		// èâä˙âª---------------------------------------------------------------------------------
		void InitBulletFactoryInstance() override;
		void SetMeshInfo() override;
	};
}