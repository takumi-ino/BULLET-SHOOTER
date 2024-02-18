#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// ステージ1ボス
class EnemyBoss_PatchouliKnowledge : public EnemyBossBase
{
public:

	EnemyBoss_PatchouliKnowledge() {}

	explicit EnemyBoss_PatchouliKnowledge(Shared<EnemyBoss_PatchouliKnowledge> boss) {}

	EnemyBoss_PatchouliKnowledge(const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera);


	void SetMeshInfo() override;

	void InitBulletHellInstance() override;

	bool Update(const float delta_time) override;

	void Render(Shared<dxe::Camera> camera) override;

private:

	void AttackPlayer(const float& delta_time) override;

	void RenderBossSpellCardName() override;

	void DoRoutineMoves(const float& delta_time) override;

public:

	static int _at;
};