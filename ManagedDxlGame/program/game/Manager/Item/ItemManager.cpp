#include "../../ScenePlay/Item/PowerUpItem.h"
// マネージャー----------------------------------------------------
#include "../../Manager/Enemy/EnemyManager.h"
#include "../Score/ScoreManager.h"
#include "ItemManager.h"
// ---------------------------------------------------
#include "../../ScenePlay/Collision/Collision.h"
#include "../../ScenePlay/Character/Player/Player.h"
#include "../../ScenePlay/EventMessage/EventNoticeText.h"
#include "../../ScenePlay/RandomValue/RandomValueGenerator.h"


// 得点アイテム--------------------------------------------------------------------------------------------------------------------------
std::vector<Shared<inl::ScoreItem>> ItemManager::_scoreItem_small;
std::vector<Shared<inl::ScoreItem>> ItemManager::_scoreItem_medium;
std::vector<Shared<inl::ScoreItem>> ItemManager::_scoreItem_large;

void ItemManager::CreateScoreItemPool(const std::string difficulty, const int stageId) {

	struct ItemSize {
		int s, m, l;
	};

	// Easyを基準とした各得点アイテムの総数
	int smallCount = 10;
	int mediumCount = 5;
	int largeCount = 1;

	std::map<std::string, std::vector<ItemSize>> itemCountSetting
	{
		// 各ステージごとに{}で分け、{}の中で s,m,l を設定

		// 難易度　Easy
		{"Easy",
		{{smallCount,    mediumCount,     largeCount},        //stage1 
		{smallCount,     mediumCount,     largeCount},        //stage2 
		{smallCount,     mediumCount,     largeCount},}},     //stage3
		// 難易度　Normal
		{"Normal",
		{{smallCount - 2,mediumCount - 1, largeCount * 2},    //stage1
		{smallCount - 2, mediumCount,     largeCount * 2},    //stage2
		{smallCount - 2, mediumCount,     largeCount * 2}}},  //stage3
		// 難易度　Hard
		{"Hard",
		{{smallCount - 4,mediumCount - 2, largeCount * 3},    //stage1
		{smallCount - 4, mediumCount - 2, largeCount * 3},	   //stage2
		{smallCount - 4, mediumCount - 2, largeCount * 3}}},  //stage3
		// 難易度　Lunatic
		{"Lunatic",
		{{smallCount - 6,mediumCount - 3, largeCount * 4},    //stage1
		{smallCount - 6, mediumCount - 3, largeCount * 4},	   //stage2
		{smallCount - 6, mediumCount - 3, largeCount * 4}}}   //stage3
	};

	// コンストラクタで受け取った難易度と一致するものがない場合
	if (itemCountSetting.count(difficulty) <= 0) return;

	//　1より小さい、もしくは３より大きい場合
	if (stageId < 1 || stageId > 3) return;


	ItemSize& size = itemCountSetting[difficulty][stageId - 1];

	// 小アイテム
	for (int i = 0; i < size.s; i++) {

		Shared<inl::ScoreItem> item =
			std::make_shared<inl::ScoreItem>(inl::ScoreItem::TYPE::Small);

		_scoreItem_small.push_back(item);
	}

	// 中アイテム
	for (int i = 0; i < size.m; i++) {

		Shared<inl::ScoreItem> item =
			std::make_shared<inl::ScoreItem>(inl::ScoreItem::TYPE::Medium);

		_scoreItem_medium.push_back(item);
	}

	// 大アイテム
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

		// メッシュがないまたは非アクティブ状態なら
		if (!it->_mesh || !it->_isActive)
			return;

		//　当たり判定
		if (_collision_ref->CheckCollision_PlayerAndScoreItem(it, _player_ref)) {

			switch (type)
			{
				//　得点アイテム小
			case inl::ScoreItem::TYPE::Small:
			{
				EventNotify_OnCaughtItem("得点アイテム小", "500ポイント加算。");
				ScoreManager::GetInstance().AddScoreItemScore(500);
				break;
			}
			//　得点アイテム中
			case inl::ScoreItem::TYPE::Medium:
			{
				EventNotify_OnCaughtItem("得点アイテム中", "1000ポイント加算。");
				ScoreManager::GetInstance().AddScoreItemScore(100);
				break;
			}
			//　得点アイテム大
			case inl::ScoreItem::TYPE::Large:
			{
				EventNotify_OnCaughtItem("得点アイテム大", "1500ポイント加算。");
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
	//　得点アイテム小
	EventHit_ScoreItemAndPlayer_DRY
	(_scoreItem_small, inl::ScoreItem::TYPE::Small);

	//　得点アイテム中
	EventHit_ScoreItemAndPlayer_DRY(
		_scoreItem_medium, inl::ScoreItem::TYPE::Medium);

	//　得点アイテム大
	EventHit_ScoreItemAndPlayer_DRY(
		_scoreItem_large, inl::ScoreItem::TYPE::Large);
}


void ItemManager::UpdateScoreItem_DRY(std::vector<Shared<inl::ScoreItem>>& scoreItems) {

	if (scoreItems.empty())
		return;

	for (auto it_scrItm = scoreItems.begin(); it_scrItm != scoreItems.end();) {

		if (!(*it_scrItm)->Update((*it_scrItm))) {

			// 非アクティブ化
			it_scrItm = scoreItems.erase(it_scrItm);
		}
		else {
			//　更新
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

// プレイヤー強化アイテム------------------------------------------------------------------------------------------------------------------------------------
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_heal;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_attack;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_defense;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_speed;
std::vector<Shared<inl::PowerUpItem>> ItemManager::_powerUpItem_bomb;


void ItemManager::CreatePowerUpItemPool(const std::string difficulty, const int stageId) {

	struct ItemType {
		int heal, attack, defense, speed, bomb;
	};

	// Easyを基準とした各プレイヤー強化アイテムの総数
	int heal_cnt = 10;
	int attack_cnt = 5;
	int defense_cnt = 1;
	int speed_cnt = 10;
	int bomb_cnt = 5;


	std::map<std::string, std::vector<ItemType>> itemCountSetting
	{
		// 各ステージごとに{}で分け、{}の中で s,m,l を設定
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


	// コンストラクタで受け取った難易度と一致するものがない場合
	if (itemCountSetting.count(difficulty) <= 0) return;

	//　1より小さい、もしくは３より大きい場合
	if (stageId < 1 && stageId > 3) return;


	ItemType& type = itemCountSetting[difficulty][stageId - 1];

	// 回復アイテム
	for (int i = 0; i < type.heal; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Heal);

		_powerUpItem_heal.push_back(item);
	}

	//　ATアイテム
	for (int i = 0; i < type.attack; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Attack);

		_powerUpItem_attack.push_back(item);
	}

	//　DEFアイテム
	for (int i = 0; i < type.defense; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Defense);

		_powerUpItem_defense.push_back(item);
	}

	//　スピードアイテム
	for (int i = 0; i < type.speed; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Speed);

		_powerUpItem_speed.push_back(item);
	}

	//　ボムアイテム
	for (int i = 0; i < type.bomb; i++) {

		Shared<inl::PowerUpItem> item =
			std::make_shared<inl::PowerUpItem>(inl::PowerUpItem::TYPE::Bomb);

		_powerUpItem_bomb.push_back(item);
	}
}


void ItemManager::EventHit_PowerUpItemAndPlayer_DRY(std::vector<Shared<inl::PowerUpItem>>& powerUpItems) {

	for (decltype(auto) it : powerUpItems) {

		// メッシュがないまたは非アクティブ状態なら
		if (!it->_mesh || !it->_isActive)
			return;

		// 当たり判定
		if (_collision_ref->CheckCollision_PlayerAndPowerUpItem(it, _player_ref)) {

			switch (it->_type)
			{
				//　回復アイテム
			case inl::PowerUpItem::TYPE::Heal:
			{
				//　HPが最大HPと同じかそれ以上なら処理を抜ける
				if (_player_ref->GetHP() >= _player_ref->GetMaxHP()) {
					break;
				}

				EventNotify_OnCaughtItem("回復アイテム", "HPが上昇。");

				_player_ref->HealHP(10);

				//　HP上昇後、HPが最大HP以上になったらHPを最大値でキャップ
				if (_player_ref->GetHP() > _player_ref->GetMaxHP()) {

					_player_ref->SetHP(_player_ref->GetMaxHP());
				}
				break;
			}
			//　攻撃アイテム
			case inl::PowerUpItem::TYPE::Attack:
			{
				EventNotify_OnCaughtItem("攻撃強化アイテム", "攻撃力が上昇。");

				_player_ref->AddAT(2);
				break;
			}
			//　防御アイテム
			case inl::PowerUpItem::TYPE::Defense:
			{
				EventNotify_OnCaughtItem("防御強化アイテム", "防御力が上昇。");

				_player_ref->AddDEF(2);
				break;
			}
			//　スピードアイテム
			case inl::PowerUpItem::TYPE::Speed:
			{
				EventNotify_OnCaughtItem("スピード強化アイテム", "スピードが上昇。");

				_player_ref->AddSpeed(0.1f);
				break;
			}
			//　ボムアイテム
			case inl::PowerUpItem::TYPE::Bomb:
			{
				EventNotify_OnCaughtItem("ボム", "");

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

// イベント通知----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::EventNotify_OnCaughtItem(const std::string itemName, const std::string effect) {

	//　取得したアイテムを文字列で取得
	std::string msgStr = itemName + "を獲得。 \n" + effect;

	Shared<inl::EventNoticeText> event =
		std::make_shared<inl::EventNoticeText>(msgStr, GetColor(0, 255, 0), 16, 35);

	//　通知
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

		// 時間切れでメッセージ削除
		if ((*it)->IsExpired()) {
			it = inl::EventNoticeText::_messageQueue.erase(it);
		}
		else {
			// 表示
			++it;
		}
	}
}


// 当たり判定-----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::AvoidOverlap_ScoreItemAndScoreItem_DRY(
	std::vector<Shared<inl::ScoreItem>>& scoreItems_1,
	std::vector<Shared<inl::ScoreItem>>& scoreItems_2) {

	for (decltype(auto) it : scoreItems_1) {
		for (decltype(auto) it2 : scoreItems_2) {

			_collision_ref->CheckCollision_ScoreItemAndScoreItem(
				it,                // アイテム１
				it2,			   // アイテム２
				it->_mesh->pos_,   // アイテム１位置
				it2->_mesh->pos_   // アイテム２位置
			);
		}
	}
}

void ItemManager::AvoidOverlap_ScoreItemAndScoreItem()
{
	//　スコアアイテムとスコアアイテムの当たり判定補正

	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_small, _scoreItem_small);    // 小と小
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_small, _scoreItem_medium);   // 小と中
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_small, _scoreItem_large);    // 小と大
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_medium, _scoreItem_medium);  // 中と中
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_medium, _scoreItem_large);   // 中と大
	AvoidOverlap_ScoreItemAndScoreItem_DRY(_scoreItem_large, _scoreItem_large);    // 大と大
}


void ItemManager::AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems_1,
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems_2) {

	for (decltype(auto) it : powerUpItems_1) {
		for (decltype(auto) it2 : powerUpItems_2) {

			_collision_ref->CheckCollision_PowerUpItemAndPowerUpItem(
				it,                // アイテム１
				it2,			   // アイテム２
				it->_mesh->pos_,   // アイテム１位置
				it2->_mesh->pos_   // アイテム２位置
			);
		}
	}
}

void ItemManager::AvoidOverlap_PowerUpItemAndPowerUpItem() {

	//　プレイヤー強化アイテムとプレイヤー強化アイテムの当たり判定補正

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_heal);    // 回復と回復
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_attack);  // 回復と攻撃
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_defense); // 回復と防御
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_speed);   // 回復とスピード
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_heal, _powerUpItem_bomb);    // 回復とボム

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_attack);   // 攻撃と攻撃
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_defense);  // 攻撃と防御
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_speed);    // 攻撃とスピード
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_attack, _powerUpItem_bomb);     // 攻撃とボム

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_defense, _powerUpItem_defense); // 防御と防御
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_defense, _powerUpItem_speed);   // 防御とスピード
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_defense, _powerUpItem_bomb);    // 防御とボム

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_speed, _powerUpItem_speed);    // スピードとスピード
	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_speed, _powerUpItem_bomb);     // スピードとボム

	AvoidOverlap_PowerUpItemAndPowerUpItem_DRY(_powerUpItem_bomb, _powerUpItem_bomb);      // ボムとボム
}


void ItemManager::AvoidOverlap_ScoreItemAndPowerUpItem_DRY(
	std::vector<Shared<inl::ScoreItem>>& scoreItems,
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems) {

	for (decltype(auto) it : scoreItems) {
		for (decltype(auto) it2 : powerUpItems) {

			_collision_ref->CheckCollision_ScoreItemAndPowerUpItem(
				it,                // アイテム１
				it2,			   // アイテム２
				it->_mesh->pos_,   // アイテム１位置
				it2->_mesh->pos_   // アイテム２位置
			);
		}
	}
}


void ItemManager::AvoidOverlap_ScoreItemAndPowerUpItem() {

	//　スコアアイテムとプレイヤー強化アイテムの当たり判定補正

	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_heal);    // 小アイテムと回復
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_attack);  // 小アイテムと攻撃
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_defense); // 小アイテムと防御
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_speed);   // 小アイテムとスピード
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_small, _powerUpItem_bomb);    // 小アイテムとボム

	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_heal);   // 中アイテムと回復
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_attack); // 中アイテムと攻撃
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_defense);// 中アイテムと防御
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_speed);  // 中アイテムとスピード
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_medium, _powerUpItem_bomb);   // 中アイテムとボム

	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_heal);   // 大アイテムと回復
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_attack); // 大アイテムと攻撃
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_defense);// 大アイテムと防御
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_speed);  // 大アイテムとスピード
	AvoidOverlap_ScoreItemAndPowerUpItem_DRY(_scoreItem_large, _powerUpItem_bomb);   // 大アイテムとボム
}


// 描画-----------------------------------------------------------------------------------------------------------------------------------
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

	// イベント情報描画
	RenderEventHitText();

	//　スコアアイテム描画
	RenderScoreItems(_scoreItem_small, camera);
	RenderScoreItems(_scoreItem_medium, camera);
	RenderScoreItems(_scoreItem_large, camera);

	//　プレイヤー強化アイテム描画
	RenderPowerUpItems(_powerUpItem_heal, camera);
	RenderPowerUpItems(_powerUpItem_attack, camera);
	RenderPowerUpItems(_powerUpItem_defense, camera);
	RenderPowerUpItems(_powerUpItem_speed, camera);
	RenderPowerUpItems(_powerUpItem_bomb, camera);
}


// 更新-----------------------------------------------------------------------------------------------------------------------------------
void ItemManager::UpdatePowerUpItem_DRY(
	std::vector<Shared<inl::PowerUpItem>>& powerUpItems) {

	if (powerUpItems.empty())
		return;

	for (auto it_powItm = powerUpItems.begin(); it_powItm != powerUpItems.end();) {

		// アイテム消去
		if (!(*it_powItm)->Update((*it_powItm))) {

			it_powItm = powerUpItems.erase(it_powItm);
		}
		else {
			it_powItm++;
		}
	}
}


void ItemManager::UpdatePowerUpItem() {

	// プレイヤー強化アイテム更新

	UpdatePowerUpItem_DRY(_powerUpItem_heal);
	UpdatePowerUpItem_DRY(_powerUpItem_attack);
	UpdatePowerUpItem_DRY(_powerUpItem_defense);
	UpdatePowerUpItem_DRY(_powerUpItem_speed);
	UpdatePowerUpItem_DRY(_powerUpItem_bomb);
}


void ItemManager::SpawnItemsOnEnemyDeath(const tnl::Vector3& enemyPos, const bool isEnemyDead) {

	// 1体撃破でスポーンするアイテムの数を指定（↓処理のループ回数）
	int loopCount = inl::RandomValueGenerator::Int(0,4);

	for (int i = 0; i < loopCount; ++i) {

		std::vector<Shared<inl::ScoreItem>>* scoreItems = nullptr;

		// 得点アイテムに番号を割り振り、ランダムで1つ選択
		int chooseScoreItem = inl::RandomValueGenerator::Int(1, 3);

		switch (chooseScoreItem)
		{
		case 1:  //　得点アイテム小
			scoreItems = &_scoreItem_small;	 break;
		case 2:  //　得点アイテム中
			scoreItems = &_scoreItem_medium; break;
		case 3:  //　得点アイテム大
			scoreItems = &_scoreItem_large;  break;
		}


		if (!scoreItems->empty()) {

			for (auto item : *scoreItems) {

				// 敵を倒し、アイテムがまだ生成されていなければ
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

		// プレイヤー強化アイテムに番号を割り振り、ランダムで選択
		int choosePowerUpItem = inl::RandomValueGenerator::Int(1, 5);

		switch (choosePowerUpItem)
		{
		case 1:  //　回復アイテム
			powerUpItems = &_powerUpItem_heal;	  break;
		case 2:  //　攻撃アイテム
			powerUpItems = &_powerUpItem_attack;  break;
		case 3:  //　防御アイテム
			powerUpItems = &_powerUpItem_defense; break;
		case 4:  //　スピードアイテム
			powerUpItems = &_powerUpItem_speed;   break;
		case 5:  //　ボムアイテム
			powerUpItems = &_powerUpItem_bomb;    break;
		}

		if (!powerUpItems->empty()) {

			for (auto item : *powerUpItems) {

				// 敵を倒し、アイテムがまだ生成されていなければ
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

	// 生成後、くっつかないように補正
	AvoidOverlap_ScoreItemAndScoreItem();
	AvoidOverlap_PowerUpItemAndPowerUpItem();
	AvoidOverlap_ScoreItemAndPowerUpItem();
}


void ItemManager::Update(const float deltaTime) {

	//　スコアアイテム
	UpdateScoreItem();
	EventHit_ScoreItemAndPlayer();

	//　プレイヤー強化アイテム
	UpdatePowerUpItem();
	EventHit_PowerUpItemAndPlayer();

	//　イベントテキスト
	UpdateEventHitText(deltaTime);
}