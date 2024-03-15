#pragma once
#include "../ScenePlay/ScenePlay.h"
#include "CsvStructData.h"


class ScenePlay;

class CsvLoader
{
public:

	CsvLoader() {}

	std::unordered_map<int, inl::EnemyZakoInfo> LoadEnemyZakoInfos(const std::string enemyCsv);
	std::unordered_map<int, inl::EnemyBossInfo> LoadEnemyBossInfos(const std::string enemyCsv);
	std::unordered_map<int, inl::BulletHellType_Info> LoadBulletHellTypeInfos(const std::string bulletHellCsv);
	inl::PlayerStatus& LoadPlayerStatus(const std::string statusCsv);

private:

	const std::string _SELECTED_DIFFICULTY = ScenePlay::GetGameDifficulty();
};