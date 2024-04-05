#pragma once
#include "EnemyBase.h"
#include "../game/Utility/CustomException.h"

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

		// 初期化----------------------------------------------------------
		virtual void InitBulletHellInstance() {}

		// ダメージ--------------------------------------------------------
		void DecreaseBossHP(int damage);

	protected:

		// HP------------------------------------------------------------
		bool ShowHpGage_EnemyBoss();
		void RenderBossRemainLife();

		// 名前-----------------------------------------------------------
		void RenderBossName();
		virtual void RenderBossSpellCardName() {}

		// 当たり判定-----------------------------------------------------
		void CheckCollision_BulletHellBulletsAndPlayer_DRY(
			std::vector<Shared<EnemyBullet>>& bulletVector
		);

		// 挙動------------------------------------------------------------
		void ActKeepDistanceToPlayer(const float deltaTime);        // プレイヤーと一定距離を保つ

		void WarpToRandomPos(const float deltaTime);				// 一定時間でランダムワープ

		void ClampMovableRange(tnl::Vector3& moveDirection);		// 移動範囲制限

	public:

		std::deque<int>                  _bossHp{};
		Shared<BulletHell>               _bulletHell = nullptr;

	protected:

		Shared<Collision>                _collision_ref = nullptr;

	protected:

		int       _MAX_HP{};
		float     _WARPING_DURATION{};

	private:

		// ボス残りHP。4つの赤い ダイヤの形をしたもの
		std::stack<std::deque<int>> _remainingLife_indicator;

		// ワープするタイミングをタイマーで制御
		float _warpToRandPosTimer{};
	};
}