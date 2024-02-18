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



void HomingBullet::Update(float delta_time) {
}