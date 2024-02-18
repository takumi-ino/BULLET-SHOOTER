#pragma once
#include "EnemyBase.h"

class Player;
class CsvLoader;
class EnemyBullet;
class EnemyManager;
class StraightBullet;
class HomingBullet;
class BulletFactory;
struct EnemyZakoInfo;

class EnemyZakoBase : public EnemyBase
{
public:

	enum class BEHAVE {

		Stop,
		Moving,
		Turn
	};

	BEHAVE _behave = EnemyZakoBase::BEHAVE::Moving;

	EnemyZakoBase() {}

	EnemyZakoBase(const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera);

	bool DecreaseHP(int damage, Shared<dxe::Camera> camera);

	virtual bool ShowHpGage_EnemyZako();

	virtual void InitBulletFactoryInstance() {}

protected:

	// 直行弾
	virtual void ShotStraightBullet(std::list<Shared<EnemyBullet>> bullet) {};
	virtual void UpdateStraightBullet(const float delta_time) {}
	virtual void UpdateHomingBullet(const float delta_time) {}
	virtual const std::list<Shared<StraightBullet>>& GetStraightBullets() { return std::list<Shared<StraightBullet>>(); }

	// 追跡弾
	virtual void ShotHomingBullet() {}

	void SearchPlayerMovementState(const float delta_time);
	void MoveToRandomInvestigatePos(const float& delta_time);

	void ChasePlayer(const float delta_time);

public:

	std::list<Shared<EnemyZakoBase>>       _enemy_list_ref{};

protected:

	Shared<EnemyManager>                   _enemyManager = nullptr;

	Shared<BulletFactory>                  _bulletFactory = nullptr;

	std::list<Shared<StraightBullet>>      _straight_bullets{};
	std::list<Shared<HomingBullet>>        _homing_bullets{};

	std::deque<Shared<StraightBullet>>     _straightBullet_queue{};
	std::deque<Shared<HomingBullet>>       _homingBullet_queue{};

public:

	static Shared<dxe::Particle> _explode_particle;

	bool     _isAllDead = false; //敵クラス(最大生成数分)の死亡フラグ
	bool     _canShotStraightBullet = true;   // 直行弾が撃てる状態か
	bool     _canShotHomingBullet = true;   // 直行弾が撃てるようになったか
	bool     _isReachedToInvestigatePos = false;

	static bool     _isNoticedPlayer;

	float   _timeFrom_noticedPlayer{};

	// CSVからロード-----------------------
	int      _hp{};
	int      _maxTotalEnemySpawnCount{};
	int      _bulletFireInterval{};
	int      _bulletMoveSpeed{};
	float    _reloadTimeInterval{};

protected:

	int   _shotSE_hdl{};

	int   _straight_bullet_count{};
	int   _homing_bullet_count{};

	float _randomInvestigateRange_x{};
	float _randomInvestigateRange_y{};
	float _randomInvestigateRange_z{};

	bool  _isShotStraightBullet = true;
	bool  _isShotHomingBullet = false;

	tnl::Vector3 prev_pos;
	// 巡回状態のときに目指す地点
	tnl::Vector3 _investigatePos{};

private:

	bool _isTurning = false;
};