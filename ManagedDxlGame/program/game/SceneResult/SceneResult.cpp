#include "../DxLibEngine.h"
#include "../Manager/Scene/SceneBase.h"
#include "SceneResult.h"
#include "../SceneTitle/SceneTitle.h"
#include "../Manager/Scene/SceneManager.h"
#include "../InputFuncTable.h"


namespace {

	const int   _backGround_posX{ 640 };
	const int   _backGround_posY{ 380 };
	const int   _backGround_baseAlpha{ 220 };
	const float _backGround_extendRate{ 0.8f };

	// Result
	const int   _resultText_posX{ 530 };
	const int   _resultText_posY{ 150 };

	// “ïˆÕ“x
	const int   _difficultyText_posX{ 320 };
	const int   _difficultyText_posY{ 250 };

	// TotalScore
	const int   _totalScoreText_posX{ 800 };
	const int   _totalScoreText_posY{ 270 };
}


SceneResult::SceneResult(const std::string difficulty, const int totalScore)
	: _TOTAL_SCORE(totalScore), _DIFFICULTY(difficulty) {

	_backGround_hdl = LoadGraph("graphics/Scene/resultBackGround.png");
	_resultSE_hdl = LoadSoundMem("sound/se/result.mp3");

	PlaySoundMem(_resultSE_hdl, DX_PLAYTYPE_BACK);
}


void SceneResult::RenderResult() noexcept
{
	SetFontSize(70);
	DrawString(_resultText_posX, _resultText_posY, "Result", GetColor(0, 200, 100));

	SetFontSize(40);
	DrawFormatString(_difficultyText_posX, _difficultyText_posY, GetColor(0, 200, 100), "%s", _DIFFICULTY.c_str());

	SetFontSize(30);
	DrawFormatString(_totalScoreText_posX, _totalScoreText_posY, GetColor(0, 200, 100), "TotalScore  %d", _TOTAL_SCORE);
}


void SceneResult::RenderBackGround() noexcept
{
	// ”wŒi‰æ‘œ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _backGround_baseAlpha);
	DrawRotaGraph(_backGround_posX, _backGround_posY, _backGround_extendRate, 0, _backGround_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void SceneResult::Render() {

	RenderBackGround();

	RenderResult();

	SetFontSize(DEFAULT_FONT_SIZE);
}


void SceneResult::MoveToSceneTitle()
{
	if (InputFuncTable::IsButtonTrigger_ENTER()) {

		auto mgr = SceneManager::GetInstance();
		mgr->ChangeScene(new SceneTitle());
	}
}


void SceneResult::Update(float deltaTime) {

	MoveToSceneTitle();
}