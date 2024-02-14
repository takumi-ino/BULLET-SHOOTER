#include "StraightBullet.h"
#include "HomingBullet.h"
#include "BulletFactory.h"




std::list<Shared<StraightBullet>> BulletFactory::CreateStraightBullet(const StraightBullet::USER user, const int maxBulletSpawnCount) {

	switch (user) {

	case StraightBullet::USER::ZakoBox:
		return CreateStraightBullet_ZakoBox(maxBulletSpawnCount);
		break;
	case  StraightBullet::USER::ZakoDome:
		return CreateStraightBullet_ZakoDome(maxBulletSpawnCount);
		break;
	case  StraightBullet::USER::ZakoCylinder:
		return CreateStraightBullet_ZakoCylinder(maxBulletSpawnCount);
		break;
	}

	return std::list<Shared<StraightBullet>>();
}



std::list<Shared<StraightBullet>> BulletFactory::CreateStraightBullet_ZakoBox(const int maxBulletSpawnCount) {

	std::list<Shared<StraightBullet>> straightBullet;

	tnl::Vector3 rot_vec = tnl::Vector3::TransformCoord({ 0, 0, 1 }, _enemyMesh_ref->rot_);

	for (int i = 0; i < maxBulletSpawnCount; i++) {

		Shared<StraightBullet> bullet = std::make_shared<StraightBullet>();

		tnl::Vector3 spawn_pos = _enemyMesh_ref->pos_;

		spawn_pos += rot_vec;

		bullet->_mesh->pos_ = spawn_pos;
		bullet->_isActive = true;

		straightBullet.push_back(bullet);
	}

	return straightBullet;
}


std::list<Shared<StraightBullet>> BulletFactory::CreateStraightBullet_ZakoDome(const int maxBulletSpawnCount) {

	std::list<Shared<StraightBullet>> straightBullet;

	for (int i = 0; i < maxBulletSpawnCount; i++) {

		Shared<StraightBullet> bullet = std::make_shared<StraightBullet>();

		tnl::Vector3 spawn_pos = _enemyMesh_ref->pos_;

		spawn_pos.x += _enemyMesh_ref->rot_.x;
		spawn_pos.y += _enemyMesh_ref->rot_.y;
		spawn_pos.z += _enemyMesh_ref->rot_.z;

		bullet->_mesh->pos_ = spawn_pos;
		bullet->_isActive = true;

		straightBullet.push_back(bullet);
	}


	return straightBullet;

}

std::list<Shared<StraightBullet>> BulletFactory::CreateStraightBullet_ZakoCylinder(const int maxBulletSpawnCount) {

	std::list<Shared<StraightBullet>> straightBullet;

	for (int i = 0; i < maxBulletSpawnCount; i++) {

		Shared<StraightBullet> bullet = std::make_shared<StraightBullet>();

		tnl::Vector3 spawn_pos = _enemyMesh_ref->pos_;

		spawn_pos.x += _enemyMesh_ref->rot_.x;
		spawn_pos.y += _enemyMesh_ref->rot_.y;
		spawn_pos.z += _enemyMesh_ref->rot_.z;

		bullet->_mesh->pos_ = spawn_pos;
		bullet->_isActive = true;

		straightBullet.push_back(bullet);
	}


	return straightBullet;

}



std::list<Shared<HomingBullet>> BulletFactory::CreateHomingBullet(
	const HomingBullet::USER user, const int maxBulletSpawnCount) {


	switch (user) {

	case HomingBullet::USER::ZakoBox:
		return CreateHomingBullet_ZakoBox( maxBulletSpawnCount);
		break;
	case  HomingBullet::USER::ZakoDome:
		return CreateHomingBullet_ZakoDome( maxBulletSpawnCount);
		break;
	case  HomingBullet::USER::ZakoCylinder:
		return CreateHomingBullet_ZakoCylinder( maxBulletSpawnCount);
		break;
	}

	return std::list<Shared<HomingBullet>>();

}



std::list<Shared<HomingBullet>> BulletFactory::CreateHomingBullet_ZakoBox(const int maxBulletSpawnCount) {

	std::list<Shared<HomingBullet>> homingBullet;

	for (int i = 0; i < maxBulletSpawnCount; i++) {

		Shared<HomingBullet> bullet = std::make_shared<HomingBullet>();


		tnl::Vector3 spawn_pos = _enemyMesh_ref->pos_;

		spawn_pos.x += _enemyMesh_ref->rot_.x;
		spawn_pos.y += _enemyMesh_ref->rot_.y;
		spawn_pos.z += _enemyMesh_ref->rot_.z;
		
		bullet->_mesh->pos_ = spawn_pos;
		bullet->_isActive = true;

		homingBullet.push_back(bullet);
	}

	return homingBullet;

}


std::list<Shared<HomingBullet>> BulletFactory::CreateHomingBullet_ZakoDome(const int maxBulletSpawnCount) {
	std::list<Shared<HomingBullet>> homingBullet;

	return homingBullet;

}


std::list<Shared<HomingBullet>> BulletFactory::CreateHomingBullet_ZakoCylinder(const int maxBulletSpawnCount) {
	std::list<Shared<HomingBullet>> homingBullet;

	return homingBullet;



}
