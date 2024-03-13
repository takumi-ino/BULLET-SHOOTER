#include "EnemyZakoBox.h"
#include "../../../Bullet/Enemy/StraightBullet.h"
#include "../../../Bullet/Enemy/HomingBullet.h"
#include "../../../Bullet/Enemy/BulletFactory.h"


EnemyZakoBox::EnemyZakoBox(const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera, const Shared<Collision>& collision)
	: EnemyZakoBase(data, player, camera, collision)
{
	_explode_particle = std::make_shared<dxe::Particle>("particle/preset/explosion.bin");

	_collideSize = { 30,30,30 };

	_at = 5;
	_def = 2;
	_randomInvestigateRange_x = 500;
	_randomInvestigateRange_y = 200;
	_randomInvestigateRange_z = 500;

	_bulletTurnDelayRate = 0.5f;

	_straightBullet_count = 0;
	_homingBullet_count = 0;

	_IDLE_DISTANCE = 500.0f;
	_ATTACK_DISTANCE = 450.0f;

	_NOTICE_LIMIT_DURATION = 5.0f;
	_CHANGE_NEXT_BEHAVE_DURATION = 4.0f;
}



void EnemyZakoBox::SetMeshInfo() {

	tnl::Vector3 mesh_size = { 20,20,20 };
	_mesh = dxe::Mesh::CreateBoxMV(mesh_size,
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_left.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_right.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_up.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_down.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_back.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_forward.bmp"));

	_mesh->pos_ = GetRandomPosition_Mt19337();
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}



void EnemyZakoBox::InitBulletFactoryInstance() {

	_bulletFactory = std::make_shared<BulletFactory>(_mesh);

	// íºçsíe
	std::list<Shared<StraightBullet>> bullets_straight =
		_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoBox, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_straight) {

		_straightBullet_queue.push_back(bullet);
	}

	// í«îˆíe
	std::list<Shared<HomingBullet>> bullets_homing =
		_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoBox, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_homing) {
		_homingBullet_queue.push_back(bullet);
	}
}