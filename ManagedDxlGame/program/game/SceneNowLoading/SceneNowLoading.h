#pragma once
#include <queue>
#include <functional>
#include <future>
#include "../ScenePlay/ScenePlay.h"
#include "../game/Manager/Enemy/EnemyManager.h"

class SceneBase;

namespace inl {

	class Player;
	class PlayerBullet;
	class SkyBox;
	class Wall;
	class Collision;
	class EnemyManager;
	class BulletHellFactory;
	class PauseMenu;


	// SceneNowLoadingでロードしてScenePlayに渡す

	class SceneNowLoading : public SceneBase
	{
	public:

		SceneNowLoading(){}
		SceneNowLoading(const std::string selectedDifficulty, const int stage);

		void Update(const float deltaTime) override;
		void Render() override;

	private:

		// ロード（オブジェクト生成）-----------------------------
		void LoadWeatherParticle();			    //　ウェザーパーティクル
		void LoadMainCamera();				    //　カメラ
		void LoadPlayer();					    //　プレイヤー
		void LoadSkyBox();					    //　スカイボックス
		void LoadWall();					    //　壁
		void LoadCollision();				    //　当たり判定
		void LoadEnemies();					    //　敵
		void LoadBulletHellFactory();		    //　弾工場
		void LoadItems();					    //　アイテム
		void LoadScreenEffect();			    //　スクリーンエフェクト
		void LoadPauseMenu();				    //　ポーズメニュー

		// 初期化--------------------------------------------------
		void InitBomb() noexcept;
		void InitPlayersBombCount() noexcept;
		void InitPlayerMesh() noexcept;
		void InitObjectsInOrder();
		void InitPlayerStraightBullet();
		void PushInitTasksToQueue();

		// その他-------------------------------------------------
		void AssignLoadedObjects();
		void ExecuteNextTask();

	private:

		/*  生成したオブジェクトを順番に queue（先入先出法） に格納
			先頭から順番にオブジェクト生成が始まる  */

		std::queue<std::function<void()>> _initObjects{};

		/*  現在のオブジェクト。生成が完了したら次のオブジェクトを代入し、生成を開始
　		  （現在のオブジェクトの生成が完了するまで次のオブジェクト生成は待機状態）　*/

		std::future<void>				  _currentTaskObject;

		// ScenePlay で使用する各オブジェクト

		Shared<Player>			  _loadedPlayer = nullptr;
		Shared<dxe::Mesh>		  _loadedPlayerBulletMesh = nullptr;
		Shared<dxe::Camera>		  _loadedCamera = nullptr;
		Shared<SkyBox>			  _loadedSkyBox = nullptr;
		Shared<Wall>			  _loadedWall = nullptr;
		Shared<Collision>		  _loadedCollision = nullptr;
		Shared< EnemyManager>	  _loadedEnemyManager = nullptr;
		Shared<BulletHellFactory> _loadedBulletHellFactory = nullptr;
		Shared<ItemManager>		  _loadedItemManager = nullptr;
		Shared<dxe::ScreenEffect> _loadedScreenEffect = nullptr;
		Shared<PauseMenu>		  _loadedPauseMenu = nullptr;

		// ScenePlayの一部関数を呼び出すためにオブジェクトを保持
		ScenePlay*				  _scenePlay = nullptr;

	private:

		bool _initializationComplete = false;

		std::string _selectedDifficulty{};

		int _stage{};

		int _gh{};
	};
}