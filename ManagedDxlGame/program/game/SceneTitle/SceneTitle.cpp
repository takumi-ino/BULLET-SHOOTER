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
	_titleLogo_hdl = LoadGraph("graphics/Scene/titleLogo.png");

	_tapSE_hdl = LoadSoundMem("sound/se/tap.mp3");

	//SoundManager::GetInstance().LoadBGM("sound/bgm/title.mp3");
	//SoundManager::GetInstance().PlayBGM();
}



void SceneTitle:: Render() {

	_shadowMap->reserveBegin();
	_screenEffect->renderBegin();

	// ”wŒi‰æ‘œ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _BACKGROUND_ALPHA);
	DrawRotaGraph(_BACKGROUND_POS_X, _BACKGROUND_POS_Y, 1, 0, _backGround_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ƒƒS
	DrawRotaGraph(_TITLELOGO_POS_X, _TITLELOGO_POS_Y, _TITLELOGO_EXTEND_RATE, 0, _titleLogo_hdl, true);

	_screenEffect->renderEnd();
	_shadowMap->reserveEnd();


	// ƒfƒoƒbƒOî•ñ•`‰æ
	//DrawDefaultLightGuiController();
	//_screenEffect->drawGuiController({ 0, 0 });
}



void SceneTitle::MakeFlushEffect_TitleLogo(float deltaTime)
{
	_transTime_logoLights += deltaTime;

	// tnl::SingleOscillationy“à‚Å
	// U“®ŠJŽnˆÊ’u‚É + 1.5f‚Ì•ÏX‚ð‰Á‚¦‚Ä‚¢‚é‚½‚ß‘¼‚ÌêŠ‚ÅŽg—p‚·‚éÛ‚É‚Í’ˆÓ
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

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {

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