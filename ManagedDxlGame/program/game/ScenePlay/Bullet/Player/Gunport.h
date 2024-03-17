#pragma once

namespace inl {

	class Gunport
	{
	public:

		//　コンストラクタ-----------------------------------------------------------
		Gunport();

		//　-----------------------------------------------------------		
		// PlayerBullet::_bulletPowerRateの値が一定以上増えるごとに連装砲を追加
		void ManageGunportCount(std::vector<Shared<Gunport>>& gunPort);

		//　描画---------------------------------------------------------------
		void Render(const Shared<dxe::Camera> playerCamera);

	public:

		Shared<dxe::Mesh> _mesh;

	private:

		int _bulletPowerPhase{};
	};
}