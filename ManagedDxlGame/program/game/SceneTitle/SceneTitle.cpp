#include "../DxLibEngine.h"
#include "../Manager/Scene/SceneBase.h"
#include "SceneTitle.h"
#include "../SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Sound/SoundManager.h"
#include "../Utility/InputFuncTable.h"
#include "../Utility/CustomException.h"


namespace {

	// 背景画像
	const int _BACKGROUND_POS_X{ 650 };
	const int _BACKGROUND_POS_Y{ 250 };
	const int _BACKGROUND_ALPHA{ 80 };
	const int _BACKGROUND_EFFECT_OSCILLATE_SPEED{ 30 };

	// ロゴ
	const int   _TITLELOGO_POS_X{ 635 };
	const int   _TITLELOGO_POS_Y{ 230 };
	const int   _TITLELOGO_EFFECT_OSCILLATE_SPEED{ 70 };
	const float _TITLELOGO_EXTEND_RATE{ 1.4f };

	// エフェクトの振幅速度
	const float _EFFECT_TRANS_OSCILLATE_RATE{ 1.3f };
}


	SceneTitle::SceneTitle() 
	{
		Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

		SetupAssetsData(cus);

		std::string sound = cus->TryLoadSoundPath("sound/bgm/title.mp3", "SceneTitle::SceneTitle()");

		SoundManager::GetInstance().LoadBGM(sound);
		SoundManager::GetInstance().PlayBGM();
	}


	void SceneTitle::SetupAssetsData(const Shared<inl::CustomException>& cus) 
	{
		// 画像ハンドル取得
		_backGround_hdl = cus->TryLoadGraph("graphics/Scene/titleBackGround.jpg", "SceneTitle::SetupAssetsData()");
		_titleLogo_hdl = cus->TryLoadGraph("graphics/Scene/titleLogo_star.png", "SceneTitle::SetupAssetsData()");

		// SEハンドル取得
		_tapSE_hdl = cus->TryLoadSound("sound/se/tap.mp3", "SceneTitle::SetupAssetsData()");

		// バイナリーファイルロード
		std::string binary = cus->TryLoadBinaryPath("screenEffect/titleSceneEffect.bin", "SceneTitle::SetupAssetsData()");

		// スクリーンエフェクト生成
		_screenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		_screenEffect->loadStatus(binary);

		// シャドウマップ生成
		_shadowMap = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);
	}


	void SceneTitle::MakeFlushEffect_TitleLogo(const float deltaTime) noexcept
	{
		_transTime_logoLights += deltaTime;
		
		// tnl::SingleOscillationy 関数内で振動開始位置に + 1.5fの変更を加えている
		float bloom = 
			tnl::SingleOscillationy(
				tnl::eOscStart::STOK,
				0,
				_EFFECT_TRANS_OSCILLATE_RATE, 
				_transTime_logoLights
			);

		_screenEffect->setBloomThreshold(bloom * _TITLELOGO_EFFECT_OSCILLATE_SPEED);
	}


	void SceneTitle::MakeMonoTransition_BackGround(const float deltaTime) noexcept
	{
		_transTime_cb += deltaTime;
		_transTime_cr -= deltaTime;

		// 青系統
		float cb = 
			tnl::SingleOscillationy(
				tnl::eOscStart::CENTER,
				0,
				_EFFECT_TRANS_OSCILLATE_RATE,
				_transTime_cb
			);

		// 赤系統
		float cr = 
			tnl::SingleOscillationy(
				tnl::eOscStart::CENTER, 
				0,
				_EFFECT_TRANS_OSCILLATE_RATE,
				_transTime_cr
			);

		_screenEffect->setMonoCb(cb * _BACKGROUND_EFFECT_OSCILLATE_SPEED);
		_screenEffect->setMonoCr(cr * _BACKGROUND_EFFECT_OSCILLATE_SPEED);
	}


	void SceneTitle::MoveToSceneSelectDifficulty()
	{
		if (inl::InputFuncTable::IsButtonTrigger_ENTER()) {

			PlaySoundMem(_tapSE_hdl, DX_PLAYTYPE_BACK);
			SoundManager::GetInstance().DestroyBGM();

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new SceneSelectDifficulty());
		}
	}


	void SceneTitle::RenderBackGroundAndLogo() noexcept
	{
		// 背景画像
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _BACKGROUND_ALPHA);
		DrawRotaGraph(_BACKGROUND_POS_X, _BACKGROUND_POS_Y, 1, 0, _backGround_hdl, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ロゴ
		DrawRotaGraph(_TITLELOGO_POS_X, _TITLELOGO_POS_Y, _TITLELOGO_EXTEND_RATE, 0, _titleLogo_hdl, true);
	}


	void SceneTitle::Render() {

		_shadowMap->reserveBegin();
		_screenEffect->renderBegin();

		RenderBackGroundAndLogo();

		_screenEffect->renderEnd();
		_shadowMap->reserveEnd();

		// デバッグ情報描画
		//DrawDefaultLightGuiController();
		//_screenEffect->drawGuiController({ 0, 0 });
	}


	void SceneTitle::Update(const float deltaTime) {

		MoveToSceneSelectDifficulty();

		MakeMonoTransition_BackGround(deltaTime);

		MakeFlushEffect_TitleLogo(deltaTime);
	}