#pragma once
#include "EnemyBase.h"

class Player;

namespace inl {
	class BulletHell;
	class EnemyBullet;
class Collision;
struct EnemyBossInfo;


	class EnemyBossBase : public EnemyBase
	{
	public:

		EnemyBossBase() {}
		~EnemyBossBase() override {}


		EnemyBossBase(
			const EnemyBossInfo& data,
			const Shared<Player>& player,
			const Shared<dxe::Camera>& camera,
			const Shared<Collision>& collision
		);

		// INIT----------------------------------------------------------
		virtual void InitBulletHellInstance() {}

		// DAMAGE--------------------------------------------------------
		void DecreaseBossHP(int damage);

	protected:

		// HP------------------------------------------------------------
		bool ShowHpGage_EnemyBoss();
		void RenderBossRemainLife();

		// NAME-----------------------------------------------------------
		void RenderBossName();
		virtual void RenderBossSpellCardName() {}

		// HIT------------------------------------------------------------
		void CheckCollision_BulletHellBulletsAndPlayer_DRY(std::vector<Shared<EnemyBullet>>& bulletVector);

		// ACT------------------------------------------------------------
		void ActKeepDistanceToPlayer(const float& deltaTime);

	public:

		std::deque<int>                  _bossHp{};
		Shared<BulletHell>               _bulletHell = nullptr;

	protected:

		Shared<Collision>                _collision_ref = nullptr;

	protected:

		int       _MAX_HP{};
		float     _WARPING_DURATION{};

	private:

		// ボス残りHP。4つの赤い ダイヤの形をしたもの-------------------------
		std::stack<std::deque<int>> _remainingLife_indicator;

		// ワープするタイミングをタイマーで制御-------------------------------
		float _warpToRandPosTimer{};
	};
}