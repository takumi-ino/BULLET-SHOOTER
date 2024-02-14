#include "CsvLoader.h"
#include "../ScenePlay/Character/Player/Player.h"
#include "../ScenePlay/Bullet/Enemy/BulletHell.h"


std::unordered_map<int, EnemyZakoInfo> CsvLoader::LoadEnemyZakoInfos(const std::string enemy_csv) {

	std::unordered_map<int, EnemyZakoInfo> infos;

	auto csv = tnl::LoadCsv(enemy_csv);

	for (int i = 1; i < csv.size(); i++) {

		EnemyZakoInfo data;

		data._id = csv[i][0].getInt();
		data._stageID = csv[i][1].getInt();
		data._scale = csv[i][2].getInt();
		data._name = csv[i][3].getString();



		if (_SELECTED_DIFFICULTY == "Easy") {
			data._hp = csv[i][4].getInt();
			data._maxBulletSpawnCount = csv[i][8].getInt();
			data._maxTotalEnemySpawnCount = csv[i][12].getInt();
			data._charaMoveSpeed = csv[i][16].getFloat();
			data._reloadTimeInterval = csv[i][20].getFloat();
			data._bulletFireInterval = csv[i][24].getInt();
			data._bulletMoveSpeed = csv[i][28].getInt();
		}
		else if (_SELECTED_DIFFICULTY == "Normal") {
			data._hp = csv[i][5].getInt();
			data._maxBulletSpawnCount = csv[i][9].getInt();
			data._maxTotalEnemySpawnCount = csv[i][13].getInt();
			data._charaMoveSpeed = csv[i][17].getFloat();
			data._reloadTimeInterval = csv[i][21].getFloat();
			data._bulletFireInterval = csv[i][25].getInt();
			data._bulletMoveSpeed = csv[i][29].getInt();

		}
		else if (_SELECTED_DIFFICULTY == "Hard") {
			data._hp = csv[i][6].getInt();
			data._maxBulletSpawnCount = csv[i][10].getInt();
			data._maxTotalEnemySpawnCount = csv[i][14].getInt();
			data._charaMoveSpeed = csv[i][18].getFloat();
			data._reloadTimeInterval = csv[i][22].getFloat();
			data._bulletFireInterval = csv[i][26].getInt();
			data._bulletMoveSpeed = csv[i][30].getInt();

		}
		else if (_SELECTED_DIFFICULTY == "Lunatic") {
			data._hp = csv[i][7].getInt();
			data._maxBulletSpawnCount = csv[i][11].getInt();
			data._maxTotalEnemySpawnCount = csv[i][15].getInt();
			data._charaMoveSpeed = csv[i][19].getFloat();
			data._reloadTimeInterval = csv[i][23].getFloat();
			data._bulletFireInterval = csv[i][27].getInt();
			data._bulletMoveSpeed = csv[i][31].getInt();

		}


		infos[data._id] = data;
	}

	return infos;
}



std::unordered_map<int, EnemyBossInfo> CsvLoader::LoadEnemyBossInfos(const std::string enemy_csv) {

	std::unordered_map<int, EnemyBossInfo> infos;

	auto csv = tnl::LoadCsv(enemy_csv);

	for (int i = 1; i < csv.size(); i++) {

		EnemyBossInfo data;

		data._id = csv[i][0].getInt();
		data._stageID = csv[i][1].getInt();
		data._scale = csv[i][2].getInt();
		data._name = csv[i][3].getString();


		if (_SELECTED_DIFFICULTY == "Easy") {
			data._hp = csv[i][4].getInt();
			data._maxBulletSpawnCount = csv[i][8].getInt();
			data._charaMoveSpeed = csv[i][12].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Normal") {
			data._hp = csv[i][5].getInt();
			data._maxBulletSpawnCount = csv[i][9].getInt();
			data._charaMoveSpeed = csv[i][13].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Hard") {
			data._hp = csv[i][6].getInt();
			data._maxBulletSpawnCount = csv[i][10].getInt();
			data._charaMoveSpeed = csv[i][14].getFloat();
		}
		else if (_SELECTED_DIFFICULTY == "Lunatic") {
			data._hp = csv[i][7].getInt();
			data._maxBulletSpawnCount = csv[i][11].getInt();
			data._charaMoveSpeed = csv[i][15].getFloat();
		}


		infos[data._id] = data;
	}

	return infos;
}


std::unordered_map<int, BulletHellType_Info> CsvLoader::LoadBulletHellTypeInfos(const std::string enemy_csv) {

	std::unordered_map<int, BulletHellType_Info> info;

	auto csv = tnl::LoadCsv(enemy_csv);


	for (int i = 1; i < csv.size(); i++) {

		BulletHellType_Info data;

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
