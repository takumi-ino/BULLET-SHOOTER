#include "PlayerBullet.h"
#include "../game/Utility/CustomException.h"


namespace inl {

	float PlayerBullet::_bulletPowerRate;


	PlayerBullet::PlayerBullet(
		const tnl::Vector3& spawn_pos,
		const tnl::Vector3& direction,
		const PlayerBullet::COLOR color,
		const float size) : _speed(1000.0f)
	{

		Shared<CustomException> cus = std::make_shared<CustomException>();

		//　ロードに失敗したら例外発生----------------------------------------------------
		auto grayGraphHandle =
			cus->TryLoadTexture("graphics/colorTexture/gray.bmp", "inl::PlayerBullet::PlayerBullet()");

		auto whiteGraphHandle = 
			cus->TryLoadTexture("graphics/colorTexture/white.bmp", "inl::PlayerBullet::PlayerBullet()");

		_mesh = dxe::Mesh::CreateSphereMV(size);

		std::map<PlayerBullet::COLOR, Shared<dxe::Texture>> colorMap = {

		   {PlayerBullet::COLOR::Gray,    grayGraphHandle},
		   {PlayerBullet::COLOR::White,  whiteGraphHandle},
		};

		_mesh->setTexture(colorMap[color]);
		_mesh->pos_ = spawn_pos;
		_moveDirection = direction;
		_moveDirection.normalize();
	}


	void PlayerBullet::Render(const Shared<dxe::Camera> mainCamera) {

		_mesh->render(mainCamera);
	}


	void PlayerBullet::Update(const float deltaTime) {

		static tnl::Vector3 start = _mesh->pos_;

		_mesh->pos_ += _moveDirection * deltaTime * _speed;

		tnl::Vector3 d;
		d.x = _mesh->pos_.x - start.x;
		d.y = _mesh->pos_.y - start.y;
		d.z = _mesh->pos_.z - start.z;
				
		float dist = sqrt(pow(d.x - 0, 2) + pow(d.y - 0, 2) + pow(d.z - 0, 2));

		if (dist > 1300.f) _isActive = false;
	}
}