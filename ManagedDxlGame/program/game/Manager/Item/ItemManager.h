#pragma once
#include "../../ScenePlay/Item/ScoreItem.h"

class Collision;
class EnemyManager;
class PowerUpItem;
class ScoreManager;


// ��Ɂ@���_�A�C�e���E�v���C���[�����A�C�e�����Ǘ�
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

	// ���_�A�C�e��--------------------------------------------------------------------------------------

	void CreateScoreItemPool(const std::string difficulty, const int stageId);     // �A�C�e���v�[��
	void EventHit_ScoreItemAndPlayer();	                                           // �v���C���[�Ƃ̓����蔻�菈��
	void UpdateScoreItem(); 	                                          	       // �X�V����

	// �v���C���[�����A�C�e��----------------------------------------------------------------------------

	void CreatePowerUpItemPool(const std::string difficulty, const int stageId);   // �A�C�e���v�[��
	void EventHit_PowerUpItemAndPlayer();	                                       // �v���C���[�Ƃ̓����蔻�菈��
	void UpdatePowerUpItem();		                                               // �X�V����

	// ���̑��֐�-------------------------------------------------------------

	void SpawnItemsOnEnemyDeath(const tnl::Vector3& enemyPos, const bool isEnemyDead); // �G�̏��Œn�_�ɃA�C�e�����X�|�[��

	void DestroyAllItems() noexcept;		                                           // �S�ẴA�C�e�����폜

	void Render(const Shared<dxe::Camera>& camera);
	void Update(const float deltaTime);

private:

	/*
	�@   DRY�iDon�ft Repeat Yourself�j�E�E�E�d�����������������ׂ��Ƃ��錴��
	�@   �����Ɂ@DRY�@�����Ă���֐��͓����������قȂ�I�u�W�F�N�g�ւ܂Ƃ߂ēK�p���Ă���
	*/

	// ���_�A�C�e��-------------------------------------------------------------------------------------------------------------------

	// �`��------------------------------------------------------------------------------
	void RenderScoreItems(
		std::vector<Shared<ScoreItem>>& scoreItems, const Shared<dxe::Camera>& camera);

	// �X�V------------------------------------------------------------------------------
	void UpdateScoreItem_DRY(std::vector<Shared<ScoreItem>>& scoreItems);

	// �����蔻��------------------------------------------------------------------------
	void EventHit_ScoreItemAndPlayer_DRY(
		std::vector<Shared<ScoreItem>>& scoreItems, const ScoreItem::TYPE type);

	void AvoidOverlap_ScoreItemAndScoreItem();

	void AvoidOverlap_ScoreItemAndScoreItem_DRY(
		std::vector<Shared<ScoreItem>>& scoreItems_1, std::vector<Shared<ScoreItem>>& scoreItems_2);

	// �v���C���[�����A�C�e��----------------------------------------------------------------------------------------------------------

	// �`��------------------------------------------------------------------------------
	void RenderPowerUpItems(
		std::vector<Shared<PowerUpItem>>& powerUpItems, const Shared<dxe::Camera>& camera);

	// �X�V------------------------------------------------------------------------------
	void UpdatePowerUpItem_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems);

	// �����蔻��------------------------------------------------------------------------
	void EventHit_PowerUpItemAndPlayer_DRY(std::vector<Shared<PowerUpItem>>& powerUpItems);

	void AvoidOverlap_PowerUpItemAndPowerUpItem();

	void AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(
		std::vector<Shared<PowerUpItem>>& powerUpItems_1, std::vector<Shared<PowerUpItem>>& powerUpItems_2);

	// ���_�A�C�e�����v���C���[�����A�C�e��-------------------------------------------------------------------------------------------

	void AvoidOverlap_ScoreItemAndPowerUpItem();

	void AvoidOverlap_ScoreItemAndPowerUpItem_DRY(
		std::vector<Shared<ScoreItem>>& scoreItems, std::vector<Shared<PowerUpItem>>& powerUpItems);

	// �C�x���g�ʒm�i�v���C���[���A�C�e�����擾�����Ƃ��ɕ\�����郁�b�Z�[�W��ʒB�j---------------------------------------------------
	void RenderEventHitText() const;
	void UpdateEventHitText(const float deltaTime);
	void EventNotify_OnCaughtItem(const std::string itemName, const std::string effect);

private:

	Shared<Player>      _player_ref = nullptr;

	Shared<Collision>   _collision_ref = nullptr;

private:

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