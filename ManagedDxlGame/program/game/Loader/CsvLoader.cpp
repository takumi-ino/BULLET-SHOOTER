#include "../DxLibEngine.h"
#include "CsvLoader.h"


std::unordered_map<int, inl::EnemyZakoInfo> CsvLoader::LoadEnemyZakoInfos(std::vector<std::vector<tnl::CsvCell>> csv) {

	std::unordered_map<int, inl::EnemyZakoInfo> infos;

	for (int i = 1; i < csv.size(); i++) {

		inl::EnemyZakoInfo data;

		data._id = csv[i][0].getInt();
		data._stageID = csv[i][1].getInt();
		data._scale = csv[i][2].getFloat();
		data._name = csv[i][3].getString();

		if (_SELECTED_DIFFICULTY == "Easy") {
			data._hp = csv[i][4].getInt();
			data._maxBulletSpawnCount = csv[i][8].getInt();
			data._maxTotalEnemy_spawnCount = csv[i][12].getInt();
			data._enemyMoveSpeed = csv[i][16].getFloat();
			data._bulletReloadTimeInterval = csv[i][20].getFloat();
			data._bulletFireInterval = csv[i][24].getInt();
			data._bulletMoveSpeed = csv[i][28].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Normal") {
			data._hp = csv[i][5].getInt();
			data._maxBulletSpawnCount = csv[i][9].getInt();
			data._maxTotalEnemy_spawnCount = csv[i][13].getInt();
			data._enemyMoveSpeed = csv[i][17].getFloat();
			data._bulletReloadTimeInterval = csv[i][21].getFloat();
			data._bulletFireInterval = csv[i][25].getInt();
			data._bulletMoveSpeed = csv[i][29].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Hard") {
			data._hp = csv[i][6].getInt();
			data._maxBulletSpawnCount = csv[i][10].getInt();
			data._maxTotalEnemy_spawnCount = csv[i][14].getInt();
			data._enemyMoveSpeed = csv[i][18].getFloat();
			data._bulletReloadTimeInterval = csv[i][22].getFloat();
			data._bulletFireInterval = csv[i][26].getInt();
			data._bulletMoveSpeed = csv[i][30].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Lunatic") {
			data._hp = csv[i][7].getInt();
			data._maxBulletSpawnCount = csv[i][11].getInt();
			data._maxTotalEnemy_spawnCount = csv[i][15].getInt();
			data._enemyMoveSpeed = csv[i][19].getFloat();
			data._bulletReloadTimeInterval = csv[i][23].getFloat();
			data._bulletFireInterval = csv[i][27].getInt();
			data._bulletMoveSpeed = csv[i][31].getFloat();
		}

		infos[data._id] = data;
	}

	return infos;
}



std::unordered_map<int, inl::EnemyBossInfo> CsvLoader::LoadEnemyBossInfos(std::vector<std::vector<tnl::CsvCell>> csv) {

	std::unordered_map<int, inl::EnemyBossInfo> infos;

	for (int i = 1; i < csv.size(); i++) {

		inl::EnemyBossInfo data;

		data._id = csv[i][0].getInt();
		data._stageID = csv[i][1].getInt();
		data._scale = csv[i][2].getFloat();
		data._name = csv[i][3].getString();

		if (_SELECTED_DIFFICULTY == "Easy") {
			data._hp = csv[i][4].getInt();
			data._maxBulletSpawnCount = csv[i][8].getInt();
			data._enemyMoveSpeed = csv[i][12].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Normal") {
			data._hp = csv[i][5].getInt();
			data._maxBulletSpawnCount = csv[i][9].getInt();
			data._enemyMoveSpeed = csv[i][13].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Hard") {
			data._hp = csv[i][6].getInt();
			data._maxBulletSpawnCount = csv[i][10].getInt();
			data._enemyMoveSpeed = csv[i][14].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Lunatic") {
			data._hp = csv[i][7].getInt();
			data._maxBulletSpawnCount = csv[i][11].getInt();
			data._enemyMoveSpeed = csv[i][15].getFloat();
		}

		infos[data._id] = data;
	}

	return infos;
}



std::unordered_map<int, inl::BulletHellType_Info> CsvLoader::LoadBulletHellTypeInfos(std::vector<std::vector<tnl::CsvCell>> csv) {

	std::unordered_map<int, inl::BulletHellType_Info> info;


	for (int i = 1; i < csv.size(); i++) {

		inl::BulletHellType_Info data;

		data._id = csv[i][0].getInt();
		data._typeName = csv[i][1].getString();

		if (_SELECTED_DIFFICULTY == "Easy") {
			data._maxBulletSpawnCount = csv[i][2].getInt();
			data._bulletSpawnRate = csv[i][6].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Normal") {
			data._maxBulletSpawnCount = csv[i][3].getInt();
			data._bulletSpawnRate = csv[i][7].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Hard") {
			data._maxBulletSpawnCount = csv[i][4].getInt();
			data._bulletSpawnRate = csv[i][8].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Lunatic") {
			data._maxBulletSpawnCount = csv[i][5].getInt();
			data._bulletSpawnRate = csv[i][9].getFloat();
		}

		info[data._id] = data;
	}

	return info;
}



inl::PlayerStatus& CsvLoader::LoadPlayerStatus(std::vector<std::vector<tnl::CsvCell>> csv) {

	inl::PlayerStatus data;

	if (_SELECTED_DIFFICULTY == "Easy") {
		data._hp = csv[1][0].getInt();
		data._at = csv[1][4].getInt();
		data._def = csv[1][8].getInt();
	}
	else if (_SELECTED_DIFFICULTY == "Normal") {
		data._hp = csv[1][1].getInt();
		data._at = csv[1][5].getInt();
		data._def = csv[1][9].getInt();
	}
	else if (_SELECTED_DIFFICULTY == "Hard") {
		data._hp = csv[1][2].getInt();
		data._at = csv[1][6].getInt();
		data._def = csv[1][10].getInt();
	}
	else {
		data._hp = csv[1][3].getInt();
		data._at = csv[1][7].getInt();
		data._def = csv[1][11].getInt();
	}

	return data;
}