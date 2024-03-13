#pragma once
#include "../Bullet.h"
#include "../../Character/Player/Player.h"


class EnemyBullet : public Bullet
{
public:

	// Enum----------------------------------------------------------------------------------------------
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

	//　コンストラクタ-----------------------------------------------------------------------------------
	EnemyBullet(){}
	EnemyBullet(EnemyBullet::SHAPE shape, EnemyBullet::COLOR color , const float size);

	// 描画（弾幕用）-------------------------------------------------------------------------------------
	void Render(Shared<dxe::Camera> camera) override;

protected:

	Shared<Player>  _player_ref{};

public:

	SPECIFICTYPE    specificType = EnemyBullet::SPECIFICTYPE::None;

	int             _id{};
	float           _speed{};
	float           _radius{};
	float           _angle{};
				   
	bool            _isActive{ false };
				   
	tnl::Vector3    _moveDirection{};
	tnl::Vector3    _collisionSize{};
};