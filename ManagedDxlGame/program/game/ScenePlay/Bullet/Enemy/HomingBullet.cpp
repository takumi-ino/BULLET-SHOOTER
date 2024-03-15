#include "HomingBullet.h"


HomingBullet::HomingBullet() {

	float sphereSize = 10;

	_mesh = dxe::Mesh::CreateSphereMV(sphereSize);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/yellow.jpg"));
	_mesh->setMtrlEmissive({ 1,1,1 });

	_collisionSize = { sphereSize ,sphereSize ,sphereSize };
}


void HomingBullet::Render(const Shared<dxe::Camera> mainCamera) {
	_mesh->render(mainCamera);
}