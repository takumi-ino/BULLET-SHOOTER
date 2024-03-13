#pragma once

class Gunport
{
public:

	Gunport();

	// PlayerBullet::_bulletPowerRate‚Ì’l‚ªˆê’èˆÈã‘‚¦‚é‚²‚Æ‚É˜A‘•–C‚ğ’Ç‰Á‚·‚é
	void ManageGunportCount(std::vector<Shared<Gunport>>& gunPort);

	void Render(const Shared<dxe::Camera> playerCamera);

public:

	Shared<dxe::Mesh> _mesh;

private:

	int _bulletPowerPhase{};
};