#pragma once
#include "../../ScenePlay/Item/ScoreItem.h"

class Collision;
class EnemyManager;
class PowerUpItem;
class ScoreManager;


// 主に　得点アイテム・プレイヤー強化アイテムを管理
class ItemManager
{
public:

	ItemManager() {}
	ItemManager(const Shared<Player>& player, const Shared<Collision>& collision)
		: _player_ref(player), _collision_ref(collision) {}

	static ItemManager& GetInstance() {

		static ItemManager instance;
		return instance;
	}

	// 得点アイテム--------------------------------------------------------------------------------------

	void CreateScoreItemPool(const std::string difficulty, const int stageId);     // アイテムプール
	void EventHit_ScoreItemAndPlayer();	                                           // プレイヤーとの当たり判定処理
	void UpdateScoreItem(); 	                                          	       // 更新処理

	// プレイヤー強化アイテム----------------------------------------------------------------------------

	void CreatePowerUpItemPool(const std::string difficulty, const int stageId);   // アイテムプール
	void EventHit_PowerUpItemAndPlayer();	                                       // プレイヤーとの当たり判定処理
	void UpdatePowerUpItem();		                                               // 更新処理

	// その他関数-------------------------------------------------------------

	void SpawnItemsOnEnemyDeath(const tnl::Vector3& enemyPos, const bool isEnemyDead); // 敵の消滅地点にアイテムをスポーン

	void DestroyAllItems() noexcept;		                                           // 全てのアイテムを削除

	void Render(const Shared<dxe::Camera>& camera);
	void Update(const float deltaTime);

private:

	/*
	　   DRY（Don’t Repeat Yourself）・・・重複した処理を避けるべきとする原則
	　   末尾に　DRY　がついている関数は同じ処理を異なるオブジェクトへまとめて適用している
	*/

	// 得点アイテム-------------------------------------------------------------------------------------------------------------------

	// 描画------------------------------------------------------------------------------
	void RenderScoreItems(
		std::vector<Shared<ScoreItem>>& scoreItems, const Shared<dxe::Camera>& camera);

	// 更新------------------------------------------------------------------------------
	void UpdateScoreItem_DRY(std::vector<Shared<ScoreItem>>& scoreItems);

	// 当たり判定------------------------------------------------------------------------
	void EventHit_ScoreItemAndPlayer_DRY(
		std::vector<Shared<ScoreItem>>& scoreItems, const ScoreItem::TYPE type);

	void AvoidOverlap_ScoreItemAndScoreItem();

	void AvoidOverlap_ScoreItemAndScoreItem_DRY(
		std::vector<Shared<ScoreItem>>& scoreItems_1, std::vector<Shared<ScoreItem>>& scoreItems_2);

	// プレイヤー強化アイテム----------------------------------------------------------------------------------------------------------

	// 描画------------------------------------------------------------------------------
	void RenderPowerUpItems(
		std::vector<Shared<PowerUpItem>>& powerUpItems, const Shared<dxe::Camera>& camera);

	// 更新------------------------------------------------------------------------------
	void UpdatePowerUpItem_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems);

	// 当たり判定------------------------------------------------------------------------
	void EventHit_PowerUpItemAndPlayer_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems);

	void AvoidOverlap_PowerUpItemAndPowerUpItem();

	void AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(
		std::vector<Shared<PowerUpItem>>& powerUpItems_1, std::vector<Shared<PowerUpItem>>& powerUpItems_2);

	// 得点アイテム＆プレイヤー強化アイテム-------------------------------------------------------------------------------------------

	void AvoidOverlap_ScoreItemAndPowerUpItem();

	void AvoidOverlap_ScoreItemAndPowerUpItem_DRY(
		std::vector<Shared<ScoreItem>>& scoreItems, std::vector<Shared<PowerUpItem>>& powerUpItems);

	// イベント通知（プレイヤーがアイテムを取得したときに表示するメッセージを通達）---------------------------------------------------
	void RenderEventHitText() const;
	void UpdateEventHitText(const float deltaTime);
	void EventNotify_OnCaughtItem(const std::string itemName, const std::string effect);

private:

	Shared<Player>      _player_ref = nullptr;

	Shared<Collision>   _collision_ref = nullptr;

private:

	// 得点アイテム
	static std::vector<Shared<ScoreItem>> _scoreItem_small;
	static std::vector<Shared<ScoreItem>> _scoreItem_medium;
	static std::vector<Shared<ScoreItem>> _scoreItem_large;

	// プレイヤー強化アイテム
	static std::vector<Shared<PowerUpItem>> _powerUpItem_heal;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_attack;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_defense;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_speed;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_bomb;
};