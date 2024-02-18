#include <random>
#include "EnemyManager.h"
#include "../Item/ItemManager.h"
#include "../../Manager/Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../../ScenePlay/ScenePlay.h"
#include "../../ScenePlay/Character/Player/Player.h"
#include "../../ScenePlay/Collision/Collision.h"
#include "../../ScenePlay/Bullet/Player/PlayerBullet.h"
#include "../../ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../../ScenePlay/Bullet/Enemy/HomingBullet.h"
#include "../../ScenePlay/Bullet/Enemy/BulletHell.h"
#include "../../ScenePlay/Bullet/Enemy/EnemyBullet.h"
#include "../../ScenePlay/Character/Enemy/EnemyZako/EnemyZakoBox.h"
#include "../../ScenePlay/Character/Enemy/EnemyZako/EnemyZakoDome.h"
#include "../../ScenePlay/Character/Enemy/EnemyZako/EnemyZakoCylinder.h"
#include "../../ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../../ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "../../ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"
#include "../../Manager/Score/ScoreManager.h"
#include "../../ScenePlay/Character/Enemy/EnemyBossBase.h"
#include "../../ScenePlay/Character/Enemy/EnemyZakoBase.h"
#include "../../SceneResult/SceneResult.h"
#include "../../ScenePlay/EventMessage/EventNoticeText.h"
#include"../../ScenePlay/Item/ItemBase.h"


// 初期化処理-------------------------------------------------------------------------------------------------------------------------
EnemyManager::EnemyManager(
	const int stageID, const Shared<Player>& player, const Shared<dxe::Camera>& camera,
	const Shared<Collision>& collision, const std::string difficulty)
	: _player_ref(player), _mainCamera_ref(camera), _collision_ref(collision), _SELECTED_LEVEL(difficulty), _stageID(stageID) {

	_isInitializedBossInfo = false;


	SoundManager::GetInstance().LoadStageBGM(_stageID);
	SoundManager::GetInstance().PlayStageBGM(false); // ボス(true)か雑魚(false)か

	_alertSE_hdl = LoadSoundMem("sound/se/bossAppears.mp3");

	LoadEnemyDataFromCsv(difficulty);

	SetMaxEnemySpawnCount();

	InitEnemyZakoInfo();
	InitEnemyBossInfo();
	SetSpawnEnemyBoss();

	_zakoBox_ref = std::make_shared<EnemyZakoBox>();
	_itemManager = std::make_shared<ItemManager>(_player_ref, _collision_ref);
	this->AttachItemManagerInstance(_itemManager);
}


void EnemyManager::SetMaxEnemySpawnCount()
{
	if (_SELECTED_LEVEL == "Easy")         _maxEnemySpawnCount_PerInterval = 2;
	else if (_SELECTED_LEVEL == "Normal")  _maxEnemySpawnCount_PerInterval = 2;
	else if (_SELECTED_LEVEL == "Hard")    _maxEnemySpawnCount_PerInterval = 3;
	else if (_SELECTED_LEVEL == "Lunatic") _maxEnemySpawnCount_PerInterval = 4;
}



void EnemyManager::LoadEnemyDataFromCsv(const std::string& difficulty)
{
	// CSVから敵データのロード
	if (_enemyZakoData_map.empty() || _enemyBossData_map.empty()) {
		_csvLoader = std::make_shared<CsvLoader>(difficulty);

		_enemyZakoData_map = _csvLoader->LoadEnemyZakoInfos("csv/EnemyZakoInfos.csv");
		_enemyBossData_map = _csvLoader->LoadEnemyBossInfos("csv/EnemyBossInfos.csv");
	}
}



void EnemyManager::InitEnemyZakoInfo() {

	// ロードしたエネミーデータ分ループ
	for (auto enemy = _enemyZakoData_map.begin(); enemy != _enemyZakoData_map.end(); enemy++) {

		switch ((*enemy).first) // id
		{
		case 0: // EnemyZakoBoxデータ取得
			_sEnemy_zakoBox_info._charaMoveSpeed = (*enemy).second._charaMoveSpeed;
			_sEnemy_zakoBox_info._maxTotalEnemySpawnCount = (*enemy).second._maxTotalEnemySpawnCount;
			_sEnemy_zakoBox_info._name = (*enemy).second._name;
			_sEnemy_zakoBox_info._scale = (*enemy).second._scale;
			_sEnemy_zakoBox_info._stageID = (*enemy).second._stageID;
			_sEnemy_zakoBox_info._reloadTimeInterval = (*enemy).second._reloadTimeInterval;
			_sEnemy_zakoBox_info._bulletFireInterval = (*enemy).second._bulletFireInterval;
			_sEnemy_zakoBox_info._bulletMoveSpeed = (*enemy).second._bulletMoveSpeed;

			_remainingZakoBox_spawnCount = _sEnemy_zakoBox_info._maxTotalEnemySpawnCount;
			break;
		case 1: // EnemyZakoDomeデータ取得
			_sEnemy_zakoDome_info._charaMoveSpeed = (*enemy).second._charaMoveSpeed;
			_sEnemy_zakoDome_info._maxTotalEnemySpawnCount = (*enemy).second._maxTotalEnemySpawnCount;
			_sEnemy_zakoDome_info._name = (*enemy).second._name;
			_sEnemy_zakoDome_info._scale = (*enemy).second._scale;
			_sEnemy_zakoDome_info._stageID = (*enemy).second._stageID;
			_sEnemy_zakoDome_info._reloadTimeInterval = (*enemy).second._reloadTimeInterval;
			_sEnemy_zakoDome_info._bulletFireInterval = (*enemy).second._bulletFireInterval;
			_sEnemy_zakoDome_info._bulletMoveSpeed = (*enemy).second._bulletMoveSpeed;

			_remainingZakoDome_spawnCount = _sEnemy_zakoDome_info._maxTotalEnemySpawnCount;
			break;
		case 2: // EnemyZakoCylinderデータ取得
			_sEnemy_zakoCylinder_info._charaMoveSpeed = (*enemy).second._charaMoveSpeed;
			_sEnemy_zakoCylinder_info._maxTotalEnemySpawnCount = (*enemy).second._maxTotalEnemySpawnCount;
			_sEnemy_zakoCylinder_info._name = (*enemy).second._name;
			_sEnemy_zakoCylinder_info._scale = (*enemy).second._scale;
			_sEnemy_zakoCylinder_info._stageID = (*enemy).second._stageID;
			_sEnemy_zakoCylinder_info._reloadTimeInterval = (*enemy).second._reloadTimeInterval;
			_sEnemy_zakoCylinder_info._bulletFireInterval = (*enemy).second._bulletFireInterval;
			_sEnemy_zakoCylinder_info._bulletMoveSpeed = (*enemy).second._bulletMoveSpeed;

			_remainingZakoCylinder_spawnCount = _sEnemy_zakoCylinder_info._maxTotalEnemySpawnCount;
			break;
		}
	}
}



void EnemyManager::InitEnemyBossInfo() {

	for (auto boss = _enemyBossData_map.begin(); boss != _enemyBossData_map.end(); boss++) {

		switch ((*boss).first) // id
		{
		case 0: // パチュリー データ取得
			_sBoss_PatchouliKnowledge_info._charaMoveSpeed = (*boss).second._charaMoveSpeed;
			_sBoss_PatchouliKnowledge_info._name = (*boss).second._name;
			_sBoss_PatchouliKnowledge_info._scale = (*boss).second._scale;
			_sBoss_PatchouliKnowledge_info._stageID = (*boss).second._stageID;
			break;
		case 1: // チルノ データ取得
			_sBoss_Cirno_info._charaMoveSpeed = (*boss).second._charaMoveSpeed;
			_sBoss_Cirno_info._name = (*boss).second._name;
			_sBoss_Cirno_info._scale = (*boss).second._scale;
			_sBoss_Cirno_info._stageID = (*boss).second._stageID;
			break;
		case 2: // 洩矢諏訪子 データ取得
			_sBoss_MoriyaSuwako_info._charaMoveSpeed = (*boss).second._charaMoveSpeed;
			_sBoss_MoriyaSuwako_info._name = (*boss).second._name;
			_sBoss_MoriyaSuwako_info._scale = (*boss).second._scale;
			_sBoss_MoriyaSuwako_info._stageID = (*boss).second._stageID;
			break;
		}
	}
}



void EnemyManager::SetSpawnEnemyBoss() {

	if (_stageID == _sBoss_PatchouliKnowledge_info._stageID) {

		auto boss_patchouli = std::make_shared<EnemyBoss_PatchouliKnowledge>(_enemyBossData_map[0], _player_ref, _mainCamera_ref);

		boss_patchouli->_isDead = false;
		boss_patchouli->_charaMoveSpeed = _sBoss_PatchouliKnowledge_info._charaMoveSpeed;
		boss_patchouli->_name = _sBoss_PatchouliKnowledge_info._name;
		boss_patchouli->_scale = _sBoss_PatchouliKnowledge_info._scale;

		_enemy_boss_list.push_back(boss_patchouli);
	}
	else if (_stageID == _sBoss_Cirno_info._stageID) {

		auto boss_cirno = std::make_shared<EnemyBoss_Cirno>(_enemyBossData_map[1], _player_ref, _mainCamera_ref);

		boss_cirno->_isDead = false;
		boss_cirno->_charaMoveSpeed = _sBoss_Cirno_info._charaMoveSpeed;
		boss_cirno->_name = _sBoss_Cirno_info._name;
		boss_cirno->_scale = _sBoss_Cirno_info._scale;

		_enemy_boss_list.push_back(boss_cirno);
	}
	else if (_stageID == _sBoss_MoriyaSuwako_info._stageID) {

		auto boss_suwako = std::make_shared<EnemyBoss_MoriyaSuwako>(_enemyBossData_map[2], _player_ref, _mainCamera_ref);

		boss_suwako->_isDead = false;
		boss_suwako->_charaMoveSpeed = _sBoss_MoriyaSuwako_info._charaMoveSpeed;
		boss_suwako->_name = _sBoss_MoriyaSuwako_info._name;
		boss_suwako->_scale = _sBoss_MoriyaSuwako_info._scale;

		_enemy_boss_list.push_back(boss_suwako);
	}
}


// -------------------------------------------------------------------------------------------------------------------------------------------
void EnemyManager::CheckDoSpawnEnemy() {

	switch (_stageID)
	{
	case 1:
	{
		// 残りの敵スポーン回数
		if (_remainingZakoBox_spawnCount > 0) {

			// 残りの敵スポーン回数と最大同時生成数を比較し、少ない方を使用
			int enemiesToSpawnNow = min(_remainingZakoBox_spawnCount, _maxEnemySpawnCount_PerInterval);

			for (int i = 0; i < enemiesToSpawnNow; i++) {

				auto enemy_box = std::make_shared<EnemyZakoBox>(_enemyZakoData_map[0], _player_ref, _mainCamera_ref);

				enemy_box->_isDead = false;
				enemy_box->SetMeshInfo();
				enemy_box->InitBulletFactoryInstance();

				_enemy_zako_list.push_back(enemy_box);	// EnemyZakoBox 生成

			}
			// プレイヤークラスでエネミーのリストを参照する変数
			_player_ref->SetEnemyZakoListRef(_enemy_zako_list);

			_remainingZakoBox_spawnCount -= enemiesToSpawnNow; // スポーンした分だけ残りの生成可能回数から引き算
		}
		else
			// ザコ敵を全て倒したら
			SummonBoss();
		break;
	}
	case 2:
	{
		if (_remainingZakoDome_spawnCount > 0) {

			int enemiesToSpawnNow = min(_remainingZakoDome_spawnCount, _maxEnemySpawnCount_PerInterval);

			for (int i = 0; i < enemiesToSpawnNow; i++) {

				auto enemy_dome = std::make_shared<EnemyZakoDome>(_enemyZakoData_map[1], _player_ref, _mainCamera_ref);

				enemy_dome->_isDead = false;
				enemy_dome->SetMeshInfo();
				enemy_dome->InitBulletFactoryInstance();

				_enemy_zako_list.push_back(enemy_dome);	// EnemyZakoDome 生成
			}
			_player_ref->SetEnemyZakoListRef(_enemy_zako_list);

			_remainingZakoDome_spawnCount -= enemiesToSpawnNow;
		}
		else
			SummonBoss();
		break;
	}
	case 3:
	{
		if (_remainingZakoCylinder_spawnCount > 0) {

			int enemiesToSpawnNow = min(_remainingZakoCylinder_spawnCount, _maxEnemySpawnCount_PerInterval);

			for (int i = 0; i < enemiesToSpawnNow; i++) {

				auto enemy_cylinder = std::make_shared<EnemyZakoCylinder>(_enemyZakoData_map[2], _player_ref, _mainCamera_ref);

				enemy_cylinder->_isDead = false;
				enemy_cylinder->SetMeshInfo();
				enemy_cylinder->InitBulletFactoryInstance();

				_enemy_zako_list.push_back(enemy_cylinder); // EnemyZakoCylinder 生成 
			}
			_player_ref->SetEnemyZakoListRef(_enemy_zako_list);

			_remainingZakoCylinder_spawnCount -= enemiesToSpawnNow;
		}
		else
			SummonBoss();
		break;
	}
	}
}


void EnemyManager::SummonBoss()
{
	if (!_isInitializedBossInfo) {
		auto boss = _enemy_boss_list.begin();
		(*boss)->SetMeshInfo();
		(*boss)->InitBulletHellInstance();

		PlaySoundMem(_alertSE_hdl, DX_PLAYTYPE_BACK, TRUE);

		// 引数はボスバトルか否かを指定。true でボスの処理のみ実行
		SoundManager::GetInstance().DestroyStageBGM(true);
		SoundManager::GetInstance().PlayStageBGM(true);

		_isShowBossAppearanceText = true;

		_player_ref->SetEnemyBossListRef(_enemy_boss_list);
		_isInitializedBossInfo = true;
	}
	else
		_summon_boss = true;
}


//当たり判定---------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyManager::SetCollisionPairList() {

	EnemyZakoCollisionPairLists();
	EnemyBossCollisionPairLists();
	BulletHellCollisionPairLists();
}

//キャラクター同士-----------------------------------------------------
void EnemyManager::EnemyZakoCollisionPairLists()
{
	for (auto it : _enemy_zako_list) {

		if (it->_mesh != nullptr) {

			// プレイヤーとZakoエネミー各種
			_collision_ref->CheckCollision_PlayerAndEnemyZako(
				_player_ref, it, _player_ref->GetPos(), it->_mesh->pos_);
		}
		// プレイヤーの弾とZakoエネミー各種
		for (Shared<PlayerBullet> pb : _player_ref->_straight_bullets_player) {

			if (_collision_ref->CheckCollision_PlayerBulletAndEnemyZako(pb, it)) {

				pb->_isActive = false;

				if (it->DecreaseHP(Player::_at, _mainCamera_ref)) {
					ScoreManager::GetInstance().AddHitBulletScore(100);
					EnemyZakoBase::_isNoticedPlayer = true;
					it->_timeFrom_noticedPlayer = 0.0f;
				}
				else {
					OnEnemyKilled(it->_name); // 名前を通知
				}
			}
		}

		// Zakoエネミー同士の当たり判定
		for (auto it2 : _enemy_zako_list) {

			if (it == it2) continue;

			_collision_ref->CheckCollision_EnemyAndEnemy(it, it2, it->_mesh->pos_, it2->_mesh->pos_);
		}
	}
}


void EnemyManager::EnemyBossCollisionPairLists()
{
	for (auto it : _enemy_boss_list) {

		if (it->_mesh != nullptr) {
			// プレイヤーとBossエネミー各種
			_collision_ref->CheckCollision_PlayerAndEnemyBoss(
				_player_ref, it, _player_ref->GetPos(), it->_mesh->pos_);
		}

		// プレイヤーの弾とBossエネミー各種
		for (Shared<PlayerBullet> pb : _player_ref->_straight_bullets_player) {

			if (_collision_ref->CheckCollision_PlayerBulletAndEnemyBoss(pb, it)) {

				pb->_isActive = false;
				it->DecreaseBossHP(Player::_at);
				ScoreManager::GetInstance().AddHitBulletScore(100);
			}
			else {
				static bool isNotify = false;

				if (it->_isDead && !isNotify) {
					OnEnemyKilled(it->_name); // 名前を通知
					isNotify = true;
				}
			}
		}
	}
}

//　雑魚敵の直行弾とプレイヤー------------------------------------------------------------------------------
void EnemyManager::EnemyZakoStraightBullet_CollisionPairLists(std::list<Shared<StraightBullet>>& straightBullets_list) {

	if (!_player_ref) return;

	if (straightBullets_list.size() >= 1) {
		for (auto blt : straightBullets_list) {
			tnl::Vector3 prev_pos = (*blt)._mesh->pos_;
			if (_collision_ref->CheckCollision_EnemyStraightBulletAndPlayer(blt, _player_ref)) {

				if (_enemy_zako_list.empty()) return;

				for (auto ene : _enemy_zako_list) {

					if ((*ene)._name == "Box") {
						if (_player_ref->DecreaseHP(EnemyZakoBox::_at - Player::_def)) {
							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
					if ((*ene)._name == "Dome") {
						if (_player_ref->DecreaseHP(EnemyZakoDome::_at - Player::_def)) {
							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}

					if ((*ene)._name == "Cylinder") {
						if (_player_ref->DecreaseHP(EnemyZakoCylinder::_at - Player::_def)) {
							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
				}

				blt->_isActive = false;
			}
		}
	}
}


//　雑魚敵の追跡弾とプレイヤー-----------------------------------------------------------------------------------------
void EnemyManager::EnemyZakoHomingBullet_CollisionPairLists(std::list<Shared<HomingBullet>>& homingBullets_list) {

	if (!_player_ref) return;

	if (homingBullets_list.size() >= 1) {
		for (auto blt : homingBullets_list) {
			tnl::Vector3 prev_pos = (*blt)._mesh->pos_;
			if (_collision_ref->CheckCollision_EnemyHomingBulletAndPlayer(blt, _player_ref)) {

				if (_enemy_zako_list.empty()) return;

				for (auto ene : _enemy_zako_list) {

					if ((*ene)._name == "Box") {
						if (_player_ref->DecreaseHP(EnemyZakoBox::_at - Player::_def)) {
							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
					if ((*ene)._name == "Dome") {
						if (_player_ref->DecreaseHP(EnemyZakoDome::_at - Player::_def)) {
							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}

					if ((*ene)._name == "Cylinder") {
						if (_player_ref->DecreaseHP(EnemyZakoCylinder::_at - Player::_def)) {
							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
				}

				blt->_isActive = false;
			}
		}
	}
}


//　弾幕（ボスの弾）とプレイヤーー---------------------------------------------------------------------------------
void EnemyManager::BulletHellCollisionPairLists()
{
	// 雑魚がフィールド上に１体もいない場合のみ
	if (_enemy_zako_list.empty()) {
		// パチュリー------------------------------------------------------------------
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_normal_patchouli);
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_metalFatigue_patchouli);
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_silentSerena_patchouli);
		// チルノー--------------------------------------------------------------------
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_normal_cirno);
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_icicleFall_cirno);
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_perfectFreeze_cirno);
		// 諏訪子ー--------------------------------------------------------------------
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_normal_suwako);
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_ironRingOfMoriya_suwako);
		BulletHellCollisionPairLists_DRY(ScenePlay::_bullet_keroChanStandsFirmAgainstTheStorm_suwako);
	}
}


void EnemyManager::BulletHellCollisionPairLists_DRY(std::vector<Shared<EnemyBullet>>& bullets_vector) {

	if (bullets_vector.size() >= 1) {
		for (auto blt : bullets_vector) {
			tnl::Vector3 prev_pos = (*blt)._mesh->pos_;
			if (_collision_ref->CheckCollision_BulletHellBulletsAndPlayer(
				blt, _player_ref, tnl::Vector3{ 30,30,30 }, prev_pos)) {

				if (_enemy_boss_list.empty()) return;

				for (auto it : _enemy_boss_list) {

					if (it->_name == "Patchouli Knowledge") {
						if (_player_ref->DecreaseHP(EnemyBoss_PatchouliKnowledge::_at - Player::_def)) {

							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
					if (it->_name == "Cirno") {
						if (_player_ref->DecreaseHP(EnemyBoss_Cirno::_at - Player::_def)) {

							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
					if (it->_name == "Moriya Suwako") {
						if (_player_ref->DecreaseHP(EnemyBoss_MoriyaSuwako::_at - Player::_def)) {

							Player::_isInvincible = true;
							Player::PlayDamageHitSE();
						}
					}
				}

				blt->_isActive = false;
			}
		}
	}
}


//　ボスの位置座標の通知-------------------------------------------------------------------------------------------------------------------
void EnemyManager::AttachItemManagerInstance(const Shared<ItemManager>& observer) {
	_observerItems.push_back(observer);
}


void EnemyManager::NotifyEnemyPosition_ToItemManager() {

	for (const auto& observer : _observerItems) {
		observer->SpawnItemsOnEnemyDeath(_enemyZako_position_ref, _isEnemyZako_dead_ref);
	}
}


void EnemyManager::SendEnemyPosition(const tnl::Vector3& new_position, const bool isEnemyDead) {
	_enemyZako_position_ref = new_position;
	_isEnemyZako_dead_ref = isEnemyDead;
	NotifyEnemyPosition_ToItemManager();
}


//　-------------------------------------------------------------------------------------------------------------------
const std::vector<tnl::Vector3>& EnemyManager::GetEnemyZakoPosition() {

	_enemyPosList.clear();
	for (auto& it_zako : _enemy_zako_list) {

		if (it_zako)
			_enemyPosList.push_back(it_zako->_mesh->pos_);
		// 例外
		else throw std::logic_error("Null enemy encountered in _enemy_zako_list");
	}

	return _enemyPosList;
}


const tnl::Vector3& EnemyManager::GetEnemyBossPosition() {

	for (auto& it_boss : _enemy_boss_list) {

		// 有効なボスのみ
		if (!it_boss->_mesh) return tnl::Vector3();

		_enemyBossPos = it_boss->_mesh->pos_;
	}
	return _enemyBossPos;
}


bool EnemyManager::IsKilledStageBoss() {

	if (_enemy_zako_list.empty() && _enemy_boss_list.empty())
		return true;
	return false;
}


void EnemyManager::ShowAppearsBossText() {

	SetFontSize(70);
	DrawFormatString(400, 200, GetColor(255, 0, 0), "Boss Appears");
	SetFontSize(20);
}


void EnemyManager::UpdateBossAppearanceTextTimer(const float& deltaTime)
{
	if (_isShowBossAppearanceText) {

		_showBossAppearanceText_timer += deltaTime;
		ShowAppearsBossText();

		if (_showBossAppearanceText_timer > 3.f) {
			_showBossAppearanceText_timer = 0.0f;
			_isShowBossAppearanceText = false;
		}
	}
}

// 敵殺傷イベント通知------------------------------------------------------------------------------------------------------------------------------------------
void EnemyManager::OnEnemyKilled(const std::string enemy_name) {

	std::string msg = enemy_name + "を撃破 ";

	Shared<EventNoticeText> event_msg = std::make_shared<EventNoticeText>(msg, GetColor(255, 0, 0), 16, 30);

	EventNoticeText::_message_queue.push_back(event_msg);
}


void EnemyManager::RenderEventHitText() const
{
	int index = 0;
	for (auto msg : EventNoticeText::_message_queue) {
		msg->Render(index);
		index++;
	}
}

void EnemyManager::UpdateEventHitText(const float& deltaTime)
{
	// イベント通知
	for (auto msg : EventNoticeText::_message_queue) {
		msg->Update(deltaTime);
	}
	// 表示時間が切れたメッセージを削除する
	auto it = EventNoticeText::_message_queue.begin();
	while (it != EventNoticeText::_message_queue.end()) {

		if ((*it)->IsExpired()) {
			it = EventNoticeText::_message_queue.erase(it);
		}
		else {
			++it;
		}
	}
}

// ステージ移動------------------------------------------------------------------------------------------------------------------------------------------
bool EnemyManager::SeqMoveToNextStage(const float deltaTime) {

	SeqMoveToResult(deltaTime);

	if (IsKilledStageBoss() && !_isDefeatedAllStageEnemy) {

		SetFontSize(80);
		DrawString(450, 200, "Stage Clear", GetColor(0, 255, 0));
		SetFontSize(40);
		DrawString(480, 450, "Move to Next Stage!", GetColor(0, 255, 0));


		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			SoundManager::GetInstance().DestroyStageBGM(false);

			ScenePlay* sp = new ScenePlay();
			sp->MoveToNextStage(_stageID, _SELECTED_LEVEL);
			delete sp; sp = nullptr;

			return true;
		}
	}
	return false;
}


bool EnemyManager::SeqMoveToResult(const float deltaTime) {

	if (IsKilledStageBoss() && _stageID == 3) {

		_isDefeatedAllStageEnemy = true;

		SetFontSize(50);
		DrawString(450, 200, "Accomplished", GetColor(0, 255, 0));
		SetFontSize(30);
		DrawString(480, 450, "Move to Result!", GetColor(0, 255, 0));


		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			SoundManager::GetInstance().DestroyStageBGM(false);

			ScenePlay::DestroyThirdStageBulletHellLists();

			auto mgr = SceneManager::GetInstance();
			int total = ScoreManager::GetInstance().GetTotalScore();

			if (_SELECTED_LEVEL == "Easy")	       mgr->ChangeScene(new SceneResult("Easy", total));
			else if (_SELECTED_LEVEL == "Normal")  mgr->ChangeScene(new SceneResult("Normal", total));
			else if (_SELECTED_LEVEL == "Hard")    mgr->ChangeScene(new SceneResult("Hard", total));
			else if (_SELECTED_LEVEL == "Lunatic") mgr->ChangeScene(new SceneResult("Lunatic", total));

			return true;
		}
	}

	return false;
}

// 描画------------------------------------------------------------------------------------------------------------------------------------------
void EnemyManager::Render(const Shared<dxe::Camera>& camera) const {

	_itemManager->Render(camera);

	RenderEventHitText();

	// ザコ
	for (const auto& enemy : _enemy_zako_list) {
		if (enemy) enemy->Render(_mainCamera_ref);
	}

	// ボス
	if (_enemy_zako_list.empty() && !_enemy_boss_list.empty()) {

		if (_summon_boss) {

			for (const auto& boss : _enemy_boss_list) {
				if (boss) boss->Render(_mainCamera_ref);
			}
		}
	}
}

// 更新------------------------------------------------------------------------------------------------------------------------------------------
void EnemyManager::UpdateEnemyBossList(const float& deltaTime)
{
	if (_enemy_zako_list.empty() && !_enemy_boss_list.empty()) {

		if (_summon_boss) {

			for (auto it_boss = _enemy_boss_list.begin(); it_boss != _enemy_boss_list.end();) {

				if ((*it_boss)->Update(deltaTime) == false) {

					_player_ref->EraseEnemyBossListRef(*it_boss);
					it_boss = _enemy_boss_list.erase(it_boss);
					_summon_boss = false;
				}
				else {
					it_boss++;
				}
			}
		}
	}
}


void EnemyManager::UpdateEnemyZakoList(const float& deltaTime)
{
	if (!_enemy_zako_list.empty()) {

		for (auto it_zako = _enemy_zako_list.begin(); it_zako != _enemy_zako_list.end();) {

			if ((*it_zako)->Update(deltaTime) == false) {

				// 関数内でItemManagerクラスに通知
				SendEnemyPosition((*it_zako)->_mesh->pos_, (*it_zako)->_isDead);

				_player_ref->EraseEnemyZakoListRef(*it_zako);
				it_zako = _enemy_zako_list.erase(it_zako);
			}
			else {

				it_zako++;
			}
		}
	}
}


float EnemyManager::_showBossAppearanceText_timer;


void EnemyManager::Update(const float& deltaTime) {

	_sequence.update(deltaTime);

	_itemManager->Update(deltaTime);

	if (_enemy_zako_list.empty()) {

		CheckDoSpawnEnemy();
	}

	SetCollisionPairList();

	UpdateEventHitText(deltaTime);

	UpdateBossAppearanceTextTimer(deltaTime);

	UpdateEnemyZakoList(deltaTime);

	UpdateEnemyBossList(deltaTime);
}