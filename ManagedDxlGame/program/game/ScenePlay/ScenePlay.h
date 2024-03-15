#pragma once
#include "../Manager/Scene/SceneBase.h"

class Player;
class SkyBox;
class HeightMap;
class Collision;
class ItemManager;
class EnemyManager;
class Score;
class PauseMenu;
class FreeLookCamera;
class EnemyBullet;
class BulletHellFactory;
class StraightBullet;
class HomingBullet;


class ScenePlay : public SceneBase
{
public:

	ScenePlay() {}
	ScenePlay(const std::string selectedDifficulty, const int stage);

	// ステージ移動----------------------------------------------------------------------
	void MoveToNextStage(const int stage, const std::string difficulty);

	// 弾幕初期化（プレイ開始前に該当のステージのボスの弾幕のみ生成）--------------------
	void CheckDoInit_FirstStageBulletHellLists();   // ステージ１
	void CheckDoInit_SecondStageBulletHellLists();  // ステージ２
	void CheckDoInit_ThirdStageBulletHellLists();   // ステージ３

	// プレイヤーのボム効果による弾の無効化・有効化--------------------------------------
	static void ReactivateEnemyBullets();
	static void DeactivateAllEnemyBullets();

	// リザルトに飛ぶときに最終ステージの弾幕を解放--------------------------------------
	static void DestroyThirdStageBulletHellLists();

	// Getter ----------------------------------------------------------------------
	static const int GetStageID() noexcept { return _STAGE_ID; }                       // ステージID
	static const std::string GetGameDifficulty() noexcept { return _GAME_DIFFICULTY; } // 選択難易度
	static const float GetDeltaTime() noexcept { return _deltaTime; };                 // デルタタイム

private:

	// パーティクル ----------------------------------------------------
	void InitWeatherParticle();

	// Setter ----------------------------------------------------------
	void SetDeltaTime(const float deltaTime) { _deltaTime = deltaTime; };

	//　各弾幕のアクティブ状態を false に一括リセット-------------------
	void TurnOff_FirstStageBulletHellLists();
	void TurnOff_SecondStageBulletHellLists();
	void TurnOff_ThirdStageBulletHellLists();

	//　弾幕描画。現在使用中のもののみ----------------------------------
	void CheckDoRender_FirstStageBulletHellLists();
	void CheckDoRender_SecondStageBulletHellLists();
	void CheckDoRender_ThirdStageBulletHellLists();

	//　弾幕更新。現在使用中のもののみ----------------------------------
	void CheckDoUpdate_FirstStageBulletHellLists();
	void CheckDoUpdate_SecondStageBulletHellLists();
	void CheckDoUpdate_ThirdStageBulletHellLists();

	//　弾幕を解放------------------------------------------------------
	void DestroyFirstStageBulletHellLists();
	void DestroySecondStageBulletHellLists();

	// Beginテキスト----------------------------------------------------
	void RenderBeginText() noexcept;
	void UpdateShowBeginTextTimer(const float deltaTime) noexcept;

	// ボム-------------------------------------------------------------
	void InitPlayersBombCount(const std::string selectedDifficulty) noexcept;

	// ミニマップ-------------------------------------------------------
	void RenderEnemyRadarOnMiniMap();
	void RenderStageGrindGround() noexcept;

	// ポーズメニュー---------------------------------------------------
	void RenderPauseMenu();

	// 描画・更新ー---------------------------------------------------
	void Render() override;
	void Update(const float deltaTime) override;

public:

	static Shared<dxe::Particle> _weatherParticle;

private:

	Shared<FreeLookCamera>    _mainCamera = nullptr;

	Shared<Player>            _player = nullptr;

	Shared<EnemyManager>      _enemyManager = nullptr;

	Shared<SkyBox>            _skyBox = nullptr;

	Shared<Collision>         _collision = nullptr;

	Shared<Score>             _score = nullptr;

	Shared<dxe::ScreenEffect> _screenEffect = nullptr;

	Shared<BulletHellFactory> _bltHellFactory = nullptr;

	Shared<PauseMenu>         _pauseMenu = nullptr;

private:

	// Getterで使用する値----------------------------------------------
	static int          _STAGE_ID;
	static std::string  _GAME_DIFFICULTY;
	static float        _deltaTime;

	// ポーズ画面表示中に調整する背景の明暗度---------------------------
	int                 _bgAlpha_whenCall_pauseMenu{ 255 };

	// 画面左下のミニマップ --------------------------------------------
	int                 _miniMap_hdl{};

	// ゲーム開始時に表示する「 Begin 」テキスト-------------------------
	float               _beginTextTimer{};
	bool                _isShowGameBeginText{};
};