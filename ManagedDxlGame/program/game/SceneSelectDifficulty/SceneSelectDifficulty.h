#pragma once
#include "../../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"


class SceneSelectDifficulty : public SceneBase
{
public:

	SceneSelectDifficulty();
	~SceneSelectDifficulty() { DeleteGraph(_backGround_hdl); }

	void Render() override;
	void Update(const float deltaTime) override;

private:

	void UpdateSelectDifficultyCursor_ByInput();

	void RenderDifficultiesAndAnnotation();

	void RenderBackGround();

	void DecideSelectedLevel_ByInput();

private:

	// 背景
	int         _backGround_hdl{};
	const int   _BACKGROUND_ALPHA{ 150 };

	// テキストオフセット
	const int   _TEXT_OFFSET_X{ 40 };
	const int   _TEXT_OFFSET_Y{ 140 };

	// 難易度
	const int   _DIFFICULTY_TEXT_POS_X{ 570 };
	const int   _DIFFICULTY_TEXT_POS_Y{ 110 };
	const int   _DIFFICULTY_COUNT{ 4 };
	int         _difficultyItemIndex{};

	// 注釈
	const int   _ANNOTATION_POS_X{ 540 };
	const int   _ANNOTATION_POS_Y{ 180 };
};