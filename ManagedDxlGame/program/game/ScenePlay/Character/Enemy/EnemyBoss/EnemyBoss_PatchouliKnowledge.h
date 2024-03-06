#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// �X�e�[�W1�{�X
class EnemyBoss_PatchouliKnowledge : public EnemyBossBase
{
public:

	EnemyBoss_PatchouliKnowledge() {}

	explicit EnemyBoss_PatchouliKnowledge(Shared<EnemyBoss_PatchouliKnowledge> boss) {}

	EnemyBoss_PatchouliKnowledge(
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
	bool Update(const float delta_time) override;

private:

	// �v���C���[�֍U��
	void AttackPlayer(const float& delta_time) override;

	// �e�����\��
	void RenderBossSpellCardName() override;

	// �v���C���[������A�U������A�ǂ�������A���������Ȃǂ��܂Ƃ߂�����
	void DoRoutineMoves(const float& delta_time) override;

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