#pragma once
#include "../Bullet.h"

class Player;

class PlayerBullet : public Bullet
{
public:

	enum class COLOR {
		None,
		Red,
		White,
	};

	COLOR color = PlayerBullet::COLOR::None;


	PlayerBullet() {} 
	PlayerBullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction, const PlayerBullet::COLOR color, const float size);

	void Update(float delta_time) override;
	void Render(Shared<dxe::Camera> _mainCamera) override;

public:

	tnl::Vector3 _spawnPosition{};

	tnl::Vector3 _moveDirection{};

	tnl::Vector3 _collisionSize{ 10,10,10 };

	bool  _isActive = true;
	float _elapsed{};
	float _speed{};

	static float _bulletPowerRate;
};