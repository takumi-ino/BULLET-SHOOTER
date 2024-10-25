#pragma once
#include "../game/DxLibEngine.h"
#include "../Bullet.h"

namespace inl {

	class PlayerBullet : public Bullet
	{
	public:

		//�@Enum-------------------------------------------------------------------
		enum class COLOR {
			None,
			Gray,
			White,
		};

		//�@�R���X�g���N�^-----------------------------------------------------------
		PlayerBullet() {}	
		PlayerBullet(Shared<dxe::Mesh> m) {}

		~PlayerBullet() override {};
		// --------------------------------------------------------------------------------

		void SetDirectionAndPosition(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction);

		void ResetDirectionAndPosition();

		//�@�e�̈З͐ݒ�-----------------------------------------------------------
		static void ClampBulletPowerRate() noexcept                        // �e�̈З͐���
		{
			if (_bulletPowerRate >= 5.0f)
				_bulletPowerRate = 5.0f;
		}

		static void ResetBulletPowerRate() noexcept { _bulletPowerRate = 0.f; } // �e�̈З̓��Z�b�g

		void SetBulletSpeed(const float speed) noexcept { _speed = speed; }

		//�@�`��E�X�V---------------------------------------------------------------
		void Render() override;
		void Update(const float deltaTime) override;

	public:

		static float _bulletPowerRate;            // �e�̈З�

		bool         _isActive{ false };          // �A�N�e�B�u���

		tnl::Vector3 _moveDirection{};            // �i�ޕ���
		tnl::Vector3 _collisionSize{ 10,10,10 };  // �����蔻��T�C�Y

	private:

		float        _speed{1000.f};              // �X�s�[�h
	};
}