#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// �X�e�[�W3�{�X
class EnemyBoss_MoriyaSuwako : public EnemyBossBase
{
public:

	EnemyBoss_MoriyaSuwako() {}

	explicit EnemyBoss_MoriyaSuwako(Shared<EnemyBoss_MoriyaSuwako> boss) {}
	
	EnemyBoss_MoriyaSuwako(
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
	static std::vector<Shared<EnemyBullet>> _bullet_normal_suwako;
	static std::vector<Shared<EnemyBullet>> _bullet_ironRingOfMoriya_suwako;
	static std::vector<Shared<EnemyBullet>> _bullet_keroChanStandsFirmAgainstTheStorm_suwako;

	//�@�e���̃A�N�e�B�u��Ԃ𐧌䂷��t���O------------------------------------------------------------
	static bool _isUsingBullet_normal_suwako;
	static bool _isUsingBullet_ironRingOfMoriya_suwako;
	static bool _isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako;
};