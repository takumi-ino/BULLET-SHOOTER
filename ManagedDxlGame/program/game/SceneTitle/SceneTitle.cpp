#include "../DxLibEngine.h"
#include "../SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "../Manager/Scene/SceneManager.h"
#include "../SceneResult/SceneResult.h"
#include "../Manager/Sound/SoundManager.h"
#include "SceneTitle.h"



SceneTitle::SceneTitle() {

	_shadowMap = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);
	_screenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	_screenEffect->loadStatus("screenEffect/titleSceneEffect.bin");

	_backGround_hdl = LoadGraph("graphics/Scene/titleBackGround.png");
	_titleLogo_hdl = LoadGraph("graphics/Scene/titleLogo_star.png");

	_tapSE_hdl = LoadSoundMem("sound/se/tap.mp3");

	//SoundManager::GetInstance().LoadBGM("sound/bgm/title.mp3");
	//SoundManager::GetInstance().PlayBGM();
}



void SceneTitle:: Render() {

	_shadowMap->reserveBegin();
	_screenEffect->renderBegin();

	// 背景画像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	DrawRotaGraph(640, 250, 1, 0, _backGround_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ロゴ
	DrawRotaGraph(630, 230, 1.4f, 0, _titleLogo_hdl, true);

	_screenEffect->renderEnd();
	_shadowMap->reserveEnd();


	// デバッグ情報描画
	//DrawDefaultLightGuiController();
	//_screenEffect->drawGuiController({ 0, 0 });
}


void SceneTitle::Update(float deltaTime) {

	_sequence.update(deltaTime);

	GameStartFadeOut(deltaTime);
	
	MakeBackGround_MonoTransition(deltaTime);

	MakeTitleLogo_FlushEffect(deltaTime);
}



void SceneTitle::MakeTitleLogo_FlushEffect(float deltaTime)
{
	trans_time_logo_lights += deltaTime;

	//　tnl::SingleOscillationy内で
	// 振動開始位置に + 1.5fの変更を加えているため他の場所で使用する際には注意
	float bloom = tnl::SingleOscillationy(tnl::eOscStart::STOK, 0, 1.3f, trans_time_logo_lights);
	_screenEffect->setBloomThreshold(bloom * 70);
}



void SceneTitle::MakeBackGround_MonoTransition(float deltaTime)
{
	trans_time_cb += deltaTime;
	trans_time_cr -= deltaTime;

	float cb = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0, 1.25f, trans_time_cb);
	float cr = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0, 1.25f, trans_time_cr);

	_screenEffect->setMonoCb(cb * 30);
	_screenEffect->setMonoCr(cr * 30);
}



void SceneTitle::GameStartFadeOut(float deltaTime)
{
	_blur_timer += deltaTime;
	if (_blur_timer >= 0.125f) {

		_blur_alpha -= (_sequence.getProgressTime() / 0.01f * 1.0f);
		_blur_timer = 0.0f;
	}
	if (_blur_alpha <= 0) _blur_alpha = 0;
	_screenEffect->setBlurAlpha(_blur_alpha);
}



bool SceneTitle::SeqIdle(float deltaTime) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {

		PlaySoundMem(_tapSE_hdl, DX_PLAYTYPE_BACK);
		SoundManager::GetInstance().DestroyBGM();

		auto mgr = SceneManager::GetInstance();
		mgr->ChangeScene(new SceneSelectDifficulty());
	}
	return true;
}