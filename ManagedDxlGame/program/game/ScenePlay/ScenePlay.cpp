#include "../DxLibEngine.h"
#include "ScenePlay.h"
#include "../SceneResult/SceneResult.h"
#include "../Manager/Item/ItemManager.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Enemy/EnemyManager.h"
#include "../Manager/Score/ScoreManager.h"
#include "Sky/SkyBox.h"
#include "Pause/PauseMenu.h"
#include "Star/ShiningStar.h"
#include "Collision/Collision.h"
#include "Camera/FreeLookCamera.h"
#include "../Utility/CustomException.h"
#include "Bullet/Enemy/EnemyBullet.h"
#include "Bullet/Enemy/BulletHell.h"
#include "Bullet/Enemy/BulletHellFactory.h"
#include "../ScenePlay/Bullet/Player/PlayerBullet.h"
#include "../ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../ScenePlay/Bullet/Enemy/HomingBullet.h"
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


ScenePlay::ScenePlay(const std::string selectedDifficulty, const int stage)
{
	Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();
	int graph = cus->TryLoadGraph("graphics/miniMap/radar.jpg", "ScenePlay::ScenePlay()");

	_miniMap_hdl = graph; // ミニマップ画像ロード

	_GAME_DIFFICULTY = selectedDifficulty;
	_STAGE_ID = stage;

	InitWeatherParticle(cus); // 雪や雨などのパーティクル

	_isShowGameBeginText = true;
	inl::PauseMenu::_isShowPauseOption = false;

	_mainCamera = std::make_shared<inl::FreeLookCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	// プレイヤー
	_player = std::make_shared<inl::Player>(_mainCamera);
	_player->SetPlayerRef(_player);

	InitPlayersBombCount(selectedDifficulty);

	// スカイボックス(天空)
	_skyBox = std::make_shared<inl::SkyBox>();

	// 当たり判定
	_collision = std::make_shared<inl::Collision>();

	// 敵に関するあらゆる処理を管理
	_enemyManager = std::make_shared<inl::EnemyManager>(_player, _mainCamera, _collision);

	// 弾幕工場
	_bltHellFactory = std::make_shared<inl::BulletHellFactory>();

	// 生成する弾幕を最初に選び、初期化
	CheckDoInit_FirstStageBulletHellLists();
	CheckDoInit_SecondStageBulletHellLists();
	CheckDoInit_ThirdStageBulletHellLists();

	// 弾幕のアクティブフラグを全てリセット
	TurnOff_FirstStageBulletHellLists();
	TurnOff_SecondStageBulletHellLists();
	TurnOff_ThirdStageBulletHellLists();

	_player->SetEnemyManagerRef(_enemyManager);

	// アイテム-----------------------------------------
	ItemManager::GetInstance().DestroyAllItems(); // 初期化
	ItemManager::GetInstance().CreateScoreItemPool(selectedDifficulty, _STAGE_ID);   // 生成
	ItemManager::GetInstance().CreatePowerUpItemPool(selectedDifficulty, _STAGE_ID); // 生成

	_screenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	_pauseMenu = std::make_shared<inl::PauseMenu>(_player);
}


void ScenePlay::InitWeatherParticle(const Shared<inl::CustomException>& cus)
{
	if (_STAGE_ID == 1) {

		auto particle = cus->TryLoadParticleBinaryFile("particle/preset/snow.bin", "ScenePlay::InitWeatherParticle()");
		_weatherParticle = particle;
	}
	else if (_STAGE_ID == 2) {

		auto particle = cus->TryLoadParticleBinaryFile("particle/preset/customRain.bin", "ScenePlay::InitWeatherParticle()");
		_weatherParticle = particle;
	}
}


// プレイヤーボム効果-------------------------------------------------------------------------------------------------------------------------
void ScenePlay::ReactivateEnemyBullets() {

	switch (_STAGE_ID)
	{
	case 1:
	{
		for (auto b : inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli) {
			b->_isActive = true;
		}
		for (auto b : inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli) {
			b->_isActive = true;
		}
		for (auto b : inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli) {
			b->_isActive = true;
		}
		break;
	}
	case 2:
	{
		for (auto b : inl::EnemyBoss_Cirno::_bullet_normal_cirno) {
			b->_isActive = true;
		}
		for (auto b : inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno) {
			b->_isActive = true;
		}
		for (auto b : inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno) {
			b->_isActive = true;
		}
		break;
	}
	case 3:
	{
		for (auto b : inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako) {
			b->_isActive = true;
		}
		for (auto b : inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako) {
			b->_isActive = true;
		}
		for (auto b : inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
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
		for (auto b : inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli) {
			b->_isActive = false;
		}
		for (auto b : inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli) {
			b->_isActive = false;
		}
		for (auto b : inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli) {
			b->_isActive = false;
		}
		break;
	}
	case 2:
	{
		for (auto b : inl::EnemyBoss_Cirno::_bullet_normal_cirno) {
			b->_isActive = false;
		}
		for (auto b : inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno) {
			b->_isActive = false;
		}
		for (auto b : inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno) {
			b->_isActive = false;
		}
		break;
	}
	case 3:
	{
		for (auto b : inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako) {
			b->_isActive = false;
		}
		for (auto b : inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako) {
			b->_isActive = false;
		}
		for (auto b : inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			b->_isActive = false;
		}
		break;
	}
	}
}


void ScenePlay::InitPlayersBombCount(const std::string selectedDifficulty) noexcept
{
	if (selectedDifficulty == "Easy")	      _player->InitBombCount(4);
	else if (selectedDifficulty == "Normal")  _player->InitBombCount(3);
	else if (selectedDifficulty == "Hard")    _player->InitBombCount(2);
	else if (selectedDifficulty == "Lunatic") _player->InitBombCount(1);
}


// 弾幕（ボスの弾）-----------------------------------------------------------------------------------------------------------------------------
void ScenePlay::CheckDoInit_FirstStageBulletHellLists()
{
	if (_STAGE_ID != 1) return;

	inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::Normal_Patchouli);

	inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::MetalFatigue_Patchouli);

	inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::SilentSerena_Patchouli);
}


void ScenePlay::TurnOff_FirstStageBulletHellLists() {

	inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli = false;
	inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli = false;
	inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli = false;
}


void ScenePlay::DestroyFirstStageBulletHellLists() {

	inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.clear();
	inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.clear();
	inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.clear();
}


void ScenePlay::RenderFirstStageBulletHellLists()
{
	if (inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli) {
		for (auto blt : inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}

		//std::string s = std::to_string(_bullet_normal_patchouli.size());　　		// 弾が何個生成されているかを表示するデバッグ用
		//DrawFormatString(1000, 50, -1, "%s個 normal", s.c_str());
	}

	if (inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli) {

		for (auto blt : inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_metalFatigue_patchouli.size());　 // 弾が何個生成されているかを表示するデバッグ用
		//DrawFormatString(1000, 50, -1, "%s個 metalFatigue", s.c_str());
	}

	if (inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli) {

		for (auto blt : inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_silentSerena_patchouli.size());　　// 弾が何個生成されているかを表示するデバッグ用
		//DrawFormatString(1000, 50, -1, "%s個 silentSerena", s.c_str());
	}
}



void ScenePlay::UpdateFirstStageBulletHellLists()
{
	if (inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli) {

		auto it = inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.begin();
		while (it != inl::EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli) {

		auto it = inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.begin();
		while (it != inl::EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (inl::EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli) {

		auto it = inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.begin();
		while (it != inl::EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.end())
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

	inl::EnemyBoss_Cirno::_bullet_normal_cirno =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::Normal_Cirno);

	inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::IcicleFall_Cirno);

	inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::Perfect_Freeze_Cirno);
}


void ScenePlay::TurnOff_SecondStageBulletHellLists() 
{
	inl::EnemyBoss_Cirno::_isUsingBullet_normal_cirno = false;
	inl::EnemyBoss_Cirno::_isUsingBullet_icicleFall_cirno = false;
	inl::EnemyBoss_Cirno::_isUsingBullet_perfectFreeze_cirno = false;
}


void ScenePlay::DestroySecondStageBulletHellLists()
{
	inl::EnemyBoss_Cirno::_bullet_normal_cirno.clear();
	inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno.clear();
	inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.clear();
}



void ScenePlay::RenderSecondStageBulletHellLists()
{
	if (inl::EnemyBoss_Cirno::_isUsingBullet_normal_cirno) {
		for (auto blt : inl::EnemyBoss_Cirno::_bullet_normal_cirno) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (inl::EnemyBoss_Cirno::_isUsingBullet_icicleFall_cirno) {

		for (auto blt : inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_icicleFall_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (inl::EnemyBoss_Cirno::_isUsingBullet_perfectFreeze_cirno) {

		for (auto blt : inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_perfectFreeze_cirno.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}
}



void ScenePlay::UpdateSecondStageBulletHellLists()
{
	if (inl::EnemyBoss_Cirno::_isUsingBullet_normal_cirno) {

		auto it = inl::EnemyBoss_Cirno::_bullet_normal_cirno.begin();
		while (it != inl::EnemyBoss_Cirno::_bullet_normal_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}

	if (inl::EnemyBoss_Cirno::_isUsingBullet_icicleFall_cirno) {

		auto it = inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno.begin();
		while (it != inl::EnemyBoss_Cirno::_bullet_icicleFall_cirno.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}

	if (inl::EnemyBoss_Cirno::_isUsingBullet_perfectFreeze_cirno) {

		auto it = inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.begin();
		while (it != inl::EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.end())
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

	inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::Normal_Suwako);

	inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::IronRingOfMoriya_Suwako);

	inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako =
		_bltHellFactory->CreateBulletHell(inl::BulletHell::TYPE::KeroChan_StandsFirm_AgainstTheStorm_Suwako);
}


void ScenePlay::TurnOff_ThirdStageBulletHellLists() {
	inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako = false;
	inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako = false;
	inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = false;
}


void ScenePlay::DestroyThirdStageBulletHellLists() {
	inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.clear();
	inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.clear();
	inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.clear();
}


void ScenePlay::RenderThirdStageBulletHellLists() {

	if (inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako) {
		for (auto blt : inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako) {
			if (blt->_isActive)	blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_normal_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako) {

		for (auto blt : inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_ironRingOfMoriya_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}

	if (inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako) {

		for (auto blt : inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako) {
			if (blt->_isActive)  blt->Render(_mainCamera);
		}
		//std::string s = std::to_string(_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size());
		//DrawFormatString(1000, 50, -1, "%s個", s.c_str());
	}
}



void ScenePlay::UpdateThirdStageBulletHellLists() {

	if (inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako) {

		auto it = inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.begin();
		while (it != inl::EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako) {

		auto it = inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.begin();
		while (it != inl::EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
	if (inl::EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako) {

		auto it = inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.begin();
		while (it != inl::EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.end())
		{
			if (!(*it)->_isActive) {
				(*it)->_isActive = true;
			}
			it++;
		}
	}
}


// 描画-----------------------------------------------------------------------------------------------------------------------------
void ScenePlay::RenderBeginText() noexcept {

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
	if (inl::PauseMenu::_isShowPauseOption) {
		_pauseMenu->Render();
		_bgAlpha_whenCall_pauseMenu = 100;
	}
	else {
		_bgAlpha_whenCall_pauseMenu = 255;
	}
}


void ScenePlay::RenderStageGrindGround() noexcept
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


	//　天候パーティクル----------------------------------------------------------------------
	if (_STAGE_ID == 1 || _STAGE_ID == 2) {

		dxe::DirectXRenderBegin();
		_weatherParticle->render(_mainCamera);
		dxe::DirectXRenderEnd();
	}
	if (_STAGE_ID == 3) {
		// お星さま描画
		inl::ShiningStar s;	s.Render();
	}


	//　ボムパーティクル----------------------------------------------------------------------
	if (_player->GetIsTriggeredBombEffect()) {

		dxe::DirectXRenderBegin();
		inl::Player::_bombParticle->setPosition(_player->_mesh->pos_);
		inl::Player::_bombParticle->start();
		inl::Player::_bombParticle->render(_mainCamera);
		dxe::DirectXRenderEnd();
	}

	//　弾幕----------------------------------------------------------------------------------
	RenderFirstStageBulletHellLists();
	RenderSecondStageBulletHellLists();
	RenderThirdStageBulletHellLists();

	//　スコア--------------------------------------------------------------------------------
	ScoreManager::GetInstance().RenderTotalScore();

	_screenEffect->renderEnd();


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//　ステージのグリッド線
	RenderStageGrindGround();

	// ミニマップ
	DrawRotaGraph(_miniMap_centerPos.x, _miniMap_centerPos.y, _miniMap_extendRate, 0, _miniMap_hdl, 1);
	RenderEnemyRadarOnMiniMap();

	//　ポーズメニュー
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

void ScenePlay::UpdateShowBeginTextTimer(const float deltaTime) noexcept
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


void ScenePlay::Update(const float deltaTime) {

	SetDeltaTime(deltaTime);

	_skyBox->Update();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LALT)) {

		if (!inl::PauseMenu::_isShowPauseOption) { 
			
			// ポーズメニューオープン
			inl::PauseMenu::_isShowPauseOption = true;
		}
	}

	if (inl::PauseMenu::_isShowPauseOption) {   
		_pauseMenu->Update();
	}
	else {

		_enemyManager->Update(deltaTime);		//　敵

		_player->Update(deltaTime);		        //　プレイヤー

		UpdateFirstStageBulletHellLists();      //　弾幕
		UpdateSecondStageBulletHellLists();
		UpdateThirdStageBulletHellLists();
	}

	UpdateShowBeginTextTimer(deltaTime);
	inl::PlayerBullet::ClampBulletPowerRate();
}