#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// ステージ2ボス
class EnemyBoss_Cirno : public EnemyBossBase
{
public:

	EnemyBoss_Cirno() {}
	explicit EnemyBoss_Cirno(Shared<EnemyBoss_Cirno> boss) {}
	EnemyBoss_Cirno(const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera);


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