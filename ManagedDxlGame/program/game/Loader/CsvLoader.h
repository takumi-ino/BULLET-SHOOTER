#pragma once
#include "../DxLibEngine.h"

class Player;
class EnemyBoss_PatchouliKnowledge;


struct EnemyZakoInfo
{
	// �{��
	int _id{};
	int _scale{};
	int _stageID{};
	int _hp{};
	int _maxTotalEnemySpawnCount{};	// �Q�[���S�̂ő��݂ł�����̂̍ő吔
	float _charaMoveSpeed{};
	std::string _name{};

	// �e
	int _maxBulletSpawnCount{};
	int _bulletFireInterval{};
	int _bulletMoveSpeed{};
	float _reloadTimeInterval{};

};

struct EnemyBossInfo
{
	int _id{};
	int _stageID{};
	int _scale{};
	int _hp;
	int _maxBulletSpawnCount{};
	float _charaMoveSpeed{};
	std::string _name{};
};

struct BulletHellType_Info
{
	int _id{};
	int _maxBulletSpawnCount{};
	float _bulletSpawnRate{};
	std::string _typeName{};
};


class CsvLoader
{
public:

	CsvLoader() {}
	explicit CsvLoader(const std::string difficulty) : _SELECTED_DIFFICULTY(difficulty) {}

	std::unordered_map<int, EnemyZakoInfo> LoadEnemyZakoInfos(const std::string enemy_csv);
	std::unordered_map<int, EnemyBossInfo> LoadEnemyBossInfos(const std::string enemy_csv);
	std::unordered_map<int, BulletHellType_Info> LoadBulletHellTypeInfos(const std::string enemy_csv);

private:

	const std::string _SELECTED_DIFFICULTY;
};
