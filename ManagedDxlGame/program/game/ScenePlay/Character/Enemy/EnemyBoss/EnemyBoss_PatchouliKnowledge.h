#pragma once
#include "../EnemyBossBase.h"



/*
�@�@�X�e�[�W1�{�X�F�p�`�����[�E�m�[���b�W

	���̃{�X�́u�����v���W�F�N�g�v�Ƃ����e���V���[�e�B���O�Q�[���𒆐S�Ƃ���
	�R���e���c�ɓo�ꂷ��L�����N�^�[�����`�[�t�ɂ��Ă��܂��B
�@�@��{�I�ȋ@�\�͑��̃{�X�Ɠ����ł����A�e���݈̂قȂ���������Ă��܂��B
*/

namespace inl {

	class BulletHell;

	class EnemyBoss_PatchouliKnowledge final : public EnemyBossBase
	{
	public:

		EnemyBoss_PatchouliKnowledge() {}
		~EnemyBoss_PatchouliKnowledge() override {}


		explicit EnemyBoss_PatchouliKnowledge(Shared<EnemyBoss_PatchouliKnowledge> boss) {}

		EnemyBoss_PatchouliKnowledge(const EnemyBossInfo& info);


		// ������---------------------------------------------------------------------------------
		void SetMeshInfo() override;
		void InitBulletHellInstance() override;

		// �`��E�X�V-----------------------------------------------------------------------------
		void Render() override;
		bool Update(const float deltaTime) override;

	private:

		// �v���C���[�֍U��
		void AttackPlayer(const float deltaTime) override;

		// �e�����\��
		void RenderBossSpellCardName() override;

		// �v���C���[������A�U������A�ǂ�������A���������Ȃǂ��܂Ƃ߂�����
		void DoRoutineMoves(const float deltaTime) override;

	public:

		//�@�e���R���e�i------------------------------------------------------------------------------------
		static std::vector<Shared<EnemyBullet>> _bullet_normal_patchouli;
		static std::vector<Shared<EnemyBullet>> _bullet_metalFatigue_patchouli;
		static std::vector<Shared<EnemyBullet>> _bullet_silentSerena_patchouli;

		//�@�e���̃A�N�e�B�u��Ԃ𐧌䂷��t���O------------------------------------------------------------
		static bool _isUsingBullet_normal_patchouli;
		static bool _isUsingBullet_metalFatigue_patchouli;
		static bool _isUsingBullet_silentSerena_patchouli;
	};
}