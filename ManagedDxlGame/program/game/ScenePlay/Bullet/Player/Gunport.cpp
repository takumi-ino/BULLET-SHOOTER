#include "../game/DxLibEngine.h"
#include "Gunport.h"
#include "../game/ScenePlay/Bullet/Player/PlayerBullet.h"
#include "../game/Utility/CustomException.h"


namespace inl {


	Gunport::Gunport() {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		auto texture = cus->TryLoadTexture("graphics/colorTexture/silver.jpg", "inl::PlayerBullet::PlayerBullet()");

		_mesh = dxe::Mesh::CreateCylinderMV(5, 15);
		_mesh->setTexture(texture);
		_mesh->setMtrlEmissive({ 1,1,1 });
	}


	void Gunport::ManageGunportCount(std::vector<Shared<Gunport>>& gunPort) {

		auto currentBulletPower = PlayerBullet::_bulletPowerRate;

		if (currentBulletPower < 1.0f) return;

		if (currentBulletPower >= 1.0f && currentBulletPower < 2.0f) {

			if (0 == _bulletPowerPhase) {

				gunPort.emplace_back(std::make_shared<Gunport>());
				_bulletPowerPhase++;
			}
		}
		if (currentBulletPower >= 2.0f && currentBulletPower < 3.0f) {

			if (1 == _bulletPowerPhase) {

				gunPort.emplace_back(std::make_shared<Gunport>());
				_bulletPowerPhase++;
			}
		}
		if (currentBulletPower >= 3.0f && currentBulletPower < 4.0f) {

			if (2 == _bulletPowerPhase) {

				gunPort.emplace_back(std::make_shared<Gunport>());
				_bulletPowerPhase++;
			}
		}
		if (currentBulletPower >= 4.0f && currentBulletPower < 5.0f) {

			if (3 == _bulletPowerPhase) {

				gunPort.emplace_back(std::make_shared<Gunport>());
				_bulletPowerPhase++;
			}
		}
		if (currentBulletPower > 4.99f) {

			if (4 == _bulletPowerPhase) {

				gunPort.emplace_back(std::make_shared<Gunport>());
				_bulletPowerPhase++;
			}
			currentBulletPower = 5.0f;
		}
	}


	void Gunport::Render(const Shared<dxe::Camera> playerCamera) {

		_mesh->render(playerCamera);
	}
}