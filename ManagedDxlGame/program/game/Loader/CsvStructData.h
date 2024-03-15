#pragma once


struct EnemyZakoInfo
{
	// �G�{��
	int   _id{};
	float _scale{};
	int   _stageID{};
	int   _hp{};
	int   _maxTotalEnemy_spawnCount{}; // �Q�[���S�̂ő��݂ł�����̂̍ő吔
	float _enemyMoveSpeed{};
	std::string _name{};

	// �e
	int    _maxBulletSpawnCount{};
	int    _bullet_fireInterval{};
	float  _bullet_moveSpeed{};
	float  _bullet_reloadTimeInterval{};
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