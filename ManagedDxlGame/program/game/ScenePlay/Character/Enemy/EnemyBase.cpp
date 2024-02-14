#include <random>
#include "../Player/Player.h"
#include "EnemyBase.h"



tnl::Vector3 EnemyBase::GetRandomPosition_Mt19337() const {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> distX(-800.0f, 800.0f);
	std::uniform_real_distribution<float> distY(-100.0f, 100.0f);
	std::uniform_real_distribution<float> distZ(-500.0f, 500.0f);

	return tnl::Vector3(distX(gen), distY(gen), distZ(gen));
}


float EnemyBase::GetDistanceToPlayer() {

	if (!_mesh) return float();

	tnl::Vector3 dis = _player_ref->GetPos() - _mesh->pos_;
	return std::sqrt(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
}


void EnemyBase::LookAtPlayer() {

	if (!_mesh) return;

	tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0,1,0 }, _mainCamera_ref->axis_y_angle_);
	tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0,0,1 }, q);
	tnl::Vector3 local_axis_y = tnl::Vector3::Cross({ -1,0,0 }, xz);

	_mesh->rot_ = tnl::Quaternion::LookAt(_mesh->pos_, _player_ref->GetPos(), local_axis_y);
}