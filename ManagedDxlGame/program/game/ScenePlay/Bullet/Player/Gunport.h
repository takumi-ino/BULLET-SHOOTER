#pragma once

class Gunport
{
public:

	Gunport();

	// PlayerBullet::_bulletPowerRate�̒l�����ȏ㑝���邲�ƂɘA���C��ǉ�����
	void ManageGunportCount(std::vector<Shared<Gunport>>& gunPort);

	void Render(const Shared<dxe::Camera> playerCamera);

public:

	Shared<dxe::Mesh> _mesh;

private:

	int _bulletPowerPhase{};
};