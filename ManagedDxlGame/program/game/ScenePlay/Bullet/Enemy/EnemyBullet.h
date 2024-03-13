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
		// �X�t�B�A(���s)
		Sphere_Straight_Blue,
		Sphere_Straight_EmeraldGreen,
		Sphere_Straight_Yellow,
		// �X�t�B�A(�S����)
		Sphere_Round_Red,
		Sphere_Round_Blue,
		Sphere_Round_EmeraldGreen,
		Sphere_Round_Sky,
		Sphere_Round_Yellow,
		Sphere_Round_White,
		// �X�t�B�A(�����_��)
		Sphere_RandomStraight_Blue,

		// �V�����_�[(���s)
		Cylinder_Straight_Blue,
		Cylinder_Straight_EmeraldGreen,

		// �V�����_�[(�S����)
		Cylinder_Round_Red,
		Cylinder_Round_Blue,
	};

	//�@�R���X�g���N�^-----------------------------------------------------------------------------------
	EnemyBullet(){}
	EnemyBullet(EnemyBullet::SHAPE shape, EnemyBullet::COLOR color , const float size);

	// �`��i�e���p�j-------------------------------------------------------------------------------------
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