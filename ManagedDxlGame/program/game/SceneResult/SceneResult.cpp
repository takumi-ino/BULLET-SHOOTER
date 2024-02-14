#include "../DxLibEngine.h"
#include "../SceneTitle/SceneTitle.h"
#include "../Manager/Scene/SceneManager.h"
#include "SceneResult.h"

SceneResult::SceneResult(const std::string level, const int totalScore) {

	_backGround_hdl = LoadGraph("graphics/Scene/resultBackGround.png");
	_totalScore = totalScore;
	_difficulty = level;

	_resultSE_hdl = LoadSoundMem("sound/se/result.mp3");
	PlaySoundMem(_resultSE_hdl, DX_PLAYTYPE_BACK);
}



void SceneResult::Render() {

	// ”wŒi‰æ‘œ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawRotaGraph(640, 380, 0.8f, 0, _backGround_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(70);
	DrawString(530, 150, "Result", GetColor(0, 200, 100));

	SetFontSize(40);
	DrawFormatString(320, 250, GetColor(0, 200, 100), "%s", _difficulty.c_str());
	SetFontSize(30);
	DrawFormatString(800, 270, GetColor(0, 200, 100), "TotalScore  %d", _totalScore);

	SetFontSize(20);
}


void SceneResult::Update(float deltaTime) {
	
	_sequence.update(deltaTime);
}


bool SceneResult::SeqIdle(float deltaTime) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {

		DeleteSoundMem(_resultSE_hdl);

		auto mgr = SceneManager::GetInstance();
		mgr->ChangeScene(new SceneTitle());
	}

	return true;
}