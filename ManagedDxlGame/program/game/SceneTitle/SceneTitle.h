#pragma once
#include "../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"


class SceneTitle : public SceneBase {
public:

	SceneTitle();

	void Render() override;
	void Update(const float deltaTime) override;

private:

	tnl::Sequence<SceneTitle> _sequence = tnl::Sequence<SceneTitle>(this, &SceneTitle::SeqIdle);
	bool SeqIdle(float deltaTime);

	void MakeBackGround_MonoTransition(const float deltaTime);
	void GameStartFadeOut(const float deltaTime);
	void MakeTitleLogo_FlushEffect(const float deltaTime);

private:

	Shared<dxe::ScreenEffect> _screenEffect = nullptr;
	Shared<dxe::ShadowMap>    _shadowMap = nullptr;

private:

	int _titleLogo_hdl{};
	int _backGround_hdl{};
	int _tapSE_hdl{};

	float _blur_alpha = 1.0f;
	float _blur_timer = 0.0f;

	float trans_time_cb{};
	float trans_time_cr{};
	float trans_time_logo_lights{};
};