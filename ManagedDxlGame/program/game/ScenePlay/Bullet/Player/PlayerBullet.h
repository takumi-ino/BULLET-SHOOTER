#pragma once
#include "../Bullet.h"


class PlayerBullet : public Bullet
{
public:

	//�@Enum-------------------------------------------------------------------
	enum class COLOR {
		None,
		Red,
		White,
	};

	//�@�R���X�g���N�^-----------------------------------------------------------
	PlayerBullet() {}

	PlayerBullet(
		const tnl::Vector3& spawnPos,
		const tnl::Vector3& direction,
		const PlayerBullet::COLOR color,
		const float size
	);

	//�@�`��E�X�V---------------------------------------------------------------
	void Render(Shared<dxe::Camera> _mainCamera) override;
	void Update(float delta_time) override;

public:

	static float _bulletPowerRate;

	bool         _isActive{ true };

	tnl::Vector3 _moveDirection{};            // �i�ޕ���
	tnl::Vector3 _collisionSize{ 10,10,10 };  // �����蔻��T�C�Y

private:

	float        _speed{};
};