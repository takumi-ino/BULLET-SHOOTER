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
	class Wall;
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
	void InitFirstStageBulletHellLists();   // ステージ１
	void InitSecondStageBulletHellLists();  // ステージ２
	void InitThirdStageBulletHellLists();   // ステージ３

	//　各弾幕のアクティブ状態を false に一括リセット-------------------
	void TurnOffFirstStageBulletHellLists();
	void TurnOffSecondStageBulletHellLists();
	void TurnOffThirdStageBulletHellLists();

	// プレイヤーのボム効果による弾の無効化・有効化--------------------------------------
	static void ReactivateEnemyBullets();
	static void DeactivateAllEnemyBullets();

	//　弾幕を解放------------------------------------------------------
	static void DestroyFirstStageBulletHellLists();
	static void DestroySecondStageBulletHellLists();
	static void DestroyThirdStageBulletHellLists();

	// Getter ---------------------------------------------------------------------------
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

	//　弾幕描画。該当ステージのもののみ実行----------------------------
	void RenderFirstStageBulletHellLists();
	void RenderSecondStageBulletHellLists();
	void RenderThirdStageBulletHellLists();

	//　弾幕更新。該当ステージのもののみ実行----------------------------
	void UpdateFirstStageBulletHellLists();
	void UpdateSecondStageBulletHellLists();
	void UpdateThirdStageBulletHellLists();

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

	static Shared<dxe::Particle>   _weatherParticle;              // ウェザーパーティクル

private:

	Shared<inl::FreeLookCamera>    _mainCamera = nullptr;         // カメラ

	Shared<inl::Player>            _player = nullptr;			  // プレイヤー

	Shared<inl::EnemyManager>      _enemyManager = nullptr;		  // エネミーマネージャー

	Shared<inl::SkyBox>            _skyBox = nullptr;			  // スカイボックス

	Shared<inl::Collision>         _collision = nullptr;		  // コリジョン（当たり判定）

	Shared<inl::Score>             _score = nullptr;			  // スコア

	Shared<inl::BulletHellFactory> _bltHellFactory = nullptr;	  // 弾幕

	Shared<inl::PauseMenu>         _pauseMenu = nullptr;		  // ポーズメニュー

	Shared<dxe::ScreenEffect>      _screenEffect = nullptr;		  // スクリーンエフェクト

	Shared<inl::Wall>              _wall = nullptr;				  // 壁

private:

	// Getterで使用する値-----------------------------------
	static int          _STAGE_ID;                                // ステージID
	static std::string  _GAME_DIFFICULTY;						  // 難易度文字列
	static float        _deltaTime;								  // デルタタイム

	// ポーズ画面表示中に調整する背景の明暗度---------------
	int                 _bgAlpha_whenCall_pauseMenu{ 255 };		  // 背景のアルファ値

	// 画面左下のミニマップ --------------------------------
	int                 _miniMap_hdl{};							  // ミニマップ画像のハンドル

	// ゲーム開始時に表示する「 Begin 」テキスト------------
	float               _beginTextTimer{};						  // タイマー
	bool                _isShowGameBeginText{};					  // 表示管理フラグ
};