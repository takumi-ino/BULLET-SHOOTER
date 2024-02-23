#include "../game/ScenePlay/Character/Player/Player.h"
#include "../game/ScenePlay/Bullet/Player/PlayerBullet.h"
#include "Gunport.h"


Gunport::Gunport() {

	_mesh = dxe::Mesh::CreateCylinderMV(5,15);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/colorTexture/silver.jpg"));
	_mesh->setMtrlEmissive({ 1,1,1 });
}



void Gunport::ManageGunportCount(std::vector<Shared<Gunport>>& gunPort) {

	if (PlayerBullet::_bulletPowerRate < 1.0f) return;

	if (PlayerBullet::_bulletPowerRate >= 1.0f && PlayerBullet::_bulletPowerRate < 2.0f) {

		if (0 == _bulletPowerPhase) {

			gunPort.emplace_back(std::make_shared<Gunport>());
			_bulletPowerPhase++;
		}
	}
	if (PlayerBullet::_bulletPowerRate >= 2.0f && PlayerBullet::_bulletPowerRate < 3.0f) {

		if (1 == _bulletPowerPhase) {

			gunPort.emplace_back(std::make_shared<Gunport>());
			_bulletPowerPhase++;
		}
	}
	if (PlayerBullet::_bulletPowerRate >= 3.0f && PlayerBullet::_bulletPowerRate < 4.0f) {

		if (2 == _bulletPowerPhase) {

			gunPort.emplace_back(std::make_shared<Gunport>());
			_bulletPowerPhase++;
		}
	}
	if (PlayerBullet::_bulletPowerRate >= 4.0f && PlayerBullet::_bulletPowerRate < 5.0f) {

		if (3 == _bulletPowerPhase) {

			gunPort.emplace_back(std::make_shared<Gunport>());
			_bulletPowerPhase++;
		}
	}
	if (PlayerBullet::_bulletPowerRate > 4.98f) {

		if (4 == _bulletPowerPhase) {

			gunPort.emplace_back(std::make_shared<Gunport>());
			_bulletPowerPhase++;
		}
		PlayerBullet::_bulletPowerRate = 5.0f;
	}
}



void Gunport::Render(const Shared<dxe::Camera> playerCamera) {

	_mesh->render(playerCamera);
}