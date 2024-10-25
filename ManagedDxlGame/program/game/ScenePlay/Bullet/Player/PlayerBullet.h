#pragma once
#include "../game/DxLibEngine.h"
#include "../Bullet.h"

namespace inl {

	class PlayerBullet : public Bullet
	{
	public:

		//　Enum-------------------------------------------------------------------
		enum class COLOR {
			None,
			Gray,
			White,
		};

		//　コンストラクタ-----------------------------------------------------------
		PlayerBullet() {}	
		PlayerBullet(Shared<dxe::Mesh> m) {}

		~PlayerBullet() override {};
		// --------------------------------------------------------------------------------

		void SetDirectionAndPosition(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction);

		void ResetDirectionAndPosition();

		//　弾の威力設定-----------------------------------------------------------
		static void ClampBulletPowerRate() noexcept                        // 弾の威力制限
		{
			if (_bulletPowerRate >= 5.0f)
				_bulletPowerRate = 5.0f;
		}

		static void ResetBulletPowerRate() noexcept { _bulletPowerRate = 0.f; } // 弾の威力リセット

		void SetBulletSpeed(const float speed) noexcept { _speed = speed; }

		//　描画・更新---------------------------------------------------------------
		void Render() override;
		void Update(const float deltaTime) override;

	public:

		static float _bulletPowerRate;            // 弾の威力

		bool         _isActive{ false };          // アクティブ状態

		tnl::Vector3 _moveDirection{};            // 進む方向
		tnl::Vector3 _collisionSize{ 10,10,10 };  // 当たり判定サイズ

	private:

		float        _speed{1000.f};              // スピード
	};
}