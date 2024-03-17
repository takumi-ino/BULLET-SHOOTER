#pragma once
#include "../ScenePlay/ScenePlay.h"
#include "CsvStructData.h"


class ScenePlay;

class CsvLoader
{
public:

	std::unordered_map<int, inl::EnemyZakoInfo> 
		LoadEnemyZakoInfos(std::vector<std::vector<tnl::CsvCell>> csv);

	std::unordered_map<int, inl::EnemyBossInfo> 
		LoadEnemyBossInfos(std::vector<std::vector<tnl::CsvCell>> csv);

	std::unordered_map<int, inl::BulletHellType_Info> 
		LoadBulletHellTypeInfos(std::vector<std::vector<tnl::CsvCell>> csv);

	inl::PlayerStatus& 
		LoadPlayerStatus(std::vector<std::vector<tnl::CsvCell>> csv);

private:

	const std::string _SELECTED_DIFFICULTY = ScenePlay::GetGameDifficulty();
};