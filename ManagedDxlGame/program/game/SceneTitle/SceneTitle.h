#pragma once
#include "../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"


class SceneTitle : public SceneBase {
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

	tnl::Sequence<SceneTitle> _sequence = tnl::Sequence<SceneTitle>(this, &SceneTitle::SeqIdle);
	bool SeqIdle(float deltaTime);

	void MakeMonoTransition_BackGround(const float deltaTime);
	void MakeFlushEffect_TitleLogo(const float deltaTime);

private:

	Shared<dxe::ScreenEffect> _screenEffect = nullptr;
	Shared<dxe::ShadowMap>    _shadowMap = nullptr;

private:

	int   _titleLogo_hdl{};
	int   _backGround_hdl{};
	int   _tapSE_hdl{};

	float _blurAlpha = 1.0f;
	float _blurTimer = 0.0f;

	// îwåiâÊëú
	const int _BACKGROUND_POS_X{ 640 };
	const int _BACKGROUND_POS_Y{ 250 };
	const int _BACKGROUND_ALPHA{ 80 };
	const int _BACKGROUND_EFFECT_OSCILLATE_SPEED{ 30 };

	// ÉçÉS
	const int   _TITLELOGO_POS_X{ 630 };
	const int   _TITLELOGO_POS_Y{ 230 };
	const int   _TITLELOGO_EFFECT_OSCILLATE_SPEED{ 70 };
	const float _TITLELOGO_EXTEND_RATE{ 1.4f };




	float _transTime_cb{};
	float _transTime_cr{};
	float _transTime_logoLights{};

	const float _EFFECT_TRANS_OSCILLATE_RATE{ 1.3f };
};