#pragma once


namespace inl {

	class CustomException;

	// ステージのテクスチャ　空模様

	class SkyBox
	{
	public:

		SkyBox();

		void Update();
		void Render();

	private:

		void SetSkyBoxTexture_CurrentStage(Shared<inl::CustomException>& cus);

	public:

		Shared<dxe::Mesh> _mesh = nullptr;
	};
}