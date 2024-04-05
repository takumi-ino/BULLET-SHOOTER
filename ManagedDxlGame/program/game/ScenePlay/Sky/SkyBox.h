#pragma once


namespace inl {

	class FreeLookCamera;
	class CustomException;

	// �X�e�[�W�̃e�N�X�`���@��͗l

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