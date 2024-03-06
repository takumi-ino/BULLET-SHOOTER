#pragma once
#include <random>
#include "EnemyBase.h"

class Player;
class StraightBullet;
class HomingBullet;
class EnemyBullet;
class BulletFactory;
class Collision;
struct EnemyZakoInfo;

class EnemyZakoBase : public EnemyBase
{
public:

	// Enum------------------------------------------------------------------
	enum class BEHAVE {

		Stop,
		Moving,
		Turn
	};

	// コンストラクタ・デストラクタ------------------------------------------
	EnemyZakoBase() {}

	EnemyZakoBase(
		const EnemyZakoInfo& data,
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& _collision
	);

	// Init ------------------------------------------------------------------
	virtual void InitBulletFactoryInstance() {}

	// HP ------------------------------------------------------------------
	bool DecreaseHP(int damage, Shared<dxe::Camera> camera);

	// 描画・更新 ------------------------------------------------------------
	void Render(Shared<dxe::Camera> camera) override;
	bool Update(const float delta_time) override;

protected:

	// 直行弾 ------------------------------------------------------------
	void ShotStraightBullet(const float& delta_time);
	void ReloadStraightBulletByTimer(const float& delta_time);
	void UpdateStraightBullet(const float delta_time);

	// 追尾弾 ------------------------------------------------------------
	void ShotHomingBullet(const float& delta_time);
	void ReloadHomingBulletByTimer(const float& delta_time);
	void UpdateHomingBullet(const float delta_time);

	// プレイヤーへ攻撃 ---------------------------------------------------
	void AttackPlayer(const float& delta_time);

	// 挙動	----------------------------------------------------------------------------------
	void DoRoutineMoves(const float& delta_time);             // 追跡、攻撃、待機を使い分ける

	void SearchPlayerMovementState(const float delta_time);   // 待機状態中、STOP・ MOVE・ TURNのステートによって行動
	void MoveToRandomInvestigatePos(const float& delta_time); // 待機状態中、ランダム移動

	void ChasePlayer(const float delta_time);                 // プレイヤー追跡

private:

	bool ShowHpGage_EnemyZako();

	const int GetIdleDistance() const { return _IDLE_DISTANCE; }
	const int GetAttackDistance() const { return _ATTACK_DISTANCE; }

protected:

	Shared<Collision>                      _collision_ref = nullptr;

	Shared<BulletFactory>                  _bulletFactory = nullptr;

	std::list<Shared<StraightBullet>>      _straight_bullets{};
	std::deque<Shared<StraightBullet>>     _straightBullet_queue{};

	std::list<Shared<HomingBullet>>        _homing_bullets{};
	std::deque<Shared<HomingBullet>>       _homingBullet_queue{};

public:

	// 敵を撃破したときに発生する爆発エフェクト---------------------------------------------
	static Shared<dxe::Particle> _explode_particle;

	// ランダムに決定した調査地点に辿り着いたか---------------------------------------------
	bool                         _isReached_toInvestigatePos{ false };

	// プレイヤーに気付いた状態 ------------------------------------------------------------
	static bool                  _isNoticedPlayer;
	float                        _timeCountFrom_noticedPlayer{};

	// CSVからロード-----------------------
	int         _hp{};                         // HP
	int         _maxTotalEnemy_spawnCount{};   // 敵の総数
	int         _bullet_fireInterval{};        // 弾の発射間隔
	float       _bullet_moveSpeed{};           // 弾の移動スピード
	float       _bullet_reloadTimeInterval{};  // リロード間隔

protected:

	//　発射SE---------------------------------
	int         _shotSE_hdl{};

	//　弾を撃つ間隔を管理するためのカウンター----------------------------------
	int         _straightBullet_count{};
	int         _homingBullet_count{};

	//　追尾弾で使用する値---------------------------------------------------------------------
	const float _minTimeToReach{ 2.0f }; // 弾のプレイヤーまでの最小到達時間
	const float _maxTimeToReach{ 3.0f }; // 弾のプレイヤーまでの最大到達時間
	float       _bulletTurnDelayRate{};  // 弾の軌道修正の遅延率

	//　距離や期間などの固定値（派生クラスで使用）----------------------------------------------
	float       _IDLE_DISTANCE{};       // 直接の使用は禁止  Getterを通して値を取得する
	float       _ATTACK_DISTANCE{};     // 直接の使用は禁止  Getterを通して値を取得する
	float       _NOTICE_LIMIT_DURATION{};
	float       _CHANGE_NEXT_BEHAVE_DURATION{};

	//　ランダムに決定した調査地点のベクトル成分------------------------------------------------
	float       _randomInvestigateRange_x{};
	float       _randomInvestigateRange_y{};
	float       _randomInvestigateRange_z{};

	//　フラグ----------------------------------------------------------------------------------
	bool        _isShotStraightBullet{ true }; // 直行弾を撃っているか
	bool        _isShotHomingBullet{ false };  // 追尾弾を撃っているか
	bool        _isAttacking{ false };         // 攻撃中か

	// 巡回状態のときに目指す地点---------------------------------------------------------------
	tnl::Vector3 _investigatePos{};

private:

	// Enum変数----------------------------------------
	BEHAVE _behave = EnemyZakoBase::BEHAVE::Moving;  //敵の挙動管理

	// ランダムデバイス--------------------------------
	std::random_device mt;

	// リロード時間を追跡------------------------------
	float       _reloadStraightBullet_timeCounter{};
	float       _reloadHomingBullet_timeCounter{};

	//　-----------------------------------------
	bool        _isTurning{ false }; // 旋回中か
};