#pragma once


namespace inl {

	class FreeLookCamera;
	class CustomException;

	// ステージのテクスチャ　空模様

	class SkyBox
	{
	public:

		SkyBox();

		void Update();
		void Render(const Shared<inl::FreeLookCamera>& camera);

	private:

		void SetSkyBoxTexture_CurrentStage(Shared<inl::CustomException>& cus);

	public:

		Shared<dxe::Mesh> _skybox = nullptr;
	};
}