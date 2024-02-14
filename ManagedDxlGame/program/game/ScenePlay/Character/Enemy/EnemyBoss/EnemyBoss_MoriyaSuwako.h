#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// ステージ3ボス
class EnemyBoss_MoriyaSuwako : public EnemyBossBase
{
public:

	EnemyBoss_MoriyaSuwako() {}

	explicit EnemyBoss_MoriyaSuwako(Shared<EnemyBoss_MoriyaSuwako> boss) {}
	
	EnemyBoss_MoriyaSuwako(const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera);

	void SetMeshInfo() override;

	void InitBulletHellInstance() override;

	bool Update(float delta_time) override;

	void Render(Shared<dxe::Camera> camera) override;

private:

	void AttackPlayer(const float& delta_time) override;

	void RenderBossSpellCardName() override;

	void DoRoutineMoves(const float& delta_time) override;


public:

	Shared<BulletHell> _bulletHell = nullptr;

public:

	static int _at;

private:

	int _MAX_HP;
};