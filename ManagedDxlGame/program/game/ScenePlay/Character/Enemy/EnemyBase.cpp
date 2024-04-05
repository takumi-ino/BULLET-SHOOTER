#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../game/ScenePlay/RandomValue/RandomValueGenerator.h"

namespace inl {

	tnl::Vector3 EnemyBase::GetRandomPosition() const {

		return 
			RandomValueGenerator::Vector(
			-_RANDOM_SPAWN_RANGE_X, _RANDOM_SPAWN_RANGE_X,
			- _RANDOM_SPAWN_RANGE_Y, _RANDOM_SPAWN_RANGE_Y,
			- _RANDOM_SPAWN_RANGE_Z, _RANDOM_SPAWN_RANGE_Z
		);
	}


	float EnemyBase::GetDistanceToPlayer() {

		if (!_mesh) return float();

		tnl::Vector3 dis = _player_ref->GetPos() - _mesh->pos_;
		return std::sqrt(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
	}


	void EnemyBase::LookAtPlayer() {

		if (!_mesh) return;

		tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0,1,0 }, _enemyCamera->axis_y_angle_);
		tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0,0,1 }, q);
		tnl::Vector3 localAxis_y = tnl::Vector3::Cross({ -1,0,0 }, xz);

		_mesh->rot_ = tnl::Quaternion::LookAt(_mesh->pos_, _player_ref->GetPos(), localAxis_y);
	}
}