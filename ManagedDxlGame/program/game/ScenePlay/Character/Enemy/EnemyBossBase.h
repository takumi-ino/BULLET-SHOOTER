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

		// ������----------------------------------------------------------
		virtual void InitBulletHellInstance() {}

		// �_���[�W--------------------------------------------------------
		void DecreaseBossHP(int damage);

	protected:

		// HP------------------------------------------------------------
		bool ShowHpGage_EnemyBoss();
		void RenderBossRemainLife();

		// ���O-----------------------------------------------------------
		void RenderBossName();
		virtual void RenderBossSpellCardName() {}

		// �����蔻��-----------------------------------------------------
		void CheckCollision_BulletHellBulletsAndPlayer_DRY(
			std::vector<Shared<EnemyBullet>>& bulletVector
		);

		// ����------------------------------------------------------------
		void ActKeepDistanceToPlayer(const float deltaTime);        // �v���C���[�ƈ�苗����ۂ�

		void WarpToRandomPos(const float deltaTime);				// ��莞�ԂŃ����_�����[�v

		void ClampMovableRange(tnl::Vector3& moveDirection);		// �ړ��͈͐���

	public:

		std::deque<int>                  _bossHp{};
		Shared<BulletHell>               _bulletHell = nullptr;

	protected:

		Shared<Collision>                _collision_ref = nullptr;

	protected:

		int       _MAX_HP{};
		float     _WARPING_DURATION{};

	private:

		// �{�X�c��HP�B4�̐Ԃ� �_�C���̌`����������
		std::stack<std::deque<int>> _remainingLife_indicator;

		// ���[�v����^�C�~���O���^�C�}�[�Ő���
		float _warpToRandPosTimer{};
	};
}