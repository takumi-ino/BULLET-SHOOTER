#pragma once
#include "../Bullet.h"

namespace inl {

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
			const inl::PlayerBullet::COLOR color,
			const float size
		);

		~PlayerBullet() override {}

		//�@�e�̈З͐ݒ�-----------------------------------------------------------
		static void ClampBulletPowerRate() noexcept { if (_bulletPowerRate >= 5.0f) _bulletPowerRate = 5.0f; }

		static void ResetBulletPowerRate() noexcept { _bulletPowerRate = 0.f; }

		//�@�`��E�X�V---------------------------------------------------------------
		void Render(const Shared<dxe::Camera> mainCamera) override;
		void Update(const float deltaTime) override;

	public:

		static float _bulletPowerRate;

		bool         _isActive{ true };

		tnl::Vector3 _moveDirection{};            // �i�ޕ���
		tnl::Vector3 _collisionSize{ 10,10,10 };  // �����蔻��T�C�Y

	private:

		float        _speed{};
	};
}