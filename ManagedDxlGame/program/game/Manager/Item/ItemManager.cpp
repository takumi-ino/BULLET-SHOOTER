#include "../../ScenePlay/Item/PowerUpItem.h"
// �}�l�[�W���[----------------------------------------------------
#include "../../Manager/Enemy/EnemyManager.h"
#include "../Score/ScoreManager.h"
#include "ItemManager.h"
// ---------------------------------------------------
#include "../../ScenePlay/Collision/Collision.h"
#include "../../ScenePlay/Character/Player/Player.h"
#include "../../ScenePlay/EventMessage/EventNoticeText.h"
#include "../../ScenePlay/RandomValue/RandomValueGenerator.h"


// ���_�A�C�e��--------------------------------------------------------------------------------------------------------------------------
std::vector<Shared<inl::ScoreItem>> ItemManager::_scoreItem_small;
std::vector<Shared<inl::ScoreItem>> ItemManager::_scoreItem_medium;
std::vector<Shared<inl::ScoreItem>> ItemManager::_scoreItem_large;

void ItemManager::CreateScoreItemPool(const std::string difficulty, const int stageId) {

	struct ItemSize {
		int s, m, l;
	};

	// Easy����Ƃ����e���_�A�C�e���̑���
	int smallCount = 10;
	int mediumCount = 5;
	int largeCount = 1;

	std::map<std::string, std::vector<ItemSize>> itemCountSetting
	{
		// �e�X�e�[�W���Ƃ�{}�ŕ����A{}�̒��� s,m,l ��ݒ�

		// ��Փx�@Easy
		{"Easy",
		{{smallCount,    mediumCount,     largeCount},        //stage1 
		{smallCount,     mediumCount,     largeCount},        //stage2 
		{smallCount,     mediumCount,     largeCount},}},     //stage3
		// ��Փx�@Normal
		{"Normal",
		{{smallCount - 2,mediumCount - 1, largeCount * 2},    //stage1
		{smallCount - 2, mediumCount,     largeCount * 2},    //stage2
		{smallCount - 2, mediumCount,     largeCount * 2}}},  //stage3
		// ��Փx�@Hard
		{"Hard",
		{{smallCount - 4,mediumCount - 2, largeCount * 3},    //stage1
		{smallCount - 4, mediumCount - 2, largeCount * 3},	   //stage2
		{smallCount - 4, mediumCount - 2, largeCount * 3}}},  //stage3
		// ��Փx�@Lunatic
		{"Lunatic",
		{{smallCount - 6,mediumCount - 3, largeCount * 4},    //stage1
		{smallCount - 6, mediumCount - 3, largeCount * 4},	   //stage2
		{smallCount - 6, mediumCount - 3, largeCount * 4}}}   //stage3
	};

	// �R���X�g���N�^�Ŏ󂯎������Փx�ƈ�v������̂��Ȃ��ꍇ
	if (itemCountSetting.count(difficulty) <= 0) return;

	//�@1��菬�����A�������͂R���傫���ꍇ
	if (stageId < 1 || stageId > 3) return;


	ItemSize& size = itemCountSetting[difficulty][stageId - 1];

	// ���A�C�e��
	for (int i = 0; i < size.s; i++) {

		Shared<inl::ScoreItem> item =
			std::make_shared<inl::ScoreItem>(inl::ScoreItem::TYPE::Small);

		_scoreItem_small.push_back(item);
	}

	// ���A�C�e��
	for (int i = 0; i < size.m; i++) {

		Shared<inl::ScoreItem> item =
			std::make_shared<inl::ScoreItem>(inl::ScoreItem::TYPE::Medium);

		_scoreItem_medium.push_back(item);
	}

	// ��A�C�e��
	for (int i = 0; i < size.l; i++) {

		Shared<inl::ScoreItem> item =
			std::make_shared<inl::ScoreItem>(inl::ScoreItem::TYPE::Large);

		_scoreItem_large.push_back(item);
	}
}



void ItemManager::EventHit_ScoreItemAndPlayer_DRY(
	std::vector<Shared<inl::ScoreItem>>& scoreItems,
	const inl::ScoreItem::TYPE type)
{
	for (decltype(auto)it : scoreItems) {

		// ���b�V�����Ȃ��܂��͔�A�N�e�B�u��ԂȂ�
		if (!it->_mesh || !it->_isActive)
			return;

		//�@�����蔻��
		if (_collision_ref->CheckCollision_PlayerAndScoreItem(it, _player_ref)) {

			switch (type)
			{
				//�@���_�A�C�e����
			case inl::ScoreItem::TYPE::Small:
			{
				EventNotify_OnCaughtItem("���_�A�C�e����", "500�|�C���g���Z�B");
				ScoreManager::GetInstance().AddScoreItemScore(500);
				break;
			}
			//�@���_�A�C�e����
			case inl::ScoreItem::TYPE::Medium:
			{
				EventNotify_OnCaughtItem("���_�A�C�e����", "1000�|�C���g���Z�B");
				ScoreManager::GetInstance().AddScoreItemScore(100);
				break;
			}
			//�@���_�A�C�e����
			case inl::ScoreItem::TYPE::Large:
			{
				EventNotify_OnCaughtItem("���_�A�C�e����", "1500�|�C���g���Z�B");
				ScoreManager::GetInstance().AddScoreItemScore(1500);
				break;
			}
			}

			it->_isActive = false;
		}
	}
}


void ItemManager::EventHit_ScoreItemAndPlayer()
{
	//�@���_�A�C�e����
	EventHit_ScoreItemAndPlayer_DRY
	(_scoreItem_small, inl::ScoreItem::TYPE::Small);

	//�@���_�A�C�e����
	EventHit_ScoreItemAndPlayer_DRY(
		_scoreItem_medium, inl::ScoreItem::TYPE::Medium);

	//�@���_�A�C�e����
	EventHit_ScoreItemAndPlayer_DRY(
		_scoreItem_large, inl::ScoreItem::TYPE::Large);
}


void ItemManager::UpdateScoreItem_DRY(std::vector<Shared<inl::ScoreItem>>& scoreItems) {

	if (scoreItems.empty())
		return;

	for (auto it_scrItm = scoreItems.begin(); it_scrItm != scoreItems.end();) {

		if (!(*it_scrItm)->Update((*it_scrItm))) {

			// ��A�N�e�B�u��
			it_scrItm = scoreItems.erase(it_scrItm);
		}
		else {
			//�@�X�V
			it_scrItm++;
		}
	}
}

void ItemManager::UpdateScoreItem()
{
	UpdateScoreItem_DRY(_scoreItem_small);
	UpdateScoreItem_DRY(_scoreItem_medium);
	UpdateScoreItem_DRY(_scoreItem_large);
}

// �v���C���[�����A�C�e��------------------------------------------------------------------------------------------------------------------------------------
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_heal;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_attack;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_defense;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_speed;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_bomb;


void ItemManager::CreatePowerUpItemPool(const std::string difficulty, const int stageId) {

	struct ItemType {
		int heal, attack, defense, speed, bomb;
	};

	// Easy����Ƃ����e�v���C���[�����A�C�e���̑���
	int heal_cnt = 10;
	int attack_cnt = 5;
	int defense_cnt = 1;
	int speed_cnt = 10;
	int bomb_cnt = 5;


	std::map<std::string, std::vector<ItemType>> itemCountSetting
	{
		// �e�X�e�[�W���Ƃ�{}�ŕ����A{}�̒��� s,m,l ��ݒ�
		{"Easy",
		{{heal_cnt, attack_cnt, defense_cnt, speed_cnt, bomb_cnt},     //stage1 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt},      //stage2 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt}, }},   //stage3
		{"Normal",
		{{heal_cnt, attack_cnt, defense_cnt, speed_cnt, bomb_cnt},     //stage1 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt},      //stage2 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt}, }},   //stage3
		{"Hard",
		{{heal_cnt, attack_cnt, defense_cnt, speed_cnt, bomb_cnt},     //stage1 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt},      //stage2 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt}, }},   //stage3
		{"Lunatic",
		{{heal_cnt, attack_cnt, defense_cnt, speed_cnt, bomb_cnt},     //stage1 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt},      //stage2 
		{heal_cnt,  attack_cnt, defense_cnt, speed_cnt, bomb_cnt}, }},   //stage3
	};


	// �R���X�g���N�^�Ŏ󂯎������Փx�ƈ�v������̂��Ȃ��ꍇ
	if (itemCountSetting.count(difficulty) <= 0) return;

	//�@1��菬�����A�������͂R���傫���ꍇ
	if (stageId < 1 && stageId > 3) return;


	ItemType& type = itemCountSetting[difficulty][stageId - 1];

	// �񕜃A�C�e��
	for (int i = 0; i < type.heal; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Heal);

		_powerUpItem_heal.push_back(item);
	}

	//�@AT�A�C�e��
	for (int i = 0; i < type.attack; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Attack);

		_powerUpItem_attack.push_back(item);
	}

	//�@DEF�A�C�e��
	for (int i = 0; i < type.defense; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Defense);

		_powerUpItem_defense.push_back(item);
	}

	//�@�X�s�[�h�A�C�e��
	for (int i = 0; i < type.speed; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Speed);

		_powerUpItem_speed.push_back(item);
	}

	//�@�{���A�C�e��
	for (int i = 0; i < type.bomb; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Bomb);

		_powerUpItem_bomb.push_back(item);
	}
}


void ItemManager::EventHit_PowerUpItemAndPlayer_DRY(std::vector<Shared<inl::PowerUpItem>>& powerUpItems) {

	for (decltype(auto) it : powerUpItems) {

		// ���b�V�����Ȃ��܂��͔�A�N�e�B�u��ԂȂ�
		if (!it->_mesh || !it->_isActive)
			return;

		// �����蔻��
		if (_collision_ref->CheckCollision_PlayerAndPowerUpItem(it, _player_ref)) {

			switch (it->_type)
			{
				//�@�񕜃A�C�e��
			case inl::PowerUpItem::TYPE::Heal:
			{
				//�@HP���ő�HP�Ɠ���������ȏ�Ȃ珈���𔲂���
				if (_player_ref->GetHP() >= _player_ref->GetMaxHP()) {
					break;
				}

				EventNotify_OnCaughtItem("�񕜃A�C�e��", "HP���㏸�B");

				_player_ref->HealHP(10);

				//�@HP�㏸��AHP���ő�HP�ȏ�ɂȂ�����HP���ő�l�ŃL���b�v
				if (_player_ref->GetHP() > _player_ref->GetMaxHP()) {

					_player_ref->SetHP(_player_ref->GetMaxHP());
				}
				break;
			}
			//�@�U���A�C�e��
			case inl::PowerUpItem::TYPE::Attack:
			{
				EventNotify_OnCaughtItem("�U�������A�C�e��", "�U���͂��㏸�B");

				_player_ref->AddAT(2);
				break;
			}
			//�@�h��A�C�e��
			case inl::PowerUpItem::TYPE::Defense:
			{
				EventNotify_OnCaughtItem("�h�䋭���A�C�e��", "�h��͂��㏸�B");

				_player_ref->AddDEF(2);
				break;
			}
			//�@�X�s�[�h�A�C�e��
			case inl::PowerUpItem::TYPE::Speed:
			{
				EventNotify_OnCaughtItem("�X�s�[�h�����A�C�e��", "�X�s�[�h���㏸�B");

				_player_ref->AddSpeed(0.1f);
				break;
			}
			//�@�{���A�C�e��
			case inl::PowerUpItem::TYPE::Bomb:
			{
				EventNotify_OnCaughtItem("�{��", "");

				_player_ref->AddBombStockCount();
				break;
			}
			}

			it->_isActive = false;
		}
	}
}

void ItemManager::EventHit_PowerUpItemAndPlayer() {
	EventHit_PowerUpItemAndPlayer_DRY(_powerUpItem_heal);
	EventHit_PowerUpItemAndPlayer_DRY(_powerUpItem_attack);
	EventHit_PowerUpItemAndPlayer_DRY(_powerUpItem_defense);
	EventHit_PowerUpItemAndPlayer_DRY(_powerUpItem_speed);
	EventHit_PowerUpItemAndPlayer_DRY(_powerUpItem_bomb);
}


void ItemManager::DestroyAllItems() noexcept {
	_scoreItem_small.clear();
	_scoreItem_medium.clear();
	_scoreItem_large.clear();

	_powerUpItem_heal.clear();
	_powerUpItem_attack.clear();
	_powerUpItem_defense.clear();
	_powerUpItem_speed.clear();
	_powerUpItem_bomb.clear();
}

// �C�x���g�ʒm----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::EventNotify_OnCaughtItem(const std::string itemName, const std::string effect) {

	//�@�擾�����A�C�e���𕶎���Ŏ擾
	std::string msgStr = itemName + "���l���B \n" + effect;

	Shared<inl::EventNoticeText> event =
		std::make_shared<inl::EventNoticeText>(msgStr, GetColor(0, 255, 0), 16, 35);

	//�@�ʒm
	inl::EventNoticeText::_messageQueue.push_back(event);
}


void ItemManager::RenderEventHitText() const {

	int index = 0;
	for (auto msg : inl::EventNoticeText::_messageQueue) {
		msg->Render(index);
		index++;
	}
}


void ItemManager::UpdateEventHitText(const float deltaTime) {

	for (const auto& msg : inl::EventNoticeText::_messageQueue) {
		msg->Update(deltaTime);
	}

	auto it = inl::EventNoticeText::_messageQueue.begin();

	while (it != inl::EventNoticeText::_messageQueue.end()) {

		// ���Ԑ؂�Ń��b�Z�[�W�폜
		if ((*it)->IsExpired()) {
			it = inl::EventNoticeText::_messageQueue.erase(it);
		}
		else {
			// �\��
			++it;
		}
	}
}


// �����蔻��-----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::AvoidOverlap_ScoreItemAndScoreItem_DRY(
	std::vector<Shared<inl::ScoreItem>>& scoreItems_1,
	std::vector<Shared<inl::ScoreItem>>& scoreItems_2) {

	for (decltype(auto) it : scoreItems_1) {
		for (decltype(auto) it2 : scoreItems_2) {

			_collision_ref->CheckCollision_ScoreItemAndScoreItem(
				it,                // �A�C�e���P
				it2,			   // �A�C�e���Q
				it->_mesh->pos_,   // �A�C�e���P�ʒu
				it2->_mesh->pos_   // �A�C�e���Q�ʒu
			);
		}
	}
}

void ItemManager::AvoidOverlap_ScoreItemAndScoreItem()
{
	//�@�X�R�A�A�C�e���ƃX�R�A�A�C�e���̓����蔻��␳

	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_small, _scoreItem_small);    // ���Ə�
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_small, _scoreItem_medium);   // ���ƒ�
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_small, _scoreItem_large);    // ���Ƒ�
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_medium, _scoreItem_medium);  // ���ƒ�
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_medium, _scoreItem_large);   // ���Ƒ�
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_large, _scoreItem_large);    // ��Ƒ�
}


void ItemManager::AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems_1,
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems_2) {

	for (decltype(auto) it : powerUpItems_1) {
		for (decltype(auto) it2 : powerUpItems_2) {

			_collision_ref->CheckCollision_PowerUpItemAndPowerUpItem(
				it,                // �A�C�e���P
				it2,			   // �A�C�e���Q
				it->_mesh->pos_,   // �A�C�e���P�ʒu
				it2->_mesh->pos_   // �A�C�e���Q�ʒu
			);
		}
	}
}

void ItemManager::AvoidOverlap_PowerUpItemAndPowerUpItem() {

	//�@�v���C���[�����A�C�e���ƃv���C���[�����A�C�e���̓����蔻��␳

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_heal);    // �񕜂Ɖ�
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_attack);  // �񕜂ƍU��
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_defense); // �񕜂Ɩh��
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_speed);   // �񕜂ƃX�s�[�h
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_bomb);    // �񕜂ƃ{��

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_attack);   // �U���ƍU��
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_defense);  // �U���Ɩh��
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_speed);    // �U���ƃX�s�[�h
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_bomb);     // �U���ƃ{��

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_defense, _powerUpItem_defense); // �h��Ɩh��
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_defense, _powerUpItem_speed);   // �h��ƃX�s�[�h
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_defense, _powerUpItem_bomb);    // �h��ƃ{��

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_speed, _powerUpItem_speed);    // �X�s�[�h�ƃX�s�[�h
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_speed, _powerUpItem_bomb);     // �X�s�[�h�ƃ{��

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_bomb, _powerUpItem_bomb);      // �{���ƃ{��
}


void ItemManager::AvoidOverlap_ScoreItemAndPowerUpItem_DRY(
	std::vector<Shared<inl::ScoreItem>>& scoreItems,
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems) {

	for (decltype(auto) it : scoreItems) {
		for (decltype(auto) it2 : powerUpItems) {

			_collision_ref->CheckCollision_ScoreItemAndPowerUpItem(
				it,                // �A�C�e���P
				it2,			   // �A�C�e���Q
				it->_mesh->pos_,   // �A�C�e���P�ʒu
				it2->_mesh->pos_   // �A�C�e���Q�ʒu
			);
		}
	}
}


void ItemManager::AvoidOverlap_ScoreItemAndPowerUpItem() {

	//�@�X�R�A�A�C�e���ƃv���C���[�����A�C�e���̓����蔻��␳

	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_heal);    // ���A�C�e���Ɖ�
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_attack);  // ���A�C�e���ƍU��
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_defense); // ���A�C�e���Ɩh��
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_speed);   // ���A�C�e���ƃX�s�[�h
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_bomb);    // ���A�C�e���ƃ{��

	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_heal);   // ���A�C�e���Ɖ�
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_attack); // ���A�C�e���ƍU��
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_defense);// ���A�C�e���Ɩh��
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_speed);  // ���A�C�e���ƃX�s�[�h
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_bomb);   // ���A�C�e���ƃ{��

	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_heal);   // ��A�C�e���Ɖ�
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_attack); // ��A�C�e���ƍU��
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_defense);// ��A�C�e���Ɩh��
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_speed);  // ��A�C�e���ƃX�s�[�h
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_bomb);   // ��A�C�e���ƃ{��
}


// �`��-----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::RenderPowerUpItems(
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems,
	const Shared<dxe::Camera>& camera) {

	if (powerUpItems.empty())
		return;

	for (const auto it_scrItm_large : powerUpItems) {

		if (it_scrItm_large->_isActive)
			it_scrItm_large->_mesh->render(camera);
	}
}


void ItemManager::RenderScoreItems(
	std::vector<Shared<inl::ScoreItem>>& scoreItems,
	const Shared<dxe::Camera>& camera) {

	if (scoreItems.empty())
		return;

	for (auto it_scrItm_small : scoreItems) {

		if (it_scrItm_small->_isActive)
			it_scrItm_small->_mesh->render(camera);
	}
}


void ItemManager::Render(const Shared<dxe::Camera>& camera) {

	// �C�x���g���`��
	RenderEventHitText();

	//�@�X�R�A�A�C�e���`��
	RenderScoreItems(_scoreItem_small, camera);
	RenderScoreItems(_scoreItem_medium, camera);
	RenderScoreItems(_scoreItem_large, camera);

	//�@�v���C���[�����A�C�e���`��
	RenderPowerUpItems(_powerUpItem_heal, camera);
	RenderPowerUpItems(_powerUpItem_attack, camera);
	RenderPowerUpItems(_powerUpItem_defense, camera);
	RenderPowerUpItems(_powerUpItem_speed, camera);
	RenderPowerUpItems(_powerUpItem_bomb, camera);
}


// �X�V-----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::UpdatePowerUpItem_DRY(
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems) {

	if (powerUpItems.empty())
		return;

	for (auto it_powItm = powerUpItems.begin(); it_powItm != powerUpItems.end();) {

		// �A�C�e������
		if (!(*it_powItm)->Update((*it_powItm))) {

			it_powItm = powerUpItems.erase(it_powItm);
		}
		else {
			it_powItm++;
		}
	}
}


void ItemManager::UpdatePowerUpItem() {

	// �v���C���[�����A�C�e���X�V

	UpdatePowerUpItem_DRY(_powerUpItem_heal);
	UpdatePowerUpItem_DRY(_powerUpItem_attack);
	UpdatePowerUpItem_DRY(_powerUpItem_defense);
	UpdatePowerUpItem_DRY(_powerUpItem_speed);
	UpdatePowerUpItem_DRY(_powerUpItem_bomb);
}


void ItemManager::SpawnItemsOnEnemyDeath(const tnl::Vector3& enemyPos, const bool isEnemyDead) {

	// 1�̌��j�ŃX�|�[������A�C�e���̐����w��i�������̃��[�v�񐔁j
	int loopCount = inl::RandomValueGenerator::Int(0,4);

	for (int i = 0; i < loopCount; ++i) {

		std::vector<Shared<inl::ScoreItem>>* scoreItems = nullptr;

		// ���_�A�C�e���ɔԍ�������U��A�����_����1�I��
		int chooseScoreItem = inl::RandomValueGenerator::Int(1, 3);

		switch (chooseScoreItem)
		{
		case 1:  //�@���_�A�C�e����
			scoreItems = &_scoreItem_small;	 break;
		case 2:  //�@���_�A�C�e����
			scoreItems = &_scoreItem_medium; break;
		case 3:  //�@���_�A�C�e����
			scoreItems = &_scoreItem_large;  break;
		}


		if (!scoreItems->empty()) {

			for (auto item : *scoreItems) {

				// �G��|���A�A�C�e�����܂���������Ă��Ȃ����
				if (isEnemyDead && !item->_hasSpawned) {

					auto randomVector = 
						inl::RandomValueGenerator::Vector(-100, 100, -10, 10, -100, 100);

					item->_mesh->pos_ = enemyPos + randomVector;
					item->_hasSpawned = true;
					item->_isActive = true;
					break;
				}
			}
		}

		std::vector<Shared<inl::PowerUpItem>>* powerUpItems = nullptr;

		// �v���C���[�����A�C�e���ɔԍ�������U��A�����_���őI��
		int choosePowerUpItem = inl::RandomValueGenerator::Int(1, 5);

		switch (choosePowerUpItem)
		{
		case 1:  //�@�񕜃A�C�e��
			powerUpItems = &_powerUpItem_heal;	  break;
		case 2:  //�@�U���A�C�e��
			powerUpItems = &_powerUpItem_attack;  break;
		case 3:  //�@�h��A�C�e��
			powerUpItems = &_powerUpItem_defense; break;
		case 4:  //�@�X�s�[�h�A�C�e��
			powerUpItems = &_powerUpItem_speed;   break;
		case 5:  //�@�{���A�C�e��
			powerUpItems = &_powerUpItem_bomb;    break;
		}

		if (!powerUpItems->empty()) {

			for (auto item : *powerUpItems) {

				// �G��|���A�A�C�e�����܂���������Ă��Ȃ����
				if (isEnemyDead && !item->_hasSpawned) {

					auto randomVector = 
						inl::RandomValueGenerator::Vector(-100, 100, -10, 10, -100, 100);

					item->_mesh->pos_ = enemyPos + randomVector;
					item->_hasSpawned = true;
					item->_isActive = true;
					break;
				}
			}
		}
	}

	// ������A�������Ȃ��悤�ɕ␳
	AvoidOverlap_ScoreItemAndScoreItem();
	AvoidOverlap_PowerUpItemAndPowerUpItem();
	AvoidOverlap_ScoreItemAndPowerUpItem();
}


void ItemManager::Update(const float deltaTime) {

	//�@�X�R�A�A�C�e��
	UpdateScoreItem();
	EventHit_ScoreItemAndPlayer();

	//�@�v���C���[�����A�C�e��
	UpdatePowerUpItem();
	EventHit_PowerUpItemAndPlayer();

	//�@�C�x���g�e�L�X�g
	UpdateEventHitText(deltaTime);
}