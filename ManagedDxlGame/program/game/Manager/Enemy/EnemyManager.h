#pragma once
#include "./../../DxLibEngine.h"
#include "../../../library/tnl_sequence.h"
#include "../../Loader/CsvLoader.h"

class Collision;
class Score;
class EnemyZakoBox;
class EnemyZakoDome;
class EnemyZakoCylinder;
class EnemyZakoBase;
class EnemyBossBase;
class Player;
class ItemManager;
class StraightBullet;
class HomingBullet;
class EnemyBullet;
class EventNoticeText;

// 生成・削除・更新を管理
class EnemyManager
{
public:

	EnemyManager() {}
	EnemyManager(
		const int stageID, 
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision,
		const std::string difficulty
	);

	~EnemyManager() {
		_enemy_zako_list.clear();
		_enemy_boss_list.clear();
	}

	// 敵座標取得
	const std::vector<tnl::Vector3>& GetEnemyZakoPosition();
	const tnl::Vector3& GetEnemyBossPosition();

	// イベント通知
	void AttachItemManagerInstance(const Shared<ItemManager>& observer);
	void NotifyEnemyPosition_ToItemManager();
	void SendEnemyPosition(const tnl::Vector3& new_position, const bool isEnemyActive);

	// 弾情報取得
	const std::list<Shared<StraightBullet>>& GetStraightBullets() { return _straightBullet_zako_ref; };
	void SetStraightBullets(std::list<Shared<StraightBullet>>& bullets) { _straightBullet_zako_ref = bullets; };

	const std::list<Shared<HomingBullet>>& GetHomingBullets() { return _homingBullet_zako_ref; };
	void SetStraightBullets(std::list<Shared<HomingBullet>>& bullets) { _homingBullet_zako_ref = bullets; };

	void EnemyZakoStraightBullet_CollisionPairLists(std::list<Shared<StraightBullet>>& straightBullets_list);
	void EnemyZakoHomingBullet_CollisionPairLists(std::list<Shared<HomingBullet>>& homingBullets_list);

	void Update(const float& deltaTime);
	void Render(const Shared<dxe::Camera>& camera) const;

private:

	// 雑魚
	void InitEnemyZakoInfo();
	void UpdateEnemyZakoList(const float& deltaTime);

	// ボス
	void InitEnemyBossInfo();
	void SetSpawnEnemyBoss();
	void ShowAppearsBossText();
	void UpdateBossAppearanceTextTimer(const float& deltaTime);
	void SummonBoss();
	bool IsKilledStageBoss();
	void UpdateEnemyBossList(const float& deltaTime);

	// 当たり判定
	void EnemyZakoCollisionPairLists();
	void EnemyBossCollisionPairLists();
	void BulletHellCollisionPairLists_DRY(std::vector<Shared<EnemyBullet>>& bullets_vector);
	void BulletHellCollisionPairLists();

	void SetCollisionPairList();

	// イベント通知
	void OnEnemyKilled(const std::string enemy_name);
	void RenderEventHitText() const;
	void UpdateEventHitText(const float& deltaTime);

	// その他
	void CheckDoSpawnEnemy();     // 一定の条件を満たすことでボスもしくは新たなザコ敵を召喚
	void SetMaxEnemySpawnCount(); // 1度に生成可能な敵数（難易度ごとに調整）
	void LoadEnemyDataFromCsv(const std::string& difficulty);

	// ステージ移動
	tnl::Sequence<EnemyManager> _sequence = tnl::Sequence<EnemyManager>(this, &EnemyManager::SeqMoveToNextStage);
	bool SeqMoveToNextStage(const float deltaTime);
	bool SeqMoveToResult(const float deltaTime);

private:

	Shared<ItemManager>   _itemManager = nullptr;
	Shared<CsvLoader>     _csvLoader = nullptr;
	Shared<EnemyZakoBase> _enemyZakoBase = nullptr;
	Shared<EnemyBossBase> _enemyBossBase = nullptr;

	// 参照
	Shared<Player>        _player_ref = nullptr;
	Shared<dxe::Camera>   _mainCamera_ref = nullptr;
	Shared<Collision>     _collision_ref = nullptr;
	Shared<Score>         _score_ref = nullptr;
	Shared<EnemyZakoBox>  _zakoBox_ref = nullptr;
	std::list<Shared<StraightBullet>> _straightBullet_zako_ref{};
	std::list<Shared<HomingBullet>> _homingBullet_zako_ref{};


	// Zako
	std::vector<Shared<EnemyZakoBase>>     _enemy_zako_list{};
	std::unordered_map<int, EnemyZakoInfo> _enemyZakoData_map{};
	EnemyZakoInfo _sEnemy_zakoBox_info{};
	EnemyZakoInfo _sEnemy_zakoDome_info{};
	EnemyZakoInfo _sEnemy_zakoCylinder_info{};

	// Boss
	std::vector<Shared<EnemyBossBase>>     _enemy_boss_list{};
	std::unordered_map<int, EnemyBossInfo> _enemyBossData_map{};
	EnemyBossInfo _sBoss_PatchouliKnowledge_info{};
	EnemyBossInfo _sBoss_Cirno_info{};
	EnemyBossInfo _sBoss_MoriyaSuwako_info{};

private:

	// オブザーバーパターン。敵の位置からアイテムをスポーン
	std::vector<Shared<ItemManager>> _observerItems;
	tnl::Vector3 _enemyZako_position_ref{};
	bool _isEnemyZako_dead_ref{};

private:

	const std::string _SELECTED_LEVEL;

	int _remainingZakoBox_spawnCount{};
	int _remainingZakoDome_spawnCount{};
	int _remainingZakoCylinder_spawnCount{};

	static float _showBossAppearanceText_timer;

	bool _isShowBossAppearanceText{};
	bool _summon_boss = false;
	bool _isInitializedBossInfo = false;
	bool _isDefeatedAllStageEnemy{};

	// PlayerSceneからのスコア参照変数
	int _CURRENT_SCORE_REF{};
	int _stageID{};

	// 1度に生成が可能な最大数
	int _maxEnemySpawnCount_PerInterval{};

	int _alertSE_hdl{};
	int _bossBattleBGM_hdl{};
	int _battleBGM_hdl{};
	
	std::vector<tnl::Vector3> _enemyPosList{};
	tnl::Vector3              _enemyBossPos{};
};