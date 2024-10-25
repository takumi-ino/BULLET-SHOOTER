#include "EnemyZakoCylinder.h"
#include "../../../Bullet/Enemy/StraightBullet.h"
#include "../../../Bullet/Enemy/HomingBullet.h"
#include "../../../Bullet/Enemy/BulletFactory.h"


namespace inl {

	EnemyZakoCylinder::EnemyZakoCylinder(const EnemyZakoInfo& data) : EnemyZakoBase(data)
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


	void EnemyZakoCylinder::SetMeshInfo() {

		_mesh = dxe::Mesh::CreateCylinderMV(20, 20);
		_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/box.bmp"));
		_mesh->pos_ = GetRandomPosition();
		_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
	}



	void EnemyZakoCylinder::InitBulletFactoryInstance() {

		_bulletFactory = std::make_shared<BulletFactory>(_mesh);

		// ���s�e
		std::list<Shared<StraightBullet>> bullets_straight =
			_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoCylinder, _maxBulletSpawnCount);
		for (const auto& bullet : bullets_straight) {

			_straightBullet_queue.push_back(bullet);
		}


		// �ǔ��e
		std::list<Shared<HomingBullet>> bullets_homing =
			_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoCylinder, _maxBulletSpawnCount);
		for (const auto& bullet : bullets_homing) {
			_homingBullet_queue.push_back(bullet);
		}
	}
}