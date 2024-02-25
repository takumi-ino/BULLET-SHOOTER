#pragma once
#include <random>
#include "EnemyBase.h"

class Player;
class CsvLoader;
class EnemyManager;
class StraightBullet;
class HomingBullet;
class EnemyBullet;
class BulletFactory;
class Collision;
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

	EnemyZakoBase(
		const EnemyZakoInfo& data, 
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& _collision
	);

	bool DecreaseHP(int damage, Shared<dxe::Camera> camera);

	bool ShowHpGage_EnemyZako();

	virtual void InitBulletFactoryInstance() {}

	void Render(Shared<dxe::Camera> camera) override;
	bool Update(const float delta_time) override;
	
protected:

	// ���s�e
	void ShotStraightBullet(const float& delta_time);
	void UpdateStraightBullet(const float delta_time);
	void ReloadStraightBulletByTimer(const float& delta_time);

	// �ǔ��e
	void ShotHomingBullet(const float& delta_time);
	void ReloadHomingBulletByTimer(const float& delta_time);
	void UpdateHomingBullet(const float delta_time);

	void AttackPlayer(const float& delta_time);

	void SearchPlayerMovementState(const float delta_time);
	void MoveToRandomInvestigatePos(const float& delta_time);

	void ChasePlayer(const float delta_time);
	void DoRoutineMoves(const float& delta_time);

private:

	const int GetIdleDistance() const { return _IDLE_DISTANCE; }
	const int GetAttackDistance() const { return _ATTACK_DISTANCE; }

public:

	std::list<Shared<EnemyZakoBase>>       _enemyList_ref{};

protected:

	Shared<Collision>                      _collision_ref = nullptr;

	Shared<BulletFactory>                  _bulletFactory = nullptr;

	std::list<Shared<StraightBullet>>      _straight_bullets{};
	std::list<Shared<HomingBullet>>        _homing_bullets{};

	std::deque<Shared<StraightBullet>>     _straightBullet_queue{};
	std::deque<Shared<HomingBullet>>       _homingBullet_queue{};

public:

	static Shared<dxe::Particle> _explode_particle;

	bool      _isAllDead = false; //�G�N���X(�ő吶������)�̎��S�t���O
	bool      _canShot_straightBullet = true;   // ���s�e�����Ă��Ԃ�
	bool      _canShot_HomingBullet = true;   // ���s�e�����Ă�悤�ɂȂ�����
	bool      _isReached_toInvestigatePos = false;

	static bool     _isNoticedPlayer;

	float       _timeCountFrom_noticedPlayer{};

	// CSV���烍�[�h-----------------------
	int         _hp{};
	int         _maxTotalEnemy_SpawnCount{};
	int         _bullet_FireInterval{};
	float       _bullet_MoveSpeed{};
	float       _bullet_reloadTimeInterval{};

protected:

	int         _shotSE_hdl{};
		       
	int         _straightBullet_count{};
	int         _homingBullet_count{};
		      
	float       _randomInvestigateRange_x{};
	float       _randomInvestigateRange_y{};
	float       _randomInvestigateRange_z{};
		       
	float       _minTimeToReach = 2.0f; // �e�̃v���C���[�܂ł̍ŏ����B����
	float       _maxTimeToReach = 3.0f; // �e�̃v���C���[�܂ł̍ő哞�B����
	float       _bulletTurnDelayRate{};

	float       _IDLE_DISTANCE{};       // ���ڂ̎g�p�͋֎~  Getter��ʂ��Ēl���擾����
	float       _ATTACK_DISTANCE{};     // ���ڂ̎g�p�͋֎~  Getter��ʂ��Ēl���擾����
	float       _NOTICE_LIMIT_DURATION{};
	float       _CHANGE_NEXT_BEHAVE_DURATION{};
		
	bool        _isShotStraightBullet = true;
	bool        _isShotHomingBullet = false;
	bool        _isAttacking = false;

	// �����Ԃ̂Ƃ��ɖڎw���n�_
	tnl::Vector3 _investigatePos{};

private:

	std::random_device mt;

	float       _reloadStraightBullet_timeCounter{}; // �����[�h���Ԃ�ǐ�
	float       _reloadHomingBullet_timeCounter{};

	const float _ROTATION_MAX_RANGE = 180.0f;
	const float _STRAIGHTBULLET_LIFETIME_LIMIT = 3.0f;
	const float _HOMINGBULLET_LIFETIME_LIMIT = 5.0f;

	bool        _isTurning = false;
};