#pragma once


	class SceneTitle : public SceneBase
	{
	public:

		SceneTitle();

		~SceneTitle() {
			DeleteGraph(_backGround_hdl);
			DeleteGraph(_titleLogo_hdl);
			DeleteSoundMem(_tapSE_hdl);
		}

		void Render() override;
		void Update(const float deltaTime) override;

	private:

		void SetupAssetsData() noexcept;         // �摜��G�t�F�N�g�̏�����
		void RenderBackGroundAndLogo() noexcept; // �w�i�ƃ^�C�g�����S�`��

		void MoveToSceneSelectDifficulty();

		void MakeMonoTransition_BackGround(const float deltaTime) noexcept; // �w�i�G�t�F�N�g
		void MakeFlushEffect_TitleLogo(const float deltaTime) noexcept;     // �^�C�g�����S�G�t�F�N�g

	private:

		Shared<dxe::ScreenEffect> _screenEffect = nullptr;
		Shared<dxe::ShadowMap>    _shadowMap = nullptr;

	private:

		int   _titleLogo_hdl{};
		int   _backGround_hdl{};
		int   _tapSE_hdl{};

		//�@YCbCr�E�E�E�F���P�x�ƐF���ŕ\����������
		float _transTime_cb{};      // �n��
		float _transTime_cr{};      // �Ԍn��
		float _transTime_logoLights{};
	};
