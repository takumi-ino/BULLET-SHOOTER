#pragma once
#include "../Character.h"

namespace inl {

	class EnemyBase : public Character
	{
	public:

		// �R���X�g���N�^�E�f�X�g���N�^------------------------------------
		EnemyBase() {}
		~EnemyBase() override {}

		// ------------------------------------------------------------------
		float GetDistanceToPlayer();

		// �`��A�e�N�X�`���A�|�W�V�����A�X�P�[��
		virtual void SetMeshInfo() {}

		virtual bool Update(const float deltaTime) { return true; }

		virtual void Render(Shared<dxe::Camera> camera) {}

	protected:

		// �ҋ@�A�ǐՁA�U���Ȃǂ̃p�^�[�����Ǘ������s
		virtual void DoRoutineMoves(const float deltaTime) {}
		//�@�v���C���[�ǐ�
		virtual void ChasePlayer(const float deltaTime) {}
		//�@�v���C���[���U��
		virtual void AttackPlayer(const float deltaTime) {}
		//�@�v���C���[������
		void LookAtPlayer();
		//�@
		tnl::Vector3 GetRandomPosition() const;

	protected:

		Shared<dxe::Camera>  _enemyCamera = nullptr;

	public:

		bool           _isDead = false; //�G�P�̂̎��S�t���O

		// CSV���烍�[�h-----------------------

		int            _id{};
		float          _enemyMoveSpeed{};
		float          _scale{};
		std::string    _name{};
		int            _maxBulletSpawnCount{};

	private:

		const float _RANDOM_SPAWN_RANGE_X = 800.0f;
		const float _RANDOM_SPAWN_RANGE_Y = 100.0f;
		const float _RANDOM_SPAWN_RANGE_Z = 500.0f;
	};
}