#pragma once


namespace inl {

	class FreeLookCamera;

	// ステージのテクスチャ　空模様

	class SkyBox
	{
	public:

		SkyBox();

		void Update();
		void Render(const Shared<inl::FreeLookCamera>& camera);

	public:

		Shared<dxe::Mesh> _skybox = nullptr;
	};
}