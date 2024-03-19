#pragma once


namespace inl {

	class FreeLookCamera;

	// �X�e�[�W�̃e�N�X�`���@��͗l

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