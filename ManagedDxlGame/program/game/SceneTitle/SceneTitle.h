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

		// 画像やエフェクトの初期化
		void SetupAssetsData(const Shared<inl::CustomException>& cus);

		// 背景とタイトルロゴ描画
		void RenderBackGroundAndLogo() noexcept; 

		// 背景エフェクト
		void MakeMonoTransition_BackGround(const float deltaTime) noexcept;

		// タイトルロゴエフェクト
		void MakeFlushEffect_TitleLogo(const float deltaTime) noexcept;

		void MoveToSceneSelectDifficulty();

	private:

		Shared<dxe::ScreenEffect> _screenEffect = nullptr;
		Shared<dxe::ShadowMap>    _shadowMap = nullptr;

	private:

		int   _tapSE_hdl{};

		// 背景エフェクト-----------------------------------------
		//　YCbCr・・・色を輝度と色相で表現したもの

		float _transTime_cb{};      // cbは青系統
		float _transTime_cr{};      // crは赤系統

		int   _backGround_hdl{};

		// タイトルロゴ-----------------------------------------
		float _transTime_logoLights{};
		int   _titleLogo_hdl{};
	};