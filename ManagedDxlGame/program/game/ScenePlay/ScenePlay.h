#pragma once
#include "../Manager/Scene/SceneBase.h"

class ItemManager;

namespace inl {

	class SkyBox;
	class FreeLookCamera;
	class PauseMenu;
	class EnemyBullet;
	class BulletHellFactory;
	class StraightBullet;
	class HomingBullet;
	class Collision;
	class EnemyManager;
	class Score;
	class Player;
	class CustomException;
}


// インゲーム地点。あらゆるモジュールをここで使用可能
class ScenePlay : public SceneBase
{
public:
	
	ScenePlay() {}
	ScenePlay(const std::string selectedDifficulty, const int stage);

	~ScenePlay() override {}

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
	// ステージID
	static const int GetStageID() noexcept { return _STAGE_ID; }

	// 選択難易度
	static const std::string GetGameDifficulty() noexcept { return _GAME_DIFFICULTY; }

	// デルタタイム
	static const float GetDeltaTime() noexcept { return _deltaTime; };

private:

	// パーティクル ----------------------------------------------------
	void InitWeatherParticle(const Shared<inl::CustomException>& cus);

	// Setter ----------------------------------------------------------
	void SetDeltaTime(const float deltaTime) { _deltaTime = deltaTime; };

	//　各弾幕のアクティブ状態を false に一括リセット-------------------
	void TurnOff_FirstStageBulletHellLists();
	void TurnOff_SecondStageBulletHellLists();
	void TurnOff_ThirdStageBulletHellLists();

	//　弾幕描画。該当ステージのもののみ実行----------------------------
	void RenderFirstStageBulletHellLists();
	void RenderSecondStageBulletHellLists();
	void RenderThirdStageBulletHellLists();

	//　弾幕更新。該当ステージのもののみ実行----------------------------
	void UpdateFirstStageBulletHellLists();
	void UpdateSecondStageBulletHellLists();
	void UpdateThirdStageBulletHellLists();

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

	static Shared<dxe::Particle>   _weatherParticle;

private:

	Shared<inl::FreeLookCamera>    _mainCamera = nullptr;

	Shared<inl::Player>            _player = nullptr;

	Shared<inl::EnemyManager>      _enemyManager = nullptr;

	Shared<inl::SkyBox>            _skyBox = nullptr;

	Shared<inl::Collision>         _collision = nullptr;

	Shared<inl::Score>             _score = nullptr;

	Shared<inl::BulletHellFactory> _bltHellFactory = nullptr;

	Shared<inl::PauseMenu>         _pauseMenu = nullptr;

	Shared<dxe::ScreenEffect>      _screenEffect = nullptr;

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