#include "StraightBullet.h"
#include "EnemyBullet.h"


StraightBullet::StraightBullet() {

	float sphereSize = 10;

	_mesh = dxe::Mesh::CreateSphereMV(sphereSize);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/purple.jpg"));

	_collisionSize = { sphereSize ,sphereSize ,sphereSize };
}


 void StraightBullet::Render(Shared<dxe::Camera> _mainCamera) {
	_mesh->render(_mainCamera);
}


void StraightBullet::Update(float delta_time) {

}