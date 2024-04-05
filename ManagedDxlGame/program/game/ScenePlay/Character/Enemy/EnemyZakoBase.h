#pragma once
#include <random>
#include "EnemyBase.h"

class Player;

namespace inl {

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
		~EnemyZakoBase() override {}


		EnemyZakoBase(
			const EnemyZakoInfo& data,
			const Shared<Player>& player,
			const Shared<dxe::Camera>& camera,
			const Shared<Collision>& collision
		);

		// ������ --------------------------------------------------------------
		virtual void InitBulletFactoryInstance() {}                                  //�@�e������

		// HP ------------------------------------------------------------------
		bool DecreaseHP(int damage, Shared<dxe::Camera> camera);                     //�@HP����

		// �`��E�X�V ----------------------------------------------------------
		void Render(Shared<dxe::Camera> camera) override;
		bool Update(const float deltaTime) override;

	protected:

		// ���s�e ------------------------------------------------------------
		void ShotStraightBullet(const float deltaTime);                              // ����
		void ReloadStraightBulletByTimer(const float deltaTime);                     // �����[�h
		void UpdateStraightBullet(const float deltaTime);                            // �X�V

		// �ǔ��e ------------------------------------------------------------
		void ShotHomingBullet(const float deltaTime);                                // ����
		void ReloadHomingBulletByTimer(const float deltaTime);	                     // �����[�h
		void UpdateHomingBullet(const float deltaTime);			                     // �X�V

		// �v���C���[�֍U�� -------------------------------------------------------------------
		void AttackPlayer(const float deltaTime);

		// ����	-------------------------------------------------------------------------------

		 // �ǐՁA�U���A�ҋ@���g��������
		void DoRoutineMoves(const float deltaTime);             

		// �ҋ@��Ԓ��ASTOP�E MOVE�E TURN�̃X�e�[�g�ɂ���čs��
		void SearchPlayerMovementState(const float deltaTime);  

		// �ҋ@��Ԓ��A�����_���ړ�
		void MoveToRandomInvestigatePos(const float deltaTime); 

		// �v���C���[�ǐ�
		void ChasePlayer(const float deltaTime);                 

	private:

		bool ShowHpGage_EnemyZako();

		const float GetIdleDistance() const noexcept { return _IDLE_DISTANCE; }       //�@�ҋ@����
		const float GetAttackDistance() const noexcept { return _ATTACK_DISTANCE; }   //�@�U���͈͋���

	protected:

		Shared<Collision>                      _collision_ref = nullptr;              // �����蔻��

		Shared<BulletFactory>                  _bulletFactory = nullptr;              // �e��

		std::list<Shared<StraightBullet>>      _straight_bullets{};                   // ���s�e
		std::deque<Shared<StraightBullet>>     _straightBullet_queue{};

		std::list<Shared<HomingBullet>>        _homing_bullets{};					  // �ǔ��e
		std::deque<Shared<HomingBullet>>       _homingBullet_queue{};

	public:

		// �G�����j�����Ƃ��ɔ������锚���G�t�F�N�g---------------------------------------------
		static Shared<dxe::Particle> _explode_particle;

		// �����_���Ɍ��肵�������n�_�ɒH�蒅������---------------------------------------------
		bool                         _isReachedToInvestigatePos{ false };

		// �v���C���[�ɋC�t������� ------------------------------------------------------------
		static bool                  _isNoticedPlayer;
		float                        _timeCountFrom_noticedPlayer{};

		// CSV���烍�[�h-----------------------
		int         _hp{};                         // HP
		int         _maxTotalEnemy_spawnCount{};   // �G�̑���
		int         _bulletFireInterval{};         // �e�̔��ˊԊu
		float       _bulletMoveSpeed{};            // �e�̈ړ��X�s�[�h
		float       _bulletReloadTimeInterval{};   // �����[�h�Ԋu

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

		// �����[�h���Ԃ�ǐ�------------------------------
		float       _reloadStraightBullet_timeCounter{};
		float       _reloadHomingBullet_timeCounter{};

		//�@-----------------------------------------
		bool        _isTurning{ false }; // ���񒆂�
	};
}