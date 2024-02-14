#include "EnemyBullet.h"

// Torus: ÉhÅ[ÉiÉcå^
// Plane: î¬
// Disk : â~î’å^

EnemyBullet::EnemyBullet(EnemyBullet::SHAPE shape, EnemyBullet::COLOR color, const float size) {

	switch (shape)
	{
	case EnemyBullet::SHAPE::None:
		break;
	case EnemyBullet::SHAPE::Sphere:
		_mesh = dxe::Mesh::CreateSphereMV(size, 10);
		break;
	case EnemyBullet::SHAPE::Cylinder:
		_mesh = dxe::Mesh::CreateCylinderMV(5, size);
		break;
	}


	std::map<EnemyBullet::COLOR, std::string> colorMap = {

		   {EnemyBullet::COLOR::EmeraldGreen, "graphics/colorTexture/emeraldGreen.jpg"},
		   {EnemyBullet::COLOR::Green,  "graphics/colorTexture/green.bmp"},
		   {EnemyBullet::COLOR::Blue,   "graphics/colorTexture/blue.bmp"},
		   {EnemyBullet::COLOR::Pink,   "graphics/colorTexture/pink.png"},
		   {EnemyBullet::COLOR::Purple, "graphics/colorTexture/purple.jpg"},
		   {EnemyBullet::COLOR::Red,    "graphics/colorTexture/red.bmp"},
		   {EnemyBullet::COLOR::Yellow, "graphics/colorTexture/yellow.jpg"},
		   {EnemyBullet::COLOR::White,  "graphics/colorTexture/white.bmp"},
		   {EnemyBullet::COLOR::Sky,    "graphics/colorTexture/skyColor.bmp"},
	};

	_mesh->setTexture(dxe::Texture::CreateFromFile(colorMap[color]));
}


void EnemyBullet::Render(Shared<dxe::Camera> camera) {

	_mesh->render(camera);
}


void EnemyBullet::Update(float deltaTime) {

}