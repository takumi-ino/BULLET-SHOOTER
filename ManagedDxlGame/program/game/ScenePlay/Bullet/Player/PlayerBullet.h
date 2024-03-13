#pragma once
#include "../Bullet.h"


class PlayerBullet : public Bullet
{
public:

	//　Enum-------------------------------------------------------------------
	enum class COLOR {
		None,
		Red,
		White,
	};

	//　コンストラクタ-----------------------------------------------------------
	PlayerBullet() {}

	PlayerBullet(
		const tnl::Vector3& spawnPos,
		const tnl::Vector3& direction,
		const PlayerBullet::COLOR color,
		const float size
	);

	//　描画・更新---------------------------------------------------------------
	void Render(Shared<dxe::Camera> _mainCamera) override;
	void Update(float delta_time) override;

public:

	static float _bulletPowerRate;

	bool         _isActive{ true };

	tnl::Vector3 _moveDirection{};            // 進む方向
	tnl::Vector3 _collisionSize{ 10,10,10 };  // 当たり判定サイズ

private:

	float        _speed{};
};