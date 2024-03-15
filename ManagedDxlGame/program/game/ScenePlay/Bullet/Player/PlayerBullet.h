#pragma once
#include "../Bullet.h"

namespace inl {

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
			const inl::PlayerBullet::COLOR color,
			const float size
		);

		~PlayerBullet() override {}

		//　弾の威力設定-----------------------------------------------------------
		static void ClampBulletPowerRate() noexcept { if (_bulletPowerRate >= 5.0f) _bulletPowerRate = 5.0f; }

		static void ResetBulletPowerRate() noexcept { _bulletPowerRate = 0.f; }

		//　描画・更新---------------------------------------------------------------
		void Render(const Shared<dxe::Camera> mainCamera) override;
		void Update(const float deltaTime) override;

	public:

		static float _bulletPowerRate;

		bool         _isActive{ true };

		tnl::Vector3 _moveDirection{};            // 進む方向
		tnl::Vector3 _collisionSize{ 10,10,10 };  // 当たり判定サイズ

	private:

		float        _speed{};
	};
}