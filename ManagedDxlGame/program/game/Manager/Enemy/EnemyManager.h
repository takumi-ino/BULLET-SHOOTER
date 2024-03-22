#pragma once
#include "../../Loader/CsvLoader.h"

class ItemManager;

namespace inl {

	class Collision;
	class EnemyZakoBase;
	class EnemyBossBase;

	// 生成・削除・更新を管理
	class EnemyManager
	{
	public:

		EnemyManager() {}
		EnemyManager(
			const Shared<Player>& player,
			const Shared<dxe::Camera>& camera,
			const Shared<Collision>& collision
		);

		virtual ~EnemyManager() {
			_enemyZakoList.clear();
			_enemyBossList.clear();
		}

		// 敵座標取得------------------------------------------------------------
		const std::vector<tnl::Vector3>& GetEnemyZakoPosition();
		const tnl::Vector3& GetEnemyBossPosition();

		// 残りの敵数取得------------------------------------------------------------
		const int GetRemainingEnemyCount() const noexcept { return _zakoEnemyTotalLeftCount; }

		//------------------------------------------------------------
		void Render(const Shared<dxe::Camera>& camera) const;
		void Update(const float deltaTime);

	private:

		// ザコ敵のみの処理----------------------------------------------------------------------------------------
		void InitEnemyZakoInfo();                          // 初期化
		void UpdateEnemyZakoList(const float deltaTime);   // 更新
		void SetMaxEnemySpawnCount() noexcept;             // 1度に生成可能な敵数を設定（難易度ごとに調整）

		// ボスのみの処理------------------------------------------------------------------------------------------
		void InitEnemyBossInfo();                          // 初期化
		void UpdateEnemyBossList(const float deltaTime);   // 更新

		// 生成はするが、ザコが全滅するまでボスは非アクティブ
		void SetSpawnEnemyBoss();

		// ボスが出現したときに一定時間表示するテキスト
		void ShowBossAppearanceText() noexcept;

		// ShowBossAppearanceText 関数の有効時間を計測
		void UpdateBossAppearanceTextTimer(const float deltaTime) noexcept;

		// 更新処理開始
		void SummonBoss();

		// ザコ・ボス共通-------------------------------------------------------------------------------------------------
		void CheckDoSpawnEnemy();           // 一定数のザコ敵を倒すことで、ボスもしくは新たなザコ敵を召喚
		void LoadEnemyDataFromCsv();        // CSVファイルから敵初期化に必要な情報を取得
		bool IsKilledStageBoss(); 	        // ザコリスト及びボスリストのオブジェクトがどちらも０になったら true

		// 当たり判定-----------------------------------------------------------------------------------------------------
		void EnemyZakoCollisionPairLists();
		void EnemyBossCollisionPairLists();

		// アイテム取得イベント通知---------------------------------------------------------------------------------------
		void AttachItemManagerInstance(const Shared<ItemManager>& observer);

		// アイテムスポーンに必要な敵の位置情報を通達
		void NotifyEnemyPosition_ToItemManager();                 

		void SendEnemyPosition(const tnl::Vector3& newPosition, const bool isEnemyActive);

		// 敵殺傷イベント通知---------------------------------------------------------------------------------------------
		void EventNotify_OnEnemyKilled(const std::string enemyName);    // 敵の撃破情報を通達
		void RenderEventHitText() const;								// テキスト描画
		void UpdateEventHitText(const float deltaTime);					// テキスト更新

		// ステージ移動---------------------------------------------------------------------------------------------------
		tnl::Sequence<EnemyManager> _sequence = tnl::Sequence<EnemyManager>(this, &EnemyManager::SeqMoveToNextStage);
		bool SeqMoveToNextStage(const float deltaTime);

		// リザルトへ飛ぶ前に必要な処理をまとめて実行
		bool SeqMoveToResult(const float deltaTime);    

		void MoveToResult(); // 純粋なリザルト移行機能

	public:

		std::vector<Shared<EnemyZakoBase>>     _enemyZakoList{};
		std::vector<Shared<EnemyBossBase>>     _enemyBossList{};

		std::vector<Shared<EnemyZakoBase>>::iterator _itZako{};
		std::vector<Shared<EnemyBossBase>>::iterator _itBoss{};

	private:

		Shared<ItemManager>                    _itemManager = nullptr;
		Shared<CsvLoader>                      _csvLoader = nullptr;

		// 参照				                   
		Shared<Player>                         _player_ref = nullptr;
		Shared<dxe::Camera>                    _mainCamera_ref = nullptr;
		Shared<Collision>                      _collision_ref = nullptr;

		// Zako
		std::unordered_map<int, EnemyZakoInfo> _enemyZakoData_map{};
		EnemyZakoInfo                          _sEnemy_zakoBox_info{};
		EnemyZakoInfo                          _sEnemy_zakoDome_info{};
		EnemyZakoInfo                          _sEnemy_zakoCylinder_info{};

		// Boss
		std::unordered_map<int, EnemyBossInfo> _enemyBossData_map{};
		EnemyBossInfo                          _sBoss_PatchouliKnowledge_info{};
		EnemyBossInfo                          _sBoss_Cirno_info{};
		EnemyBossInfo                          _sBoss_MoriyaSuwako_info{};

	private:

		// オブザーバーパターン。敵の位置からアイテムをスポーン
		std::vector<Shared<ItemManager>>       _observerItems;
		tnl::Vector3                           _enemyZako_position{};
		bool                                   _isEnemyZako_dead{};

	private:

		//　ザコ---------------------------------------------------------------------------------------------------

		// 敵の残り生成可能数（_remainingEnemyZako_spawnCount - 生成した数）
		int                        _remainingEnemyZako_spawnCount{};

		// PlaySceneにGetterで渡す、プレイヤーとまだ戦闘可能な敵の総数（生成済み・未生成両方を含む）
		int                        _zakoEnemyTotalLeftCount{};

		// 1度に生成が可能な最大数
		int                        _maxEnemySpawnCount_PerInterval{};

		std::vector<tnl::Vector3>  _enemyPosList{};

		//　ボス---------------------------------------------------------------------------------------------------
		static float   _bossAppearanceTextTimer;            // ボス出現警告テキストのタイマー
		bool           _isShowBossAppearanceText{};         // ボス出現警告テキストのフラグ

		bool           _isSummonBoss{ false };              // ボス生成フラグ
		bool           _isInitializedBossInfo{ false };	    // ボス初期化フラグ

		bool           _isDefeatedAllStageEnemy{};          // 敵を全滅させたかのフラグ

		tnl::Vector3   _enemyBossPos{};

		//　SE・BGMハンドル	---------------------------------------------------------------------------------------------------
		int            _alertSE_hdl{};
		int            _bossBattleBGM_hdl{};
		int            _battleBGM_hdl{};

		//　スコア---------------------------------------------------------------------------------------------------
		int            _CURRENT_SCORE_REF{};	// PlayerSceneからのスコア参照変数
	};
}