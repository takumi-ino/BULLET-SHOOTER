#pragma once
#include "../../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"


class SceneSelectDifficulty : public SceneBase
{
public:

	SceneSelectDifficulty();

	void Render() override;
	void Update(const float deltaTime) override;

private:

	void UpdateSelectDifficultyCursor_ByInput();

	void RenderDifficultiesAndAnnotation();

	void RenderBackGround();

	void DecideSelectedLevel_ByInput();

private:

	int _backGround_hdl{};

	const int _LEVEL_POS_X = 560;
	const int _LEVEL_POS_Y = 110;

	const int ANNOTATION_POS_X = 540;
	const int ANNOTATION_POS_Y = 180;

	const int _LELVEL_COUNT = 4;
	int _levelIndex = 0;
};