#include "PlayerBullet.h"
#include "../../Character/Player/Player.h"
#include "../../../../dxlib_ext/dxlib_ext.h"

PlayerBullet::PlayerBullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction, Shared<Player> player_ref)  {

	_player_ref = player_ref;
	_mesh = dxe::Mesh::CreateSphereMV(10);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/red.bmp"));
	_mesh->pos_ = spawn_pos;
	_moveDirection = direction; 
	_speed = 1000.0f;
}


void PlayerBullet::Render(Shared<dxe::Camera> _mainCamera) {

	_mesh->render(_mainCamera);
}


void PlayerBullet::Update(float delta_time) {

	static tnl::Vector3 start{};

	_mesh->pos_ += _moveDirection * delta_time * _speed;

	float dx = _mesh->pos_.x - start.x;
	float dy = _mesh->pos_.y - start.y;
	float dz = _mesh->pos_.z - start.z;

	float current_distance = sqrt(dx * dx + dy * dy + dz * dz);

	// ”ò‹——£‚ª900‚ð’´‚¦‚½‚çÁ‹Ž
	if (abs(current_distance) > 1200) _isActive = false;	
}