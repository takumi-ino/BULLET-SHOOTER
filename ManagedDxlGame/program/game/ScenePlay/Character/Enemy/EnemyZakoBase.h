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

	// ���s�e
	virtual void ShotStraightBullet(std::list<Shared<EnemyBullet>> bullet) {};
	virtual void UpdateStraightBullet(const float delta_time) {}
	virtual void UpdateHomingBullet(const float delta_time) {}
	virtual const std::list<Shared<StraightBullet>>& GetStraightBullets() { return std::list<Shared<StraightBullet>>(); }

	// �ǐՒe
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

	bool     _isAllDead = false; //�G�N���X(�ő吶������)�̎��S�t���O
	bool     _canShotStraightBullet = true;   // ���s�e�����Ă��Ԃ�
	bool     _canShotHomingBullet = true;   // ���s�e�����Ă�悤�ɂȂ�����
	bool     _isReachedToInvestigatePos = false;

	static bool     _isNoticedPlayer;

	float   _timeFrom_noticedPlayer{};

	// CSV���烍�[�h-----------------------
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
	// �����Ԃ̂Ƃ��ɖڎw���n�_
	tnl::Vector3 _investigatePos{};

private:

	bool _isTurning = false;
};