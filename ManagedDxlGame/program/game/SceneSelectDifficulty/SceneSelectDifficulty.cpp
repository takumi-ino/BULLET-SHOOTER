#include "../DxLibEngine.h"
#include "../SceneTitle/SceneTitle.h"
#include "../ScenePlay/ScenePlay.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Score/ScoreManager.h"
#include "SceneSelectDifficulty.h"


SceneSelectDifficulty::SceneSelectDifficulty() {

	_backGround_hdl = LoadGraph("graphics/Scene/select.jpg");
}



void SceneSelectDifficulty::UpdateSelectDifficultyCursor_ByInput() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
	{
		_levelIndex--;
		if (_levelIndex < 0) _levelIndex = _LELVEL_COUNT - 1; // 末尾へ
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))  // 下キー
	{
		_levelIndex++;
		if (_levelIndex >= _LELVEL_COUNT) _levelIndex = 0;	  // 先頭へ
	}
}



void SceneSelectDifficulty::RenderDifficultiesAndAnnotation() {

	int r = 1, g = 1, b = 1, SummonBoss = 1;

	for (int i = 0; i < _LELVEL_COUNT; i++) {

		if (i == _levelIndex && _levelIndex == 0)	r = -1;
		else if (i == _levelIndex && _levelIndex == 1)	g = -1;
		else if (i == _levelIndex && _levelIndex == 2)	b = -1;
		else if (i == _levelIndex && _levelIndex == 3)	SummonBoss = -1;

		SetFontSize(66);
		DrawStringEx(_LEVEL_POS_X, _LEVEL_POS_Y, r, "Easy");
		DrawStringEx(_LEVEL_POS_X - 40, _LEVEL_POS_Y + 140, g, "Normal");
		DrawStringEx(_LEVEL_POS_X - 80, _LEVEL_POS_Y + 280, b, "Hard");
		DrawStringEx(_LEVEL_POS_X - 120, _LEVEL_POS_Y + 420, SummonBoss, "Lunatic");

		SetFontSize(20);
		DrawStringEx(ANNOTATION_POS_X, ANNOTATION_POS_Y, r,
			"もっとも易しい難易度です。\nシューティングゲームは若干苦手、\nもしくはノーマルでは難しすぎる、という方向けです。");
		DrawStringEx(ANNOTATION_POS_X - 40, ANNOTATION_POS_Y + 140, g,
			"もっとも標準の難易度です。\nシューティングが普通な方向け難易度です。");
		DrawStringEx(ANNOTATION_POS_X - 80, ANNOTATION_POS_Y + 280, b, 
			"ちょっと高めの難易度です。\nアーケードでシューティングをプレイする方には、\n丁度いいくらいになっています。");
		DrawStringEx(ANNOTATION_POS_X - 120, ANNOTATION_POS_Y + 420, SummonBoss,
			"奇特な難易度です。");
	}
}



void SceneSelectDifficulty::DecideSelectedLevel_ByInput() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {

		ScoreManager::GetInstance().InitScoreInstance();

		int start_stage = 1;

		if (_levelIndex == 0) {

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Easy", start_stage));
		}
		if (_levelIndex == 1) {

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Normal", start_stage));
		}
		if (_levelIndex == 2) {
			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Hard", start_stage));
		}
		if (_levelIndex == 3) {

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Lunatic", start_stage));
		}
	}
}


void SceneSelectDifficulty::RenderBackGround() {
	DrawRotaGraph(600, 360, 1, 0, _backGround_hdl, false);
}

void SceneSelectDifficulty::Render() {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	RenderBackGround();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	RenderDifficultiesAndAnnotation();
}

void SceneSelectDifficulty::Update(float deltaTime) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		auto mgr = SceneManager::GetInstance();
		mgr->ChangeScene(new SceneTitle());
	}

	UpdateSelectDifficultyCursor_ByInput();
	DecideSelectedLevel_ByInput();
}