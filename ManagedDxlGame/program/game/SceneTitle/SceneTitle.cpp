#include "../DxLibEngine.h"
#include "../SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "../Manager/Scene/SceneManager.h"
#include "../SceneResult/SceneResult.h"
#include "../Manager/Sound/SoundManager.h"
#include "../InputFuncTable.h"
#include "SceneTitle.h"


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


SceneTitle::SceneTitle() {

	_shadowMap = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);
	_screenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	_screenEffect->loadStatus("screenEffect/titleSceneEffect.bin");

	_backGround_hdl = LoadGraph("graphics/Scene/titleBackGround.jpg");
	_titleLogo_hdl = LoadGraph("graphics/Scene/titleLogo_star.png");

	_tapSE_hdl = LoadSoundMem("sound/se/tap.mp3");

	SoundManager::GetInstance().LoadBGM("sound/bgm/title.mp3");
	SoundManager::GetInstance().PlayBGM();
}



void SceneTitle::Render() {

	_shadowMap->reserveBegin();
	_screenEffect->renderBegin();

	// 背景画像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _BACKGROUND_ALPHA);
	DrawRotaGraph(_BACKGROUND_POS_X, _BACKGROUND_POS_Y, 1, 0, _backGround_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ロゴ
	DrawRotaGraph(_TITLELOGO_POS_X, _TITLELOGO_POS_Y, _TITLELOGO_EXTEND_RATE, 0, _titleLogo_hdl, true);

	_screenEffect->renderEnd();
	_shadowMap->reserveEnd();


	// デバッグ情報描画
	//DrawDefaultLightGuiController();
	//_screenEffect->drawGuiController({ 0, 0 });
}



void SceneTitle::MakeFlushEffect_TitleLogo(float deltaTime)
{
	_transTime_logoLights += deltaTime;

	// tnl::SingleOscillationy内で
	// 振動開始位置に + 1.5fの変更を加えているため他の場所で使用する際には注意
	float bloom = tnl::SingleOscillationy(tnl::eOscStart::STOK, 0, _EFFECT_TRANS_OSCILLATE_RATE, _transTime_logoLights);

	_screenEffect->setBloomThreshold(bloom * _TITLELOGO_EFFECT_OSCILLATE_SPEED);
}



void SceneTitle::MakeMonoTransition_BackGround(float deltaTime)
{
	_transTime_cb += deltaTime;
	_transTime_cr -= deltaTime;

	float cb = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0, _EFFECT_TRANS_OSCILLATE_RATE, _transTime_cb);
	float cr = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0, _EFFECT_TRANS_OSCILLATE_RATE, _transTime_cr);

	_screenEffect->setMonoCb(cb * _BACKGROUND_EFFECT_OSCILLATE_SPEED);
	_screenEffect->setMonoCr(cr * _BACKGROUND_EFFECT_OSCILLATE_SPEED);
}



bool SceneTitle::SeqIdle(float deltaTime) {

	if (InputFuncTable::IsButtonTrigger_ENTER()) {

		PlaySoundMem(_tapSE_hdl, DX_PLAYTYPE_BACK);
		SoundManager::GetInstance().DestroyBGM();

		auto mgr = SceneManager::GetInstance();
		mgr->ChangeScene(new SceneSelectDifficulty());
	}
	return true;
}



void SceneTitle::Update(float deltaTime) {

	_sequence.update(deltaTime);

	MakeMonoTransition_BackGround(deltaTime);

	MakeFlushEffect_TitleLogo(deltaTime);
}