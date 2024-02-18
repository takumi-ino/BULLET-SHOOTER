#pragma once
#include "../Bullet.h"
#include "../../Character/Enemy/EnemyZakoBase.h"
#include "../../Character/Player/Player.h"


class Player;

class EnemyBullet : public Bullet
{
public:

	enum class SHAPE {

		None,
		Sphere,
		Cylinder,
	};

	enum class COLOR {

		None,
		EmeraldGreen,
		Green,
		Blue,
		Pink,
		Purple,
		Red,
		Yellow,
		White,
		Sky
	};

	enum class SPECIFICTYPE {
		None,
		// スフィア(直行)
		Sphere_Straight_Blue,
		Sphere_Straight_EmeraldGreen,
		Sphere_Straight_Yellow,
		// スフィア(全方位)
		Sphere_Round_Red,
		Sphere_Round_Blue,
		Sphere_Round_EmeraldGreen,
		Sphere_Round_Sky,
		Sphere_Round_Yellow,
		Sphere_Round_White,
		// スフィア(ランダム)
		Sphere_RandomStraight_Blue,

		// シリンダー(直行)
		Cylinder_Straight_Blue,
		Cylinder_Straight_EmeraldGreen,

		// シリンダー(全方位)
		Cylinder_Round_Red,
		Cylinder_Round_Blue,

	};


	SHAPE shape = EnemyBullet::SHAPE::None;
	SPECIFICTYPE specificType = EnemyBullet::SPECIFICTYPE::None;


	EnemyBullet(){}
	explicit EnemyBullet(EnemyBullet::SHAPE shape, EnemyBullet::COLOR color , const float size);
	explicit EnemyBullet(int){}
	explicit EnemyBullet(Shared<EnemyBullet> enemy_bullet) {}
	EnemyBullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction) {}
	EnemyBullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction, Shared<Player> player_ref) {}

	// 弾幕専用
	void Update(const float deltaTime) override;
	// 弾幕専用
	void Render(Shared<dxe::Camera> camera) override;

public:

	Shared<Player> _player_ref;

protected:

	Shared<EnemyZakoBase> _enemy_ref;

	tnl::Vector3 prev_pos;

public:

	int _id{};
	float _speed{};
	float _radius{};
	float _angle{};

	bool _isActive = false;

	tnl::Vector3 _moveDirection{};
	tnl::Vector3 _startPosition{};
	tnl::Vector3 _collisionSize{};
};