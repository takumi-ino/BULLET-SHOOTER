#pragma once
#include "../Manager/Scene/SceneBase.h"

class ItemManager;

namespace inl {

	class SkyBox;
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
	class SceneNowLoading;
}


// インゲーム地点。あらゆるモジュールをここで使用可能
class ScenePlay : public SceneBase
{
private:

	static ScenePlay* instance;

public:

	ScenePlay() {}
	~ScenePlay() override {}

	ScenePlay(const std::string difficulty, const int stage);

	static ScenePlay* GetInstance() {

		return instance;
	}

	static void CreateInstance() {

		if (instance == nullptr) {

			instance = new ScenePlay();
			instance->Initialize();
		}
	}

	static void Destroy() {

		if (instance != nullptr) {

			delete instance;
			instance = nullptr;
		}
	}

	void Initialize();

	// ステージ移動----------------------------------------------------------------------
	void MoveToNextStage(const int stage, const std::string difficulty);

	// 弾幕初期化（プレイ開始前に該当のステージのボスの弾幕のみ生成）--------------------
	void InitFirstStageBulletHellLists();   // ステージ１
	void InitSecondStageBulletHellLists();  // ステージ２
	void InitThirdStageBulletHellLists();   // ステージ３

	//　各弾幕のアクティブ状態を false に一括リセット-------------------
	static void TurnOffFirstStageBulletHellLists();
	static void TurnOffSecondStageBulletHellLists();
	static void TurnOffThirdStageBulletHellLists();

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

	// ステージID
	static void SetStageID(const int next) noexcept { _STAGE_ID = next; }


	// 選択難易度
	static const std::string GetGameDifficulty() noexcept { return _GAME_DIFFICULTY; }

	// デルタタイム
	static const float GetDeltaTime() noexcept { return _deltaTime; };

	bool GetInitializationComplete()
	{
		return ScenePlay::GetInstance()->_initComplete;
	}

	// Setter ---------------------------------------------------------------------------
	void SetInitializationComplete()
	{
		ScenePlay::GetInstance()->_initComplete = true;
	}

	void SetWeatherParticle(const Shared<dxe::Particle>& particle)
	{
		_weatherParticle = particle;
	}

	void SetMainCamera(const Shared<dxe::Camera>& camera)
	{
		_mainCamera = camera;
	}

	void SetPlayer(const Shared<inl::Player>& player)
	{
		_player = player;
	}

	void SetSkyBox(Shared<inl::SkyBox>& skyBox)
	{
		_skyBox = std::move(skyBox);
	}

	void SetWall(Shared<inl::Wall>& wall)
	{
		_wall = std::move(wall);
	}

	void SetCollision(Shared<inl::Collision>& collision)
	{
		_collision = std::move(collision);
	}

	void SetEnemyManager(const Shared<inl::EnemyManager>& enemyManager)
	{
		_enemyManager = enemyManager;
	}

	void SetBulletHellFactory(Shared<inl::BulletHellFactory>& bulletHellFactory)
	{
		_bltHellFactory = std::move(bulletHellFactory);
	}

	void SetPauseMenu(Shared<inl::PauseMenu>& pauseMenu)
	{
		_pauseMenu = std::move(pauseMenu);
	}

	void SetPauseMenu(Shared<dxe::ScreenEffect>& screenEffect)
	{
		_screenEffect = std::move(screenEffect);
	}


private:

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

	// ミニマップ-------------------------------------------------------
	void RenderEnemyRadarOnMiniMap();
	void RenderStageGrindGround() noexcept;

	// ポーズメニュー---------------------------------------------------
	void RenderPauseMenu();

	// 描画・更新ー---------------------------------------------------
	void Render() override;
	void Update(const float deltaTime) override;

public:

	static Shared<dxe::Particle>   _weatherParticle;  // ウェザーパーティクル

	Shared<inl::SceneNowLoading>   _nowLoading;

	Shared<dxe::Camera>			   _mainCamera;       // カメラ

	Shared<inl::Player>            _player;			  // プレイヤー

	Shared<inl::EnemyManager>      _enemyManager;	  // エネミーマネージャー

	Shared<inl::SkyBox>            _skyBox;			  // スカイボックス

	Shared<inl::Collision>         _collision;		  // コリジョン（当たり判定）

	Shared<inl::Score>			   _score = nullptr;  // スコア

	Shared<inl::BulletHellFactory> _bltHellFactory;	  // 弾幕

	Shared<inl::PauseMenu>         _pauseMenu;		  // ポーズメニュー

	Shared<dxe::ScreenEffect>      _screenEffect;	  // スクリーンエフェクト

	Shared<inl::Wall>              _wall;			  // 壁

public:

	bool				_initComplete{};

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