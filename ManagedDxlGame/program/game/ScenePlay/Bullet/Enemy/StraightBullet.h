#pragma once
#include "EnemyBullet.h"

class BulletFactory;

namespace inl {

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
		~StraightBullet() override {}

		void Render() override;
	};
}