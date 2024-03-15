#include "StraightBullet.h"

namespace inl {

	StraightBullet::StraightBullet() {

		float sphereSize = 10;

		_mesh = dxe::Mesh::CreateSphereMV(sphereSize);
		_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/purple.jpg"));
		_mesh->setMtrlEmissive({ 1,1,1 });

		_collisionSize = { sphereSize ,sphereSize ,sphereSize };
	}


	void StraightBullet::Render(const Shared<dxe::Camera> mainCamera) {
		_mesh->render(mainCamera);
	}
}