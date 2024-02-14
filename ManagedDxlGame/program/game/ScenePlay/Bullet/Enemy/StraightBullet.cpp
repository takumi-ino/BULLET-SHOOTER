#include "StraightBullet.h"
#include "EnemyBullet.h"


StraightBullet::StraightBullet() {

	float sphereSize = 10;

	_mesh = dxe::Mesh::CreateSphereMV(sphereSize);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/purple.jpg"));

	_collisionSize = { sphereSize ,sphereSize ,sphereSize };
}



void StraightBullet::CheckLifeTimeDistance(const Shared<StraightBullet>& straightBullet)
{
	static tnl::Vector3 start = straightBullet->_mesh->pos_;

	float dx = straightBullet->_mesh->pos_.x - start.x;
	float dy = straightBullet->_mesh->pos_.y - start.y;
	float dz = straightBullet->_mesh->pos_.z - start.z;

	float current_distance = sqrt(dx * dx + dy * dy + dz * dz);

	if (abs(current_distance) > 500) straightBullet->_isActive = false;
}




 void StraightBullet::Render(Shared<dxe::Camera> _mainCamera) {
	_mesh->render(_mainCamera);
}


void StraightBullet::Update(float delta_time) {

}