#pragma once
#include "../../DxLibEngine.h"


class Bullet
{
public:

	Bullet(){}
	Bullet(const tnl::Vector3& spawnPos, const tnl::Vector3& direction) {}
	virtual ~Bullet() {}


	virtual void Update(const float deltaTime){}
	virtual void Render(const Shared<dxe::Camera> mainCamera){}

public:

	Shared<dxe::Mesh> _mesh;

public:

	float _timer{};
};