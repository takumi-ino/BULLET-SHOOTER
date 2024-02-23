#pragma once
#include "../game/DxLibEngine.h"

class Gunport
{
public:

	Gunport();

	void ManageGunportCount(std::vector<Shared<Gunport>>& gunPort);

	void Render(const Shared<dxe::Camera> playerCamera);

public:

	Shared<dxe::Mesh> _mesh;

private:

	int _bulletPowerPhase{};
};