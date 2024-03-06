#include "../../Character/Player/Player.h"
#include "PlayerBullet.h"


float PlayerBullet::_bulletPowerRate;


PlayerBullet::PlayerBullet(
	const tnl::Vector3& spawn_pos, 
	const tnl::Vector3& direction,
	const PlayerBullet::COLOR color, 
	const float size) : _speed(1000.0f)
{

	_mesh = dxe::Mesh::CreateSphereMV(size);

	std::map<PlayerBullet::COLOR, std::string> colorMap = {

	   {PlayerBullet::COLOR::Red,    "graphics/colorTexture/red.bmp"},
	   {PlayerBullet::COLOR::White,  "graphics/colorTexture/white.bmp"},
	};

	_mesh->setTexture(dxe::Texture::CreateFromFile(colorMap[color]));
	_mesh->pos_ = spawn_pos;
	_moveDirection = direction; 
	_moveDirection.normalize();
}


void PlayerBullet::Render(Shared<dxe::Camera> _mainCamera) {
	
	_mesh->render(_mainCamera);
}


void PlayerBullet::Update(float delta_time) {

	static tnl::Vector3 start = _mesh->pos_;

	_mesh->pos_ += _moveDirection * delta_time * _speed;

	float dx = _mesh->pos_.x - start.x;
	float dy = _mesh->pos_.y - start.y;
	float dz = _mesh->pos_.z - start.z;

	float current_distance = sqrt(dx * dx + dy * dy + dz * dz);

	if (abs(current_distance) > 1200) _isActive = false;	
}