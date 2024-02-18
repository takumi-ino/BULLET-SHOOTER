#include "ScenePlay.h"
#include "Pause/PauseMenu.h"
#include "../Manager/Item/ItemManager.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Enemy/EnemyManager.h"
#include "Collision/Collision.h"
#include "Sky/SkyBox.h"
#include "Ground/HeightMap.h"
#include "Star/ShiningStar.h"
#include "Bullet/Player/PlayerBullet.h"
#include "Bullet/Enemy/EnemyBullet.h"
#include "../ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../ScenePlay/Bullet/Enemy/HomingBullet.h"
#include "../SceneResult/SceneResult.h"
#include "../Manager/Score/ScoreManager.h"
#include "Camera/FreeLookCamera.h"
#include "Bullet/Enemy/BulletHell.h"
#include "Bullet/Enemy/BulletHellFactory.h"
#include "Character/Enemy/EnemyZako/EnemyZakoBox.h"

Shared<dxe::Particle> ScenePlay::_weather_particle;

// ステージ1
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_normal_patchouli;
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_silentSerena_patchouli;
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_metalFatigue_patchouli;
bool ScenePlay::_isUsingBullet_normal_patchouli;
bool ScenePlay::_isUsingBullet_silentSerena_patchouli;
bool ScenePlay::_isUsingBullet_metalFatigue_patchouli;

// ステージ2
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_normal_cirno;
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_icicleFall_cirno;
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_perfectFreeze_cirno;
bool ScenePlay::_isUsingBullet_normal_cirno;
bool ScenePlay::_isUsingBullet_icicleFall_cirno;
bool ScenePlay::_isUsingBullet_perfectFreeze_cirno;

// ステージ3
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_normal_suwako;
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_ironRingOfMoriya_suwako;
std::vector<Shared<EnemyBullet>> ScenePlay::_bullet_keroChanStandsFirmAgainstTheStorm_suwako;
bool ScenePlay::_isUsingBullet_normal_suwako;
bool ScenePlay::_isUsingBullet_ironRingOfMoriya_suwako;
bool ScenePlay::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako;


bool ScenePlay::_isRenderPlayersBombEffect;
bool ScenePlay::_isShowBeginGameText = false;

int ScenePlay::_STAGE_ID;


ScenePlay::ScenePlay(const std::string selected_difficulty, const int stage) {

	_isShowBeginGameText = true;
	PauseMenu::_isShowPauseOption = false;

	_STAGE_ID = stage;

	_mainCamera = std::make_shared<FreeLookCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	if (_STAGE_ID == 1)
		_weather_particle = std::make_shared<dxe::Particle>("particle/preset/snow.bin");
	else if (_STAGE_ID == 2)
		_weather_particle = std::make_shared<dxe::Particle>("particle/preset/customRain.bin");


	// プレイヤー
	_player = std::make_shared<Player>(_mainCamera);
	_player->SetPlayerRef(_player);

	InitPlayersBombCount(selected_difficulty);

	// スカイボックス(天空)
	_skyBox = std::make_shared<SkyBox>(_STAGE_ID);

	// 当たり判定
	_collision = std::make_shared<Collision>();

	// 敵に関するあらゆる処理を全て管理
	_enemyManager = std::make_shared<EnemyManager>(_STAGE_ID, _player, _mainCamera, _collision, selected_difficulty);

	ItemManager::GetInstance().DestroyAllItems();

	ItemManager::GetInstance().CreateScoreItemPool(selected_difficulty, _STAGE_ID);
	ItemManager::GetInstance().CreatePowerUpItemPool(selected_difficulty, _STAGE_ID);

	TurnOff_FirstStageBulletHellLists();
	TurnOff_SecondStageBulletHellLists();
	TurnOff_ThirdStageBulletHellLists();

	_bltHellFactory = std::make_shared<BulletHellFactory>();
	// 生成する弾幕を最初に選び、初期化
	CheckDoInit_FirstStageBulletHellLists();
	CheckDoInit_SecondStageBulletHellLists();
	CheckDoInit_ThirdStageBulletHellLists();

	_screen_effect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	_pauseMenu = std::make_shared<PauseMenu>(_STAGE_ID, selected_difficulty);

	// 画面左下の索敵レーダーの画像
	_miniMap_hdl = LoadGraph("graphics/miniMap/radar.jpg");
}



// プレイヤーボム効果-------------------------------------------------------------------------------------------------------------------------
void ScenePlay::ReactivateEnemyBullets() {

	switch (_STAGE_ID)
	{
	case 1:
	{
		for (auto b : _bullet_normal_patchouli) {
			b->_isActive = true;
		}
		for (auto b : _bullet_metalFatigue_patchouli) {
			b->_isActive = true;
		}
		for (auto b : _bullet_silentSerena_patchouli) {
			b->_isActive = true;
		}
		break;
	}
	case 2:
	{
		for (auto b : _bullet_normal_cirno) {
			b->_isActive = true;
		}
		for (auto b : _bullet_icicleFall_cirno) {
			b->_isActive = true;
		}
		for (auto b : _bullet_perfectFreeze_cirno) {
			b->_isActive = true;
		}
		break;
	}
	case 3:
	{
		for (auto b : _bullet_normal_suwako) {
			b->_isActive = true;
		}
		for (auto b : _bullet_ironRingOfMoriya_suwako) {
			b->_isActive = true;
		}
		for (auto b : _bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			b->_isActive = true;
		}
		break;
	}
	}
}

void ScenePlay::DeactivateAllEnemyBullets() {

	switch (_STAGE_ID)
	{
	case 1:
	{
		for (auto b : _bullet_normal_patchouli) {
			b->_isActive = false;
		}
		for (auto b : _bullet_metalFatigue_patchouli) {
			b->_isActive = false;
		}
		for (auto b : _bullet_silentSerena_patchouli) {
			b->_isActive = false;
		}
		break;
	}
	case 2:
	{
		for (auto b : _bullet_normal_cirno) {
			b->_isActive = false;
		}
		for (auto b : _bullet_icicleFall_cirno) {
			b->_isActive = false;
		}
		for (auto b : _bullet_perfectFreeze_cirno) {
			b->_isActive = false;
		}
		break;
	}
	case 3:
	{
		for (auto b : _bullet_normal_suwako) {
			b->_isActive = false;
		}
		for (auto b : _bullet_ironRingOfMoriya_suwako) {
			b->_isActive = false;
		}
		for (auto b : _bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			b->_isActive = false;
		}
		break;
	}
	}
}


void ScenePlay::InitPlayersBombCount(const std::string& selected_difficulty)
{
	if (selected_difficulty == "Easy")	       _player->SetBombCount(4);
	else if (selected_difficulty == "Normal")  _player->SetBombCount(3);
	else if (selected_difficulty == "Hard")    _player->SetBombCount(2);
	else if (selected_difficulty == "Lunatic") _player->SetBombCount(1);
}


// 弾幕（ボスの弾）-----------------------------------------------------------------------------------------------------------------------------
void ScenePlay::CheckDoInit_FirstStageBulletHellLists()
{
	if (_STAGE_ID != 1) return;

	_bullet_normal_patchouli =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Normal_Patchouli);

	_bullet_metalFatigue_patchouli =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::MetalFatigue_Patchouli);

	_bullet_silentSerena_patchouli =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::SilentSerena_Patchouli);
}


void ScenePlay::TurnOff_FirstStageBulletHellLists() {

	_isUsingBullet_normal_patchouli = false;
	_isUsingBullet_metalFatigue_patchouli = false;
	_isUsingBullet_silentSerena_patchouli = false;
}


void ScenePlay::DestroyFirstStageBulletHellLists() {

	_bullet_normal_patchouli.clear();
	_bullet_metalFatigue_patchouli.clear();
	_bullet_silentSerena_patchouli.clear();
}


void ScenePlay::CheckDoRender_FirstStageBulletHellLists()
{
	if (_isUsingBullet_normal_patchouli) {
		for (auto blt : _bullet_normal_patchouli) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_patchouli.size());
		//DrawFormatString(1000, 50, -1, "%s個 normal", s.c_str());
	}

	if (_isUsingBullet_metalFatigue_patchouli) {

		for (auto blt : _bullet_metalFatigue_patchouli) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_metalFatigue_patchouli.size());
		//DrawFormatString(1000, 50, -1, "%s個 metalFatigue", s.c_str());
	}

	if (_isUsingBullet_silentSerena_patchouli) {

		for (auto blt : _bullet_silentSerena_patchouli) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_silentSerena_patchouli.size());
		//DrawFormatString(1000, 50, -1, "%s個 silentSerena", s.c_str());
	}
}



void ScenePlay::CheckDoUpdate_FirstStageBulletHellLists()
{
	if (_isUsingBullet_normal_patchouli) {

		auto it = _bullet_normal_patchouli.begin();
		while (it != _bullet_normal_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (_isUsingBullet_metalFatigue_patchouli) {

		auto it = _bullet_metalFatigue_patchouli.begin();
		while (it != _bullet_metalFatigue_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (_isUsingBullet_silentSerena_patchouli) {

		auto it = _bullet_silentSerena_patchouli.begin();
		while (it != _bullet_silentSerena_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
}


void ScenePlay::CheckDoInit_SecondStageBulletHellLists()
{
	if (_STAGE_ID != 2) return;

	_bullet_normal_cirno =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Normal_Cirno);

	_bullet_icicleFall_cirno =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::IcicleFall_Cirno);

	_bullet_perfectFreeze_cirno =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Perfect_Freeze_Cirno);
}


void ScenePlay::TurnOff_SecondStageBulletHellLists() {
	_isUsingBullet_normal_cirno = false;
	_isUsingBullet_icicleFall_cirno = false;
	_isUsingBullet_perfectFreeze_cirno = false;
}

void ScenePlay::DestroySecondStageBulletHellLists() {

	_bullet_normal_cirno.clear();
	_bullet_icicleFall_cirno.clear();
	_bullet_perfectFreeze_cirno.clear();
}



void ScenePlay::CheckDoRender_SecondStageBulletHellLists()
{
	if (_isUsingBullet_normal_cirno) {
		for (auto blt : _bullet_normal_cirno) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (_isUsingBullet_icicleFall_cirno) {

		for (auto blt : _bullet_icicleFall_cirno) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_icicleFall_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (_isUsingBullet_perfectFreeze_cirno) {

		for (auto blt : _bullet_perfectFreeze_cirno) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_perfectFreeze_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}
}



void ScenePlay::CheckDoUpdate_SecondStageBulletHellLists()
{
	if (_isUsingBullet_normal_cirno) {

		auto it = _bullet_normal_cirno.begin();
		while (it != _bullet_normal_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}

	if (_isUsingBullet_icicleFall_cirno) {

		auto it = _bullet_icicleFall_cirno.begin();
		while (it != _bullet_icicleFall_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}

	if (_isUsingBullet_perfectFreeze_cirno) {

		auto it = _bullet_perfectFreeze_cirno.begin();
		while (it != _bullet_perfectFreeze_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
}


void ScenePlay::CheckDoInit_ThirdStageBulletHellLists() {

	if (_STAGE_ID != 3) return;

	_bullet_normal_suwako =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Normal_Suwako);

	_bullet_ironRingOfMoriya_suwako =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::IronRingOfMoriya_Suwako);

	_bullet_keroChanStandsFirmAgainstTheStorm_suwako =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::KeroChan_StandsFirm_AgainstTheStorm_Suwako);
}


void ScenePlay::TurnOff_ThirdStageBulletHellLists() {
	_isUsingBullet_normal_suwako = false;
	_isUsingBullet_ironRingOfMoriya_suwako = false;
	_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = false;
}


void ScenePlay::DestroyThirdStageBulletHellLists() {
	_bullet_normal_suwako.clear();
	_bullet_ironRingOfMoriya_suwako.clear();
	_bullet_keroChanStandsFirmAgainstTheStorm_suwako.clear();
}


void ScenePlay::CheckDoRender_ThirdStageBulletHellLists() {

	if (_isUsingBullet_normal_suwako) {
		for (auto blt : _bullet_normal_suwako) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (_isUsingBullet_ironRingOfMoriya_suwako) {

		for (auto blt : _bullet_ironRingOfMoriya_suwako) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_ironRingOfMoriya_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako) {

		for (auto blt : _bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}
}



void ScenePlay::CheckDoUpdate_ThirdStageBulletHellLists() {

	if (_isUsingBullet_normal_suwako) {

		auto it = _bullet_normal_suwako.begin();
		while (it != _bullet_normal_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (_isUsingBullet_ironRingOfMoriya_suwako) {

		auto it = _bullet_ironRingOfMoriya_suwako.begin();
		while (it != _bullet_ironRingOfMoriya_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako) {

		auto it = _bullet_keroChanStandsFirmAgainstTheStorm_suwako.begin();
		while (it != _bullet_keroChanStandsFirmAgainstTheStorm_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
}


// 描画-----------------------------------------------------------------------------------------------------------------------------
void ScenePlay::RenderBeginText() {

	SetFontSize(85);
	DrawStringEx(530, 150, GetColor(0, 0, 255), "Begin");
	SetFontSize(20);
}


void ScenePlay::RenderEnemyRadarOnMiniMap() {

	// 左下のミニマップレーダー
	std::vector<tnl::Vector3> enemy_pos_list = _enemyManager->GetEnemyZakoPosition();

	for (auto e_pos = enemy_pos_list.begin(); e_pos != enemy_pos_list.end(); e_pos++) {

		tnl::Vector3 screen_pos = tnl::Vector3::ConvertToScreen(
			{ (*e_pos).x, (*e_pos).y, (*e_pos).z },
			miniMap_center_pos.x,
			miniMap_center_pos.y,
			_mainCamera->view_,
			_mainCamera->proj_
		);

		// 敵位置を緑の円で描画
		DrawCircleAA(
			(float)std::clamp((int)screen_pos.x, miniMap_center_pos.x, 160),
			(float)std::clamp((int)screen_pos.y, miniMap_center_pos.y, 660),
			2, 10, _radarColor);
	}
}


void ScenePlay::RenderPauseMenu()
{
	if (PauseMenu::_isShowPauseOption) {
		_pauseMenu->Render();
		_bgAlpha_when_call_pauseMenu = 100;
	}
	else {
		_bgAlpha_when_call_pauseMenu = 255;
	}
}


void ScenePlay::Render() {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bgAlpha_when_call_pauseMenu);

	_screen_effect->renderBegin();

	_skyBox->Render(_mainCamera);
	_player->Render(_mainCamera);
	_enemyManager->Render(_mainCamera);


	if (_STAGE_ID == 1 || _STAGE_ID == 2) {

		dxe::DirectXRenderBegin();
		_weather_particle->render(_mainCamera);
		dxe::DirectXRenderEnd();
	}
	if (_STAGE_ID == 3) {
		// お星さま描画
		ShiningStar s;	s.Render();
	}


	if (_isRenderPlayersBombEffect) {

		dxe::DirectXRenderBegin();
		Player::_bomb_particle->setPosition(_player->_mesh->pos_);
		Player::_bomb_particle->start();
		Player::_bomb_particle->render(_mainCamera);
		dxe::DirectXRenderEnd();
	}

	CheckDoRender_FirstStageBulletHellLists();
	CheckDoRender_SecondStageBulletHellLists();
	CheckDoRender_ThirdStageBulletHellLists();

	ScoreManager::GetInstance().RenderTotalScore();

	_screen_effect->renderEnd();


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// グリッド線
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawGridGround(_mainCamera, 80, 25);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ミニマップ
	DrawRotaGraph(miniMap_center_pos.x, miniMap_center_pos.y, 0.035, 0, _miniMap_hdl, 1);
	RenderEnemyRadarOnMiniMap();

	RenderPauseMenu();
}


// 更新-------------------------------------------------------------------------------------------------------------------------
void ScenePlay::MoveToNextStage(const int stage, const std::string difficulty) {

	switch (stage)
	{
	case 1:
	{
		auto mgr = SceneManager::GetInstance();

		if (difficulty == "Easy")	      mgr->ChangeScene(new ScenePlay("Easy", stage + 1));
		else if (difficulty == "Normal")  mgr->ChangeScene(new ScenePlay("Normal", stage + 1));
		else if (difficulty == "Hard")	  mgr->ChangeScene(new ScenePlay("Hard", stage + 1));
		else if (difficulty == "Lunatic") mgr->ChangeScene(new ScenePlay("Lunatic", stage + 1));
		return;
	}
	case 2:
	{
		auto mgr = SceneManager::GetInstance();

		if (difficulty == "Easy")	      mgr->ChangeScene(new ScenePlay("Easy", stage + 1));
		else if (difficulty == "Normal")  mgr->ChangeScene(new ScenePlay("Normal", stage + 1));
		else if (difficulty == "Hard")	  mgr->ChangeScene(new ScenePlay("Hard", stage + 1));
		else if (difficulty == "Lunatic") mgr->ChangeScene(new ScenePlay("Lunatic", stage + 1));
		return;
	}
	}
}


float ScenePlay::_showBeginText_timer;
float ScenePlay::_deltaTime_ref;

void ScenePlay::UpdateShowBeginTextTimer(const float deltaTime)
{
	_showBeginText_timer += deltaTime;

	if (_isShowBeginGameText) {
		RenderBeginText();

		if (_showBeginText_timer > 2.0f) {
			_isShowBeginGameText = false;
			_showBeginText_timer = 0;
		}
	}
}


void ScenePlay::Update(float deltaTime) {

	SetDeltaTime(deltaTime);

	_skyBox->Update();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LALT)) {

		if (!PauseMenu::_isShowPauseOption)
			PauseMenu::_isShowPauseOption = true;
	}

	if (PauseMenu::_isShowPauseOption) {
		_pauseMenu->Update();
	}
	else {
		_enemyManager->Update(deltaTime);
		_player->Update(deltaTime);
		CheckDoUpdate_FirstStageBulletHellLists();
		CheckDoUpdate_SecondStageBulletHellLists();
		CheckDoUpdate_ThirdStageBulletHellLists();
	}

	UpdateShowBeginTextTimer(deltaTime);
}