#pragma once
#include "../EnemyZakoBase.h"

namespace inl {

	class EnemyZakoBox final : public EnemyZakoBase
	{
	public:

		EnemyZakoBox() {}
		~EnemyZakoBox() override {}


		EnemyZakoBox(const EnemyZakoInfo& info);

		// ������---------------------------------------------------------------------------------
		void InitBulletFactoryInstance() override;
		void SetMeshInfo() override;
	};
}