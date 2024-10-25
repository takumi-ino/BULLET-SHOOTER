#pragma once
#include "../EnemyBossBase.h"



/*
　　ステージ1ボス：パチュリー・ノーレッジ

	このボスは「東方プロジェクト」という弾幕シューティングゲームを中心とした
	コンテンツに登場するキャラクターをモチーフにしています。
　　基本的な機能は他のボスと同じですが、弾幕のみ異なる実装をしています。
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


		// 初期化---------------------------------------------------------------------------------
		void SetMeshInfo() override;
		void InitBulletHellInstance() override;

		// 描画・更新-----------------------------------------------------------------------------
		void Render() override;
		bool Update(const float deltaTime) override;

	private:

		// プレイヤーへ攻撃
		void AttackPlayer(const float deltaTime) override;

		// 弾幕名表示
		void RenderBossSpellCardName() override;

		// プレイヤーを見る、攻撃する、追いかける、距離を取るなどをまとめた動き
		void DoRoutineMoves(const float deltaTime) override;

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
}