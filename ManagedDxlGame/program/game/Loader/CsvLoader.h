#pragma once
#include "../DxLibEngine.h"


struct EnemyZakoInfo
{
	// ñ{ëÃ
	int   _id{};
	float _scale{};
	int   _stageID{};
	int   _hp{};
	int   _maxTotalEnemy_SpawnCount{}; // ÉQÅ[ÉÄëSëÃÇ≈ë∂ç›Ç≈Ç´ÇÈé¿ëÃÇÃç≈ëÂêî
	float _charaMoveSpeed{};
	std::string _name{};

	// íe
	int    _maxBulletSpawnCount{};
	int    _bullet_FireInterval{};
	float  _bullet_MoveSpeed{};
	float  _bullet_reloadTimeInterval{};
};

struct EnemyBossInfo
{
	int    _id{};
	int    _stageID{};
	float  _scale{};
	int    _hp{};
	int    _maxBulletSpawnCount{};
	float  _charaMoveSpeed{};
	std::string _name{};
};

struct BulletHellType_Info
{
	int   _id{};
	int   _maxBulletSpawnCount{};
	float _bulletSpawnRate{};
	std::string _typeName{};
};

struct PlayerStatus
{
	int _hp{};
	int _at{};
	int _def{};
};


class CsvLoader
{
public:

	CsvLoader() {}
	explicit CsvLoader(const std::string difficulty) : _SELECTED_DIFFICULTY(difficulty) {}

	std::unordered_map<int, EnemyZakoInfo> LoadEnemyZakoInfos(const std::string enemy_csv);
	std::unordered_map<int, EnemyBossInfo> LoadEnemyBossInfos(const std::string enemy_csv);
	std::unordered_map<int, BulletHellType_Info> LoadBulletHellTypeInfos(const std::string bulletHell_csv);
	PlayerStatus& LoadPlayerStatus(const std::string status_csv);

private:

	const std::string _SELECTED_DIFFICULTY;
};