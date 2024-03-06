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

	// �R���X�g���N�^�E�f�X�g���N�^------------------------------------------
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

	// �`��E�X�V ------------------------------------------------------------
	void Render(Shared<dxe::Camera> camera) override;
	bool Update(const float delta_time) override;

protected:

	// ���s�e ------------------------------------------------------------
	void ShotStraightBullet(const float& delta_time);
	void ReloadStraightBulletByTimer(const float& delta_time);
	void UpdateStraightBullet(const float delta_time);

	// �ǔ��e ------------------------------------------------------------
	void ShotHomingBullet(const float& delta_time);
	void ReloadHomingBulletByTimer(const float& delta_time);
	void UpdateHomingBullet(const float delta_time);

	// �v���C���[�֍U�� ---------------------------------------------------
	void AttackPlayer(const float& delta_time);

	// ����	----------------------------------------------------------------------------------
	void DoRoutineMoves(const float& delta_time);             // �ǐՁA�U���A�ҋ@���g��������

	void SearchPlayerMovementState(const float delta_time);   // �ҋ@��Ԓ��ASTOP�E MOVE�E TURN�̃X�e�[�g�ɂ���čs��
	void MoveToRandomInvestigatePos(const float& delta_time); // �ҋ@��Ԓ��A�����_���ړ�

	void ChasePlayer(const float delta_time);                 // �v���C���[�ǐ�

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

	// �G�����j�����Ƃ��ɔ������锚���G�t�F�N�g---------------------------------------------
	static Shared<dxe::Particle> _explode_particle;

	// �����_���Ɍ��肵�������n�_�ɒH�蒅������---------------------------------------------
	bool                         _isReached_toInvestigatePos{ false };

	// �v���C���[�ɋC�t������� ------------------------------------------------------------
	static bool                  _isNoticedPlayer;
	float                        _timeCountFrom_noticedPlayer{};

	// CSV���烍�[�h-----------------------
	int         _hp{};                         // HP
	int         _maxTotalEnemy_spawnCount{};   // �G�̑���
	int         _bullet_fireInterval{};        // �e�̔��ˊԊu
	float       _bullet_moveSpeed{};           // �e�̈ړ��X�s�[�h
	float       _bullet_reloadTimeInterval{};  // �����[�h�Ԋu

protected:

	//�@����SE---------------------------------
	int         _shotSE_hdl{};

	//�@�e�����Ԋu���Ǘ����邽�߂̃J�E���^�[----------------------------------
	int         _straightBullet_count{};
	int         _homingBullet_count{};

	//�@�ǔ��e�Ŏg�p����l---------------------------------------------------------------------
	const float _minTimeToReach{ 2.0f }; // �e�̃v���C���[�܂ł̍ŏ����B����
	const float _maxTimeToReach{ 3.0f }; // �e�̃v���C���[�܂ł̍ő哞�B����
	float       _bulletTurnDelayRate{};  // �e�̋O���C���̒x����

	//�@��������ԂȂǂ̌Œ�l�i�h���N���X�Ŏg�p�j----------------------------------------------
	float       _IDLE_DISTANCE{};       // ���ڂ̎g�p�͋֎~  Getter��ʂ��Ēl���擾����
	float       _ATTACK_DISTANCE{};     // ���ڂ̎g�p�͋֎~  Getter��ʂ��Ēl���擾����
	float       _NOTICE_LIMIT_DURATION{};
	float       _CHANGE_NEXT_BEHAVE_DURATION{};

	//�@�����_���Ɍ��肵�������n�_�̃x�N�g������------------------------------------------------
	float       _randomInvestigateRange_x{};
	float       _randomInvestigateRange_y{};
	float       _randomInvestigateRange_z{};

	//�@�t���O----------------------------------------------------------------------------------
	bool        _isShotStraightBullet{ true }; // ���s�e�������Ă��邩
	bool        _isShotHomingBullet{ false };  // �ǔ��e�������Ă��邩
	bool        _isAttacking{ false };         // �U������

	// �����Ԃ̂Ƃ��ɖڎw���n�_---------------------------------------------------------------
	tnl::Vector3 _investigatePos{};

private:

	// Enum�ϐ�----------------------------------------
	BEHAVE _behave = EnemyZakoBase::BEHAVE::Moving;  //�G�̋����Ǘ�

	// �����_���f�o�C�X--------------------------------
	std::random_device mt;

	// �����[�h���Ԃ�ǐ�------------------------------
	float       _reloadStraightBullet_timeCounter{};
	float       _reloadHomingBullet_timeCounter{};

	//�@-----------------------------------------
	bool        _isTurning{ false }; // ���񒆂�
};