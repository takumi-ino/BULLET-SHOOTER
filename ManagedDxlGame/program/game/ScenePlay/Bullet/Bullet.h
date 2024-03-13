#pragma once
#include "../../DxLibEngine.h"


class Bullet
{
public:

	Bullet(){}
	Bullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction)  {}

	virtual void Update(const float delta_time){}
	virtual void Render(Shared<dxe::Camera> mainCamera){}

public:

	Shared<dxe::Mesh> _mesh;

public:

	float _timer{};
};