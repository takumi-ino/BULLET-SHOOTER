#include "../DxLibEngine.h"
#include "../SceneTitle/SceneTitle.h"
#include "../Manager/Scene/SceneManager.h"
#include "SceneResult.h"


SceneResult::SceneResult(const std::string difficulty, const int totalScore)
	: _totalScore(totalScore), _difficulty(difficulty) {

	_backGround_hdl = LoadGraph("graphics/Scene/resultBackGround.png");

	_resultSE_hdl = LoadSoundMem("sound/se/result.mp3");
	PlaySoundMem(_resultSE_hdl, DX_PLAYTYPE_BACK);
}


void SceneResult::Render() {

	// ”wŒi‰æ‘œ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _backGround_baseAlpha);
	DrawRotaGraph(_backGround_posX, _backGround_posY, _backGround_extendRate, 0, _backGround_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(70);
	DrawString(_resultText_posX, _resultText_posY, "Result", GetColor(0, 200, 100));

	SetFontSize(40);
	DrawFormatString(_difficultyText_posX, _difficultyText_posY, GetColor(0, 200, 100), "%s", _difficulty.c_str());
	SetFontSize(30);
	DrawFormatString(_totalScoreText_posX, _totalScoreText_posY, GetColor(0, 200, 100), "TotalScore  %d", _totalScore);

	SetFontSize(DEFAULT_FONT_SIZE);
}


void SceneResult::Update(float deltaTime) {	
	_sequence.update(deltaTime);
}


bool SceneResult::SeqIdle(float deltaTime) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {

		auto mgr = SceneManager::GetInstance();
		mgr->ChangeScene(new SceneTitle());
	}

	return true;
}