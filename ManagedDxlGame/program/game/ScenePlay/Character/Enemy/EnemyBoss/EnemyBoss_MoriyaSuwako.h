#pragma once
#include "../EnemyBossBase.h"

class BulletHell;


/*
　　ステージ3ボス：洩矢諏訪子

	このボスは「東方プロジェクト」という弾幕シューティングゲームを中心とした
	コンテンツに登場するキャラクターをモチーフにしています。
　　基本的な機能は他のボスと同じですが、弾幕のみ異なる実装をしています。
*/



class EnemyBoss_MoriyaSuwako final : public EnemyBossBase
{
public:

	EnemyBoss_MoriyaSuwako() {}
	~EnemyBoss_MoriyaSuwako() override{}

	explicit EnemyBoss_MoriyaSuwako(Shared<EnemyBoss_MoriyaSuwako> boss) {}
	
	EnemyBoss_MoriyaSuwako(
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
	bool Update(const float deltaTime) override;

private:

	// プレイヤーへ攻撃
	void AttackPlayer(const float& deltaTime) override;

	// 弾幕名表示
	void RenderBossSpellCardName() override;

	// プレイヤーを見る、攻撃する、追いかける、距離を取るなどをまとめた動き
	void DoRoutineMoves(const float& deltaTime) override;

public:

	//　弾幕コンテナ------------------------------------------------------------------------------------
	static std::vector<Shared<EnemyBullet>> _bullet_normal_suwako;
	static std::vector<Shared<EnemyBullet>> _bullet_ironRingOfMoriya_suwako;
	static std::vector<Shared<EnemyBullet>> _bullet_keroChanStandsFirmAgainstTheStorm_suwako;

	//　弾幕のアクティブ状態を制御するフラグ------------------------------------------------------------
	static bool _isUsingBullet_normal_suwako;
	static bool _isUsingBullet_ironRingOfMoriya_suwako;
	static bool _isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako;
};