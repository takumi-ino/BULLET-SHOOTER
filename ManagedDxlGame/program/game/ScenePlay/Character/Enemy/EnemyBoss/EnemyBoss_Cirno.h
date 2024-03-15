#pragma once
#include "../EnemyBossBase.h"

class BulletHell;


/*
�@�@�X�e�[�W2�{�X�F�`���m

	���̃{�X�́u�����v���W�F�N�g�v�Ƃ����e���V���[�e�B���O�Q�[���𒆐S�Ƃ���
	�R���e���c�ɓo�ꂷ��L�����N�^�[�����`�[�t�ɂ��Ă��܂��B
�@�@��{�I�ȋ@�\�͑��̃{�X�Ɠ����ł����A�e���݈̂قȂ���������Ă��܂��B
*/



class EnemyBoss_Cirno final : public EnemyBossBase
{
public:

	EnemyBoss_Cirno() {}
	~EnemyBoss_Cirno() override {}


	explicit EnemyBoss_Cirno(Shared<EnemyBoss_Cirno> boss) {}

	EnemyBoss_Cirno(
		const EnemyBossInfo& info,
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision
	);


	// ������---------------------------------------------------------------------------------
	void SetMeshInfo() override;
	void InitBulletHellInstance() override;

	// �`��E�X�V-----------------------------------------------------------------------------
	void Render(Shared<dxe::Camera> camera) override;
	bool Update(const float deltaTime) override;

private:

	// �v���C���[�֍U��
	void AttackPlayer(const float& deltaTime) override;

	// �e�����\��
	void RenderBossSpellCardName() override;

	// �v���C���[������A�U������A�ǂ�������A���������Ȃǂ��܂Ƃ߂�����
	void DoRoutineMoves(const float& deltaTime) override;

public:

	//�@�e���R���e�i------------------------------------------------------------------------------------
	static std::vector<Shared<EnemyBullet>> _bullet_normal_cirno;
	static std::vector<Shared<EnemyBullet>> _bullet_icicleFall_cirno;
	static std::vector<Shared<EnemyBullet>> _bullet_perfectFreeze_cirno;

	//�@�e���̃A�N�e�B�u��Ԃ𐧌䂷��t���O------------------------------------------------------------
	static bool _isUsingBullet_normal_cirno;
	static bool _isUsingBullet_icicleFall_cirno;
	static bool _isUsingBullet_perfectFreeze_cirno;
};