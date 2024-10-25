#include "EnemyBossBase.h"
#include "../Player/Player.h"
#include "../../../Loader/CsvLoader.h"
#include "../../../Manager/Score/ScoreManager.h"
#include "../game/ScenePlay/Collision/Collision.h"
#include "../../Bullet/Enemy/EnemyBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/BulletHell.h"
#include "../game/Utility/FilePathChecker.h"
#include "../game/Utility//CustomException.h"

namespace {

	//　名前---------------------------------------------------------------------
	const int _BOSSNAME_X{ 430 };
	const int _BOSSNAME_Y{ 700 };

	//　Indicator-----------------------------------------------------------------
	const float _BOSS_HP_INDICATOR_X{ 600.0f };
	const float _BOSS_HP_INDICATOR_OFFSET_X{ 30.0f };
	const float _BOSS_HP_INDICATOR_Y{ 655.0f };
	const float _BOSS_HP_INDICATOR_WIDTH{ 12.0f };
	const float _BOSS_HP_INDICATOR_HEIGHT{ 12.0f };
	const int   _BOSS_HP_INDICATOR_EDGE_NUM{ 4 };

	//　HP ( BOSS_HP_GAGE_X2 )のみ変動するためローカル変数で定義)------------------
	const float _BOSS_HP_GAGE_X1{ 430.0f };
	const float _BOSS_HP_GAGE_Y1{ 675.0f };
	const float _BOSS_HP_GAGE_Y2{ 695.0f };

	//　行動可能範囲---------------------------------------------------------------
	const float _MOVABLE_RANGE_FROM_ORIGIN{ 1000.0f };

	//　ボスとプレイヤー間の基準距離-----------------------------------------------
	const float _DISTANCE_THRESHOLD{ 500.0f };
}

namespace inl {


	// ボスエネミーデータ読み取り
	EnemyBossBase::EnemyBossBase(const EnemyBossInfo& data)
	{

		_bossHp.clear(); //　ボスHPをリセット
		while (!_remainingLife_indicator.empty()) {
			_remainingLife_indicator.pop();
		}

		const int maxHpSize = 4;
		for (int i = 0; i < maxHpSize; i++) {

			_bossHp.push_back(data._hp);  //　ボスHPを初期化
			_remainingLife_indicator.push(_bossHp);
		}

		_id = data._id;                                    // ID
		_name = data._name;								   // 名前
		_MAX_HP = data._hp;								   // 最大HP
		_scale = data._scale;							   // サイズ
		_enemyMoveSpeed = data._enemyMoveSpeed;			   // 移動スピード
		_maxBulletSpawnCount = data._maxBulletSpawnCount;  // 弾の最大生成数


		Shared<CustomException> cus = std::make_shared<CustomException>();
		auto warpSound = cus->TryLoadSound("sound/se/enemy/warp.mp3","EnemyBossBase::EnemyBossBase()");

		_warppingSound = warpSound;
	}


	void EnemyBossBase::ActKeepDistanceToPlayer(const float deltaTime) {

		// 敵とプレイヤーの距離
		float distance_from_player = GetDistanceToPlayer();
		// 敵とプレイヤーの距離の差分
		tnl::Vector3 differenceVector = ScenePlay::GetInstance()->_player->_mesh->pos_ - _mesh->pos_;

		tnl::Vector3 moveDirection;

		//　基準距離よりも近い
		if (distance_from_player < _DISTANCE_THRESHOLD) {

			//　プレイヤーから離れる
			moveDirection.x = differenceVector.x * -1.f;
			moveDirection.z = differenceVector.z * -1.f;
		}
		//　基準距離よりも離れてる
		else if (distance_from_player > _DISTANCE_THRESHOLD) {

			moveDirection.x = differenceVector.x;
			moveDirection.z = differenceVector.z;
		}
		else {
			moveDirection = tnl::Vector3::Cross(differenceVector, tnl::Vector3::up);
		}

		ClampMovableRange(moveDirection);

		// Y座標はプレイヤーに合わせるように動く
		float diffY = ScenePlay::GetInstance()->_player->GetPos().y - _mesh->pos_.y;
		moveDirection.y = diffY;

		moveDirection.normalize();
		_mesh->pos_ += moveDirection * _enemyMoveSpeed * deltaTime;
	}



	void EnemyBossBase::WarpToRandomPos(const float deltaTime)
	{
		_warpToRandPosTimer += deltaTime;

		//　一定間隔でランダムな地点にワープ
		if (_warpToRandPosTimer > _WARPING_DURATION) {

			tnl::Vector3 targetPos = _mesh->pos_ + GetRandomPosition();

			_mesh->pos_ += (targetPos - _mesh->pos_);

			PlaySoundMem(_warppingSound, DX_PLAYTYPE_BACK);

			_warpToRandPosTimer = 0.f;
		}
	}


	void EnemyBossBase::ClampMovableRange(tnl::Vector3& moveDirection)
	{
		float distance_from_origin = (tnl::Vector3{ 0,0,0 } - _mesh->pos_).length();

		// 原点から離れすぎてしまったらフィールド内に戻るようにする
		if (distance_from_origin > _MOVABLE_RANGE_FROM_ORIGIN) {

			tnl::Vector3 direction = tnl::Vector3{ 0,0,0 } - _mesh->pos_;
			direction.normalize();

			moveDirection = direction;
		}
	}


	void EnemyBossBase::CheckCollision_BulletHellBulletsAndPlayer_DRY(std::vector<Shared<EnemyBullet>>& bulletVector) {

		for (auto& blt : bulletVector) {

			if (ScenePlay::GetInstance()->_collision->CheckCollision_BulletHellBulletsAndPlayer(blt, ScenePlay::GetInstance()->_player)) {

				// 敵の攻撃力からプレイヤーの防御力を引いた分 HPを削る
				if (ScenePlay::GetInstance()->_player->DecreaseHP(_at - ScenePlay::GetInstance()->_player->GetDEF())) {

					ScenePlay::GetInstance()->_player->_isInvincible = true;
					ScenePlay::GetInstance()->_player->PlayDamageHitSE();      // ダメージ音
				}
			}

			blt->_isActive = false;
		}
	}


	void EnemyBossBase::DecreaseBossHP(int damage) {

		if (!_bossHp.empty()) {

			damage = max(damage, 1);

			_bossHp.front() -= damage;

			if (_bossHp.front() <= 0) {
				_bossHp.pop_front();
				_remainingLife_indicator.pop();
			}
		}
		else {
			ScoreManager::GetInstance().AddKillBonus(10000);
			_isDead = true;
		}
	}


	bool EnemyBossBase::ShowHpGage_EnemyBoss() {

		if (_bossHp.empty()) {
			return false;
		}

		float BOSS_HP_GAGE_X2 = 860.0f;

		//　HPゲージの幅
		float gageWidth = abs(BOSS_HP_GAGE_X2 - _BOSS_HP_GAGE_X1);  //　この値は変わらない

		//　HPゲージの1ポイントあたりの幅
		float average = (_MAX_HP > 0) ? gageWidth / _MAX_HP : 0;    //  この値は変わらない

		//　平均と現在HPを掛けた値を Ｘ１に足す
		BOSS_HP_GAGE_X2 = _BOSS_HP_GAGE_X1 + static_cast<int>(average * _bossHp.front());

		DrawBoxAA(
			_BOSS_HP_GAGE_X1,
			_BOSS_HP_GAGE_Y1,
			860,
			_BOSS_HP_GAGE_Y2,
			GetColor(255, 255, 255),
			true
		);

		DrawBoxAA(
			_BOSS_HP_GAGE_X1,
			_BOSS_HP_GAGE_Y1,
			BOSS_HP_GAGE_X2,
			_BOSS_HP_GAGE_Y2,
			GetColor(255, 0, 0),
			true
		);

		RenderBossName();       // 名前
		RenderBossRemainLife(); //　ＨＰコア

		return true;
	}



	void EnemyBossBase::RenderBossRemainLife() {

		for (int i = 0; i < _remainingLife_indicator.size(); i++) {

			DrawOvalAA(
				_BOSS_HP_INDICATOR_X + (i * _BOSS_HP_INDICATOR_OFFSET_X),
				_BOSS_HP_INDICATOR_Y,
				_BOSS_HP_INDICATOR_WIDTH,
				_BOSS_HP_INDICATOR_HEIGHT,
				_BOSS_HP_INDICATOR_EDGE_NUM,
				GetColor(230, 0, 0),
				true
			);
		}
		for (int i = 0; i < 4; i++) {

			DrawOvalAA(
				_BOSS_HP_INDICATOR_X + (i * _BOSS_HP_INDICATOR_OFFSET_X),
				_BOSS_HP_INDICATOR_Y,
				_BOSS_HP_INDICATOR_WIDTH,
				_BOSS_HP_INDICATOR_HEIGHT,
				_BOSS_HP_INDICATOR_EDGE_NUM,
				GetColor(230, 0, 0),
				false
			);
		}
	}


	void EnemyBossBase::RenderBossName() {

		SetFontSize(DEFAULT_FONT_SIZE);
		DrawString(_BOSSNAME_X, _BOSSNAME_Y, _name.c_str(), -1);
	}
}