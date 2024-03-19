#pragma once

namespace inl {

	class CustomException;
}


	class SceneTitle : public SceneBase
	{
	public:

		SceneTitle();

		~SceneTitle() override {
			DeleteGraph(_backGround_hdl);
			DeleteGraph(_titleLogo_hdl);
			DeleteSoundMem(_tapSE_hdl);
		}

		void Render() override;
		void Update(const float deltaTime) override;

	private:

		// �摜��G�t�F�N�g�̏�����
		void SetupAssetsData(const Shared<inl::CustomException>& cus);

		// �w�i�ƃ^�C�g�����S�`��
		void RenderBackGroundAndLogo() noexcept; 

		// �w�i�G�t�F�N�g
		void MakeMonoTransition_BackGround(const float deltaTime) noexcept;

		// �^�C�g�����S�G�t�F�N�g
		void MakeFlushEffect_TitleLogo(const float deltaTime) noexcept;

		void MoveToSceneSelectDifficulty();

	private:

		Shared<dxe::ScreenEffect> _screenEffect = nullptr;
		Shared<dxe::ShadowMap>    _shadowMap = nullptr;

	private:

		int   _tapSE_hdl{};

		// �w�i�G�t�F�N�g-----------------------------------------
		//�@YCbCr�E�E�E�F���P�x�ƐF���ŕ\����������

		float _transTime_cb{};      // cb�͐n��
		float _transTime_cr{};      // cr�͐Ԍn��

		int   _backGround_hdl{};

		// �^�C�g�����S-----------------------------------------
		float _transTime_logoLights{};
		int   _titleLogo_hdl{};
	};