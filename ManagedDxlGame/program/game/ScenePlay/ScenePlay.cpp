#include "../DxLibEngine.h"
#include "ScenePlay.h"
#include "Pause/PauseMenu.h"
#include "../Manager/Item/ItemManager.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Enemy/EnemyManager.h"
#include "Collision/Collision.h"
#include "Sky/SkyBox.h"
#include "Ground/HeightMap.h"
#include "Star/ShiningStar.h"
#include "Bullet/Enemy/EnemyBullet.h"
#include "../ScenePlay/Bullet/Player/PlayerBullet.h"
#include "../ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../ScenePlay/Bullet/Enemy/HomingBullet.h"
#include "../SceneResult/SceneResult.h"
#include "../Manager/Score/ScoreManager.h"
#include "Camera/FreeLookCamera.h"
#include "Bullet/Enemy/BulletHell.h"
#include "Bullet/Enemy/BulletHellFactory.h"
#include "Character/Enemy/EnemyZako/EnemyZakoBox.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"


Shared<dxe::Particle> ScenePlay::_weatherParticle;

int ScenePlay::_STAGE_ID;
std::string  ScenePlay::_GAME_DIFFICULTY;


namespace {

	// 画面左下のミニマップ --------------------------------------------
	const float               _miniMap_extendRate{ 0.035f };
	const tnl::Vector2i       _miniMap_centerPos{ 120,600 };

	// ミニマップ上に表示する索敵レーダー ------------------------------
	const int                 _miniMapRadar_clampX{ 160 };
	const int                 _miniMapRadar_clampY{ 660 };
	const int                 _miniMapRadar_edgeNum{ 4 };
	const int                 _radarColor = GetColor(0, 255, 0);

	// ゲーム開始時に表示する「 Begin 」テキスト-------------------------
	const int                 _beginText_posX{ 535 };
	const int                 _beginText_posY{ 150 };
	const float               _showBeginTextDuration{ 2.0f };

	// ステージ上に表示するグリッド--------------------------------------
	const int                 _gridRowNum{ 25 };
	const float               _gridSquareSize{ 80.0f };
}


ScenePlay::ScenePlay(const std::string selected_difficulty, const int stage)
{
	_isShowGameBeginText = true;
	PauseMenu::_isShowPauseOption = false;

	_GAME_DIFFICULTY = selected_difficulty;
	_STAGE_ID = stage;

	_mainCamera = std::make_shared<FreeLookCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	InitWeatherParticle();

	_miniMap_hdl = LoadGraph("graphics/miniMap/radar.jpg"); // ミニマップ画像ロード

	// プレイヤー
	_player = std::make_shared<Player>(_mainCamera);
	_player->SetPlayerRef(_player);

	InitPlayersBombCount(selected_difficulty);

	// スカイボックス(天空)
	_skyBox = std::make_shared<SkyBox>();

	// 当たり判定
	_collision = std::make_shared<Collision>();

	// 敵に関するあらゆる処理を全て管理
	_enemyManager = std::make_shared<EnemyManager>(_player, _mainCamera, _collision);

	_bltHellFactory = std::make_shared<BulletHellFactory>();
	// 生成する弾幕を最初に選び、初期化
	CheckDoInit_FirstStageBulletHellLists();
	CheckDoInit_SecondStageBulletHellLists();
	CheckDoInit_ThirdStageBulletHellLists();

	TurnOff_FirstStageBulletHellLists();
	TurnOff_SecondStageBulletHellLists();
	TurnOff_ThirdStageBulletHellLists();

	_player->SetEnemyManagerRef(_enemyManager);

	ItemManager::GetInstance().DestroyAllItems();

	ItemManager::GetInstance().CreateScoreItemPool(selected_difficulty, _STAGE_ID);
	ItemManager::GetInstance().CreatePowerUpItemPool(selected_difficulty, _STAGE_ID);

	_screenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	_pauseMenu = std::make_shared<PauseMenu>(_player);
}


void ScenePlay::InitWeatherParticle()
{
	if (_STAGE_ID == 1)
		_weatherParticle = std::make_shared<dxe::Particle>("particle/preset/snow.bin");
	else if (_STAGE_ID == 2)
		_weatherParticle = std::make_shared<dxe::Particle>("particle/preset/customRain.bin");
}


// プレイヤーボム効果-------------------------------------------------------------------------------------------------------------------------
void ScenePlay::ReactivateEnemyBullets() {

	switch (_STAGE_ID)
	{
	case 1:
	{
		for (auto b : EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli) {
			b->_isActive = true;
		}
		for (auto b : EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli) {
			b->_isActive = true;
		}
		for (auto b : EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli) {
			b->_isActive = true;
		}
		break;
	}
	case 2:
	{
		for (auto b : EnemyBoss_Cirno::_bullet_normal_cirno) {
			b->_isActive = true;
		}
		for (auto b : EnemyBoss_Cirno::_bullet_icicleFall_cirno) {
			b->_isActive = true;
		}
		for (auto b : EnemyBoss_Cirno::_bullet_perfectFreeze_cirno) {
			b->_isActive = true;
		}
		break;
	}
	case 3:
	{
		for (auto b : EnemyBoss_MoriyaSuwako::_bullet_normal_suwako) {
			b->_isActive = true;
		}
		for (auto b : EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako) {
			b->_isActive = true;
		}
		for (auto b : EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
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
		for (auto b : EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli) {
			b->_isActive = false;
		}
		for (auto b : EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli) {
			b->_isActive = false;
		}
		for (auto b : EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli) {
			b->_isActive = false;
		}
		break;
	}
	case 2:
	{
		for (auto b : EnemyBoss_Cirno::_bullet_normal_cirno) {
			b->_isActive = false;
		}
		for (auto b : EnemyBoss_Cirno::_bullet_icicleFall_cirno) {
			b->_isActive = false;
		}
		for (auto b : EnemyBoss_Cirno::_bullet_perfectFreeze_cirno) {
			b->_isActive = false;
		}
		break;
	}
	case 3:
	{
		for (auto b : EnemyBoss_MoriyaSuwako::_bullet_normal_suwako) {
			b->_isActive = false;
		}
		for (auto b : EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako) {
			b->_isActive = false;
		}
		for (auto b : EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			b->_isActive = false;
		}
		break;
	}
	}
}


void ScenePlay::InitPlayersBombCount(const std::string selected_difficulty)
{
	if (selected_difficulty == "Easy")	       _player->InitBombCount(4);
	else if (selected_difficulty == "Normal")  _player->InitBombCount(3);
	else if (selected_difficulty == "Hard")    _player->InitBombCount(2);
	else if (selected_difficulty == "Lunatic") _player->InitBombCount(1);
}


// 弾幕（ボスの弾）-----------------------------------------------------------------------------------------------------------------------------
void ScenePlay::CheckDoInit_FirstStageBulletHellLists()
{
	if (_STAGE_ID != 1) return;

	EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Normal_Patchouli);

	EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::MetalFatigue_Patchouli);

	EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::SilentSerena_Patchouli);
}


void ScenePlay::TurnOff_FirstStageBulletHellLists() {

	EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli = false;
	EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli = false;
	EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli = false;
}


void ScenePlay::DestroyFirstStageBulletHellLists() {

	EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.clear();
	EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.clear();
	EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.clear();
}


void ScenePlay::CheckDoRender_FirstStageBulletHellLists()
{
	if (EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli) {
		for (auto blt : EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_patchouli.size());
		//DrawFormatString(1000, 50, -1, "%s個 normal", s.c_str());
	}

	if (EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli) {

		for (auto blt : EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_metalFatigue_patchouli.size());
		//DrawFormatString(1000, 50, -1, "%s個 metalFatigue", s.c_str());
	}

	if (EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli) {

		for (auto blt : EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_silentSerena_patchouli.size());
		//DrawFormatString(1000, 50, -1, "%s個 silentSerena", s.c_str());
	}
}



void ScenePlay::CheckDoUpdate_FirstStageBulletHellLists()
{
	if (EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli) {

		auto it = EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.begin();
		while (it != EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli) {

		auto it = EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.begin();
		while (it != EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli) {

		auto it = EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.begin();
		while (it != EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.end())
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

	EnemyBoss_Cirno::_bullet_normal_cirno =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Normal_Cirno);

	EnemyBoss_Cirno::_bullet_icicleFall_cirno =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::IcicleFall_Cirno);

	EnemyBoss_Cirno::_bullet_perfectFreeze_cirno =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Perfect_Freeze_Cirno);
}


void ScenePlay::TurnOff_SecondStageBulletHellLists() {
	EnemyBoss_Cirno::_isUsingBullet_normal_cirno = false;
	EnemyBoss_Cirno::_isUsingBullet_icicleFall_cirno = false;
	EnemyBoss_Cirno::_isUsingBullet_perfectFreeze_cirno = false;
}


void ScenePlay::DestroySecondStageBulletHellLists() {

	EnemyBoss_Cirno::_bullet_normal_cirno.clear();
	EnemyBoss_Cirno::_bullet_icicleFall_cirno.clear();
	EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.clear();
}



void ScenePlay::CheckDoRender_SecondStageBulletHellLists()
{
	if (EnemyBoss_Cirno::_isUsingBullet_normal_cirno) {
		for (auto blt : EnemyBoss_Cirno::_bullet_normal_cirno) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (EnemyBoss_Cirno::_isUsingBullet_icicleFall_cirno) {

		for (auto blt : EnemyBoss_Cirno::_bullet_icicleFall_cirno) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_icicleFall_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (EnemyBoss_Cirno::_isUsingBullet_perfectFreeze_cirno) {

		for (auto blt : EnemyBoss_Cirno::_bullet_perfectFreeze_cirno) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_perfectFreeze_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}
}



void ScenePlay::CheckDoUpdate_SecondStageBulletHellLists()
{
	if (EnemyBoss_Cirno::_isUsingBullet_normal_cirno) {

		auto it = EnemyBoss_Cirno::_bullet_normal_cirno.begin();
		while (it != EnemyBoss_Cirno::_bullet_normal_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}

	if (EnemyBoss_Cirno::_isUsingBullet_icicleFall_cirno) {

		auto it = EnemyBoss_Cirno::_bullet_icicleFall_cirno.begin();
		while (it != EnemyBoss_Cirno::_bullet_icicleFall_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}

	if (EnemyBoss_Cirno::_isUsingBullet_perfectFreeze_cirno) {

		auto it = EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.begin();
		while (it != EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.end())
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

	EnemyBoss_MoriyaSuwako::_bullet_normal_suwako =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::Normal_Suwako);

	EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::IronRingOfMoriya_Suwako);

	EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako =
		_bltHellFactory->CreateBulletHell(BulletHell::TYPE::KeroChan_StandsFirm_AgainstTheStorm_Suwako);
}


void ScenePlay::TurnOff_ThirdStageBulletHellLists() {
	EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako = false;
	EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako = false;
	EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = false;
}


void ScenePlay::DestroyThirdStageBulletHellLists() {
	EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.clear();
	EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.clear();
	EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.clear();
}


void ScenePlay::CheckDoRender_ThirdStageBulletHellLists() {

	if (EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako) {
		for (auto blt : EnemyBoss_MoriyaSuwako::_bullet_normal_suwako) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako) {

		for (auto blt : EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_ironRingOfMoriya_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako) {

		for (auto blt : EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}
}



void ScenePlay::CheckDoUpdate_ThirdStageBulletHellLists() {

	if (EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako) {

		auto it = EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.begin();
		while (it != EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako) {

		auto it = EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.begin();
		while (it != EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako) {

		auto it = EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.begin();
		while (it != EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.end())
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

	SetFontSize(88);
	DrawStringEx(_beginText_posX, _beginText_posY, GetColor(0, 0, 255), "Begin");
	SetFontSize(DEFAULT_FONT_SIZE);
}


void ScenePlay::RenderEnemyRadarOnMiniMap() {

	// 左下のミニマップレーダー
	std::vector<tnl::Vector3> enemyPosList = _enemyManager->GetEnemyZakoPosition();

	for (auto enemyPos = enemyPosList.begin(); enemyPos != enemyPosList.end(); enemyPos++) {

		tnl::Vector3 screenPos = tnl::Vector3::ConvertToScreen(
			{ (*enemyPos).x, (*enemyPos).y, (*enemyPos).z },
			(float)_miniMap_centerPos.x,
			(float)_miniMap_centerPos.y,
			_mainCamera->view_,
			_mainCamera->proj_
		);

		// 敵位置を緑の円で描画
		DrawCircleAA(
			(float)std::clamp((int)screenPos.x, _miniMap_centerPos.x, _miniMapRadar_clampX),
			(float)std::clamp((int)screenPos.y, _miniMap_centerPos.y, _miniMapRadar_clampY),
			2,
			_miniMapRadar_edgeNum,
			_radarColor
		);
	}
}


void ScenePlay::RenderPauseMenu()
{
	if (PauseMenu::_isShowPauseOption) {
		_pauseMenu->Render();
		_bgAlpha_whenCall_pauseMenu = 100;
	}
	else {
		_bgAlpha_whenCall_pauseMenu = 255;
	}
}


void ScenePlay::RenderStageGrindGround()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawGridGround(_mainCamera, _gridSquareSize, _gridRowNum);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void ScenePlay::Render() {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _bgAlpha_whenCall_pauseMenu);

	_screenEffect->renderBegin();

	_skyBox->Render(_mainCamera);
	_player->Render(_mainCamera);
	_enemyManager->Render(_mainCamera);


	if (_STAGE_ID == 1 || _STAGE_ID == 2) {

		dxe::DirectXRenderBegin();
		_weatherParticle->render(_mainCamera);
		dxe::DirectXRenderEnd();
	}
	if (_STAGE_ID == 3) {
		// お星さま描画
		ShiningStar s;	s.Render();
	}


	if (_player->GetIsTriggeredBombEffect()) {

		dxe::DirectXRenderBegin();
		Player::_bombParticle->setPosition(_player->_mesh->pos_);
		Player::_bombParticle->start();
		Player::_bombParticle->render(_mainCamera);
		dxe::DirectXRenderEnd();
	}

	CheckDoRender_FirstStageBulletHellLists();
	CheckDoRender_SecondStageBulletHellLists();
	CheckDoRender_ThirdStageBulletHellLists();

	ScoreManager::GetInstance().RenderTotalScore();

	_screenEffect->renderEnd();


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	RenderStageGrindGround();

	// ミニマップ
	DrawRotaGraph(_miniMap_centerPos.x, _miniMap_centerPos.y, _miniMap_extendRate, 0, _miniMap_hdl, 1);
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


float ScenePlay::_deltaTime;

void ScenePlay::UpdateShowBeginTextTimer(const float deltaTime)
{
	_beginTextTimer += deltaTime;

	if (_isShowGameBeginText) {

		RenderBeginText();

		if (_beginTextTimer > _showBeginTextDuration) {

			_isShowGameBeginText = false;
			_beginTextTimer = 0;
		}
	}
}


void ScenePlay::Update(float deltaTime) {

	SetDeltaTime(deltaTime);

	_skyBox->Update();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LALT)) {

		if (!PauseMenu::_isShowPauseOption) {
			PauseMenu::_isShowPauseOption = true;
		}
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

	if (PlayerBullet::_bulletPowerRate >= 5.0f) {
		PlayerBullet::_bulletPowerRate = 5.0f;
	}
}