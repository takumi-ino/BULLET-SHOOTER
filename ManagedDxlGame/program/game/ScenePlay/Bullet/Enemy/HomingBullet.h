#pragma once
#include "EnemyBullet.h"

namespace inl {

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
		~HomingBullet() override {}

		void Render() override;
	};
}