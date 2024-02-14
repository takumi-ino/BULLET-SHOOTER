#include "HeightMap.h"


HeightMap::HeightMap(int stage_id) {

	_mesh = dxe::Mesh::CreateFromHeightMapMV("graphics/heightMap/height_map.png", 30000, 30000, 50, 50, 50);

	switch (stage_id)
	{
	case 1:
		_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/heightMap/_heightMap_c.png"));
		break;
	case 2:
		_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/heightMap/_heightMap_b.png"));
		break;
	case 3:
		_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/heightMap/_heightMap_a.png"));
		break;
	default:
		break;
	}

	_mesh->pos_ = { 0,-500,0 };
}


void HeightMap::Update() {

	_mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.01f));
}


void HeightMap::Render(Shared<dxe::Camera> camera) {

	_mesh->render(camera);
}