#pragma once


namespace inl {

	class CustomException;

	// �X�e�[�W�̃e�N�X�`���@��͗l

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