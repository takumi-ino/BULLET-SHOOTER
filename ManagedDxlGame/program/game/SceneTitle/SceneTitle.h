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

		void SetupAssetsData() noexcept;         // 画像やエフェクトの初期化
		void RenderBackGroundAndLogo() noexcept; // 背景とタイトルロゴ描画

		void MoveToSceneSelectDifficulty();

		void MakeMonoTransition_BackGround(const float deltaTime) noexcept; // 背景エフェクト
		void MakeFlushEffect_TitleLogo(const float deltaTime) noexcept;     // タイトルロゴエフェクト

	private:

		Shared<dxe::ScreenEffect> _screenEffect = nullptr;
		Shared<dxe::ShadowMap>    _shadowMap = nullptr;

	private:

		int   _titleLogo_hdl{};
		int   _backGround_hdl{};
		int   _tapSE_hdl{};

		//　YCbCr・・・色を輝度と色相で表現したもの
		float _transTime_cb{};      // 青系統
		float _transTime_cr{};      // 赤系統
		float _transTime_logoLights{};
	};
