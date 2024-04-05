#pragma once
#include "EnemyBullet.h"


class ScenePlay;
class Player;

/*
 �@�@ �@�e�̐����A�X�V�A�폜�Ȃǂ��s���N���X (EnemyBoss����)
�@�@�@�{�X�͒ʏ�U���A����U���P�A����U���Q�A�̌v�R��ނ̒e�������

    ���@�֐��̒�`��1�ӏ��ɂ܂Ƃ߂�ƒ����Ȃ��Ă��܂����߁A�e�{�X���Ƃ� cpp �ɕ����Ă��܂��B

		�@1. AllBulletHell_PatchouliKnowledge.cpp�@ �i�X�e�[�W�P�{�X�j
		�@2. AllBulletHell_Cirno.cpp				�i�X�e�[�W�Q�{�X�j
		�@3. AllBulletHell_MoriyaSuwako.cpp		  �@�i�X�e�[�W�R�{�X�j


�@�@��    �e���̖��́E������
        �u�����v���W�F�N�g�v�Ƃ����e���V���[�e�B���O�Q�[���𒆐S�Ƃ����R���e���c���璼�ڈ��p���Ă��܂��B
	    �ŗL���������ɑ���������Â炭�Ȃ��Ă��邩�Ƒ����܂����A�������������������܂��ƍK���ł��B
*/


namespace inl {

	class BulletHellFactory;
	class RandomValueGenerator;

	class BulletHell : public EnemyBullet
	{
	public:

		enum class TYPE {

			// �p�`�����[�i�X�e�[�W�P�{�X�j
			Normal_Patchouli,
			MetalFatigue_Patchouli,
			SilentSerena_Patchouli,

			// �`���m�i�X�e�[�W�Q�{�X�j
			Normal_Cirno,
			IcicleFall_Cirno,
			Perfect_Freeze_Cirno,

			// �z�K�q�i�X�e�[�W�R�{�X�j
			Normal_Suwako,
			IronRingOfMoriya_Suwako,
			KeroChan_StandsFirm_AgainstTheStorm_Suwako,
		};

	public:

		// �R���X�g���N�^--------------------------------------------------------------------------------

		BulletHell() {}
		BulletHell(const Shared<dxe::Mesh>& bossMesh, const Shared<Player>& player)
			: _bossMesh_ref(bossMesh) {

			_player_ref = player;
		}

		~BulletHell() override {}

		// ----------------------------------------------------------------------------------------------

		/*
			 �ȉ��֐���

			 1. �@�\�i��ɔ��ˁE�������j
			 2. �X�y���i�e�����j
			 3. �g�p�ҁi�L�����N�^�[���j

			 �Ƃ����K���ɑ����Ė������Ă��܂��B
		*/

		// �X�e�[�W1�{�X�i�p�`�����[�j--------------------------------------------------
		void ShotBulletHell_Normal_Patchouli(const float deltaTime);
		void ShotBulletHell_MetalFatigue_Patchouli(const float deltaTime);
		void ShotBulletHell_SilentSerena_Patchouli(const float deltaTime);

		// �X�e�[�W2�{�X�i�`���m�j----------------------------------------------------------
		void ShotBulletHell_Normal_Cirno(const float deltaTime);
		void ShotBulletHell_IcicleFall_Cirno(const float deltaTime);
		void ShotBulletHell_PerfectFreeze_Cirno(const float deltaTime);

		// �X�e�[�W3�{�X�i�z�K�q�j----------------------------------------------------------
		void ShotBulletHell_Normal_Suwako(const float deltaTime);
		void ShotBulletHell_IronRingOfMoriya_Suwako(const float deltaTime);
		void ShotBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako(const float deltaTime);

	private:

		// ���������������ꍇ�A�������̂��ߕ⏕�֐����g�p�i InitAssist, WaveAssist, UpdateAssist�j�Ȃ�

		// �X�e�[�W1�{�X�i�p�`�����[�j-----------------------------------------------------------
		void WaveAssist_MetalFatigue_Patchouli(
			Shared<EnemyBullet>& bullet,
			const float deltaTime,
			const float angleOrigin,
			const float radiusOrigin,
			const float startMoveTime
		);

		// �X�e�[�W2�{�X�i�`���m�j---------------------------------------------------------------
		void WaveAssist_Normal_Cirno(
			Shared<EnemyBullet>& bullet,
			const float timing,
			const float delayOffset,
			const float deltaTime
		);

		void InitAssist_IcicleFall_StraightBlue_Cirno(
			Shared<EnemyBullet>& bullet,
			const float radius,
			const float angle
		);

		void UpdateAssist_IcicleFall_StraightBlue_Cirno(
			Shared<EnemyBullet>& bullet,
			const float radius,
			const float newAngle,
			const float startChangeDirTime,
			const float deltaTime
		);

		void InitAssist_IcicleFall_StraightYellow_Cirno(
			Shared<EnemyBullet>& bullet
		);

		void UpdateAssist_IcicleFall_StraightYellow_Cirno(
			Shared<EnemyBullet>& bullet,
			const float deltaTime,
			const float bulletSpeed
		);

		// �X�e�[�W3�{�X�i�z�K�q�j-----------------------------------------------------------------------
		void ResetAssist_Normal_Suwako(
			Shared<EnemyBullet>& bullet,
			const float timeLimit
		);

		void WaveAssist_IronRingOfMoriya_Suwako(
			Shared<EnemyBullet>& bullet,
			const float circleRadius,
			const float angle,
			const float deltaTime,
			const float bulletSpeed,
			const int startMoveTime
		);

		void InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
			const float deltaTime,
			const float upwardPower,
			Shared<EnemyBullet>& bullet,
			const bool isOscillate
		);

		void UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
			Shared<EnemyBullet>& bullet,
			const float deltaTime
		);

	private:

		const Shared<dxe::Mesh> _bossMesh_ref = nullptr;
	};
}