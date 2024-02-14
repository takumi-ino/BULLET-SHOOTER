#pragma once

#include "../../ScenePlay/Item/ScoreItem.h"

class Collision;
class EnemyZakoBase;
class EnemyManager;
class PowerUpItem;
class ScoreManager;
class EventNoticeText;

// ��Ɂ@���_�A�C�e���E�v���C���[�����A�C�e�����Ǘ�
class ItemManager
{
public:

	ItemManager(){}
	ItemManager(const Shared<Player>& player, const Shared<Collision>& collision) 
		: _player_ref(player), _collision_ref(collision){}

	ItemManager(const Shared<EnemyManager>& enemyManager) {}

	static ItemManager& GetInstance() {

		static ItemManager instance;
		return instance;
	}

	// ���_�A�C�e��
	void CreateScoreItemPool(const std::string difficulty, const int stage_id);
	void SetScoreItem_CollisionPairLists();
	void UpdateScoreItem();


	// �v���C���[�����A�C�e��
	void CreatePowerUpItemPool(const std::string difficulty, const int stage_id);
	void SetPowerUpItem_CollisionPairLists();
	void UpdatePowerUpItem();

	void SpawnItemsOnEnemyDeath(const tnl::Vector3& enemyPos, const bool isEnemyDead);

	void DestroyAllItems();

	void Render(const Shared<dxe::Camera>& camera);
	void Update(const float& deltaTime);

private:

	// ���_�A�C�e��
	void UpdateScoreItem_DRY(std::vector<Shared<ScoreItem>>& scoreItems);
	void SetScoreItem_CollisionPairLists_DRY(std::vector<Shared<ScoreItem>>& scoreItems, const ScoreItem::TYPE type);
	void RenderScoreItems(std::vector<Shared<ScoreItem>>& scoreItems, const Shared<dxe::Camera>& camera);
	void ScoreItemCollisionPairLists();
	void ScoreItemCollisionPairLists_DRY(std::vector<Shared<ScoreItem>>& scoreItems_1, std::vector<Shared<ScoreItem>>& scoreItems_2);

	// �v���C���[�����A�C�e��
	void UpdatePowerUpItem_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems);
	void SetPowerUpItem_CollisionPairLists_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems);
	void RenderPowerUpItems(std::vector<Shared<PowerUpItem>>& powerUpItems, const Shared<dxe::Camera>& camera);
	void PowerUpItemCollisionPairLists();
	void PowerUpItemCollisionPairLists_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems_1, std::vector<Shared<PowerUpItem>>& powerUpItems_2);

	// ���_�A�C�e�����v���C���[�����A�C�e��
	void ScoreItemAndPowerUpItem_CollisionPairLists();
	void ScoreItemAndPowerUpItem_CollisionPairLists_DRY(std::vector<Shared<ScoreItem>>& scoreItems, std::vector<Shared<PowerUpItem>>& powerUpItems);


	// �C�x���g�ʒm
	void RenderEventHitText() const;
	void UpdateEventHitText(const float& deltaTime);
	void OnCaughtItem(const std::string item_name, const std::string effect);

private:

	Shared<Player>      _player_ref = nullptr;

	Shared<Collision> _collision_ref = nullptr;

public:

	// ���_�A�C�e��
	static std::vector<Shared<ScoreItem>> _scoreItem_small;
	static std::vector<Shared<ScoreItem>> _scoreItem_medium;
	static std::vector<Shared<ScoreItem>> _scoreItem_large;

	// �v���C���[�����A�C�e��
	static std::vector<Shared<PowerUpItem>> _powerUpItem_heal;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_attack;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_defense;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_speed;
	static std::vector<Shared<PowerUpItem>> _powerUpItem_bomb;
};