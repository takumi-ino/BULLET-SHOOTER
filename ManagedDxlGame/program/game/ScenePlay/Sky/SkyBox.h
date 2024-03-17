#pragma once


namespace inl {

	class FreeLookCamera;


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