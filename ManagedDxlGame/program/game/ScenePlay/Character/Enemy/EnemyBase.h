#pragma once
#include "../../../DxLibEngine.h"

class Player;


class EnemyBase {
public:

	EnemyBase(){}

	float GetDistanceToPlayer();

	// 形状、テクスチャ、ポジション、スケール
	virtual void SetMeshInfo() {}

	virtual bool Update(const float delta_time) { return true; }

	virtual void Render(Shared<dxe::Camera> camera) {}

protected:

	// 待機、追跡、攻撃などのパターンを管理し実行
	virtual void DoRoutineMoves(const float& delta_time) {}

	virtual void ChasePlayer(const float delta_time) {}

	virtual void AttackPlayer(const float& delta_time) {}

	void LookAtPlayer();

	tnl::Vector3 GetRandomPosition_Mt19337() const;

public:

	Shared<Player>            _player_ref = nullptr;

	Shared<dxe::Mesh>         _mesh = nullptr;

	Shared<dxe::Camera>       _mainCamera_ref = nullptr;

public:

	tnl::Vector3 _collide_size{};

	bool _isDead = false;    //敵単体の死亡フラグ

	// CSVからロード-----------------------

	int            _id{};
	float          _charaMoveSpeed{};
	float          _scale{};
	std::string    _name{};
	int _MAX_HP{};
	int      _maxBulletSpawnCount{};
};