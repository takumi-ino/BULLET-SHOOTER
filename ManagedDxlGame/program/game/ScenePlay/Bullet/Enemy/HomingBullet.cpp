#include "HomingBullet.h"
#include "../../../DxLibEngine.h"


HomingBullet::HomingBullet() {

	float sphereSize = 10;

	_mesh = dxe::Mesh::CreateSphereMV(sphereSize);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/yellow.jpg"));

	_collisionSize = { sphereSize ,sphereSize ,sphereSize };
}


void HomingBullet::Render(Shared<dxe::Camera> _mainCamera) {
	_mesh->render(_mainCamera);
}


void HomingBullet::CheckLifeTimeDistance(const Shared<HomingBullet>& homingBullet) {

	static tnl::Vector3 start = homingBullet->_mesh->pos_;

	float dx = homingBullet->_mesh->pos_.x - start.x;
	float dy = homingBullet->_mesh->pos_.y - start.y;
	float dz = homingBullet->_mesh->pos_.z - start.z;

	float current_distance = sqrt(dx * dx + dy * dy + dz * dz);

	if (abs(current_distance) > 500) 
		homingBullet->_isActive = false;

}


void HomingBullet::Update(float delta_time) {
}