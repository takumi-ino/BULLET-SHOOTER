#pragma once
#include "EnemyBase.h"

class Player;
class CsvLoader;
class BulletHell;
class EnemyBullet;
class EnemyManager;
struct EnemyZakoInfo;

class EnemyZakoBase : public EnemyBase
{
public:

	enum class MOVE_BEHAVE {

		Stop,
		Moving,
		Turn
	};

	MOVE_BEHAVE _behave = EnemyZakoBase::MOVE_BEHAVE::Moving;

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

public:

	std::list<Shared<EnemyZakoBase>> _enemy_list_ref;

protected:

	Shared<EnemyManager>             _enemyManager = nullptr;

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

	// 巡回状態のときに目指す地点
	tnl::Vector3 _investigatePos{};

	float _randomInvestigateRange_x{};
	float _randomInvestigateRange_y{};
	float _randomInvestigateRange_z{};

private:

	bool _isTurning = false;
};