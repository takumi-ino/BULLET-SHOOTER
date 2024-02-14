#pragma once
#include "../Bullet.h"

class Player;

class PlayerBullet : public Bullet
{
public:

	PlayerBullet() {}
	PlayerBullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction, Shared<Player> player_ref);

	void Update(float delta_time) override;
	void Render(Shared<dxe::Camera> _mainCamera) override;

public:

	Shared<dxe::Mesh> _mesh = nullptr;
	Shared<Player> _player_ref = nullptr;

public:

	tnl::Vector3 _moveDirection{};

	tnl::Vector3 prev_pos;

	tnl::Vector3 _collisionSize{ 10,10,10 };

	bool _isActive;
	float _elapsed{};
	float _speed{};
};