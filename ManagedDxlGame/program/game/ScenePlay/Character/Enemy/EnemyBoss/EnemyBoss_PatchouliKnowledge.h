#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// ステージ1ボス
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


	// 初期化---------------------------------------------------------------------------------
	void SetMeshInfo() override;
	void InitBulletHellInstance() override;

	// 描画・更新-----------------------------------------------------------------------------
	void Render(Shared<dxe::Camera> camera) override;
	bool Update(const float delta_time) override;

private:

	// プレイヤーへ攻撃
	void AttackPlayer(const float& delta_time) override;

	// 弾幕名表示
	void RenderBossSpellCardName() override;

	// プレイヤーを見る、攻撃する、追いかける、距離を取るなどをまとめた動き
	void DoRoutineMoves(const float& delta_time) override;

public:

	//　弾幕コンテナ------------------------------------------------------------------------------------
	static std::vector<Shared<EnemyBullet>> _bullet_normal_patchouli;
	static std::vector<Shared<EnemyBullet>> _bullet_metalFatigue_patchouli;
	static std::vector<Shared<EnemyBullet>> _bullet_silentSerena_patchouli;

	//　弾幕のアクティブ状態を制御するフラグ------------------------------------------------------------
	static bool _isUsingBullet_normal_patchouli;
	static bool _isUsingBullet_metalFatigue_patchouli;
	static bool _isUsingBullet_silentSerena_patchouli;
};