#pragma once

namespace inl {


	struct EnemyZakoInfo
	{
		// 敵本体
		int   _id{};
		float _scale{};
		int   _stageID{};
		int   _hp{};
		int   _maxTotalEnemy_spawnCount{}; // ゲーム全体で存在できる実体の最大数
		float _enemyMoveSpeed{};
		std::string _name{};

		// 弾
		int    _maxBulletSpawnCount{};
		int    _bulletFireInterval{};
		float  _bulletMoveSpeed{};
		float  _bulletReloadTimeInterval{};
	};

	struct EnemyBossInfo
	{
		int    _id{};
		int    _stageID{};
		float  _scale{};
		int    _hp{};
		int    _maxBulletSpawnCount{};
		float  _enemyMoveSpeed{};
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
}