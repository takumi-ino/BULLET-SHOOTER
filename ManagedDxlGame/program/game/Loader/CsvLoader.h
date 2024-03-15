#pragma once
#include "../ScenePlay/ScenePlay.h"
#include "CsvStructData.h"


class ScenePlay;

class CsvLoader
{
public:

	CsvLoader() {}

	std::unordered_map<int, EnemyZakoInfo> LoadEnemyZakoInfos(const std::string enemyCsv);
	std::unordered_map<int, EnemyBossInfo> LoadEnemyBossInfos(const std::string enemyCsv);
	std::unordered_map<int, BulletHellType_Info> LoadBulletHellTypeInfos(const std::string bulletHellCsv);
	PlayerStatus& LoadPlayerStatus(const std::string statusCsv);

private:

	const std::string _SELECTED_DIFFICULTY = ScenePlay::GetGameDifficulty();
};