#pragma once
#include "../EnemyBossBase.h"

class BulletHell;

// ステージ2ボス
class EnemyBoss_Cirno : public EnemyBossBase
{
public:

	EnemyBoss_Cirno() {}

	explicit EnemyBoss_Cirno(Shared<EnemyBoss_Cirno> boss) {}

	EnemyBoss_Cirno(
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
	static std::vector<Shared<EnemyBullet>> _bullet_normal_cirno;
	static std::vector<Shared<EnemyBullet>> _bullet_icicleFall_cirno;
	static std::vector<Shared<EnemyBullet>> _bullet_perfectFreeze_cirno;

	//　弾幕のアクティブ状態を制御するフラグ------------------------------------------------------------
	static bool _isUsingBullet_normal_cirno;
	static bool _isUsingBullet_icicleFall_cirno;
	static bool _isUsingBullet_perfectFreeze_cirno;
};