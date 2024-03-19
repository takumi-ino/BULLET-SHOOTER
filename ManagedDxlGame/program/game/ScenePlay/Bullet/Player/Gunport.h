#pragma once

namespace inl {

	class Gunport
	{
	public:

		Gunport();

		// PlayerBullet::_bulletPowerRate‚Ì’l‚ªˆê’èˆÈã‘‚¦‚é‚²‚Æ‚É˜A‘•–C‚ğ’Ç‰Á
		void ManageGunportCount(std::vector<Shared<Gunport>>& gunPort);

		//@•`‰æ
		void Render(const Shared<dxe::Camera> playerCamera);

	public:

		Shared<dxe::Mesh> _mesh;

	private:

		int _bulletPowerPhase{};
	};
}