#include "../DxLibEngine.h"
#include "../SceneTitle/SceneTitle.h"
#include "../ScenePlay/ScenePlay.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Score/ScoreManager.h"
#include "SceneSelectDifficulty.h"


namespace {

	const int   _BACKGROUND_ALPHA{ 150 };

	// �e�L�X�g�I�t�Z�b�g
	const int   _TEXT_OFFSET_X{ 40 };
	const int   _TEXT_OFFSET_Y{ 140 };

	// ��Փx
	const int   _DIFFICULTY_TEXT_POS_X{ 570 };
	const int   _DIFFICULTY_TEXT_POS_Y{ 110 };
	const int   _DIFFICULTY_COUNT{ 4 };

	// ����
	const int   _ANNOTATION_POS_X{ 540 };
	const int   _ANNOTATION_POS_Y{ 180 };
}


SceneSelectDifficulty::SceneSelectDifficulty() {

	_backGround_hdl = LoadGraph("graphics/Scene/select.jpg");
}


void SceneSelectDifficulty::UpdateSelectDifficultyCursor_ByInput() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
	{
		_difficultyItemIndex--;

		if (_difficultyItemIndex < 0)
			_difficultyItemIndex = _DIFFICULTY_COUNT - 1;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))
	{
		_difficultyItemIndex++;

		if (_difficultyItemIndex >= _DIFFICULTY_COUNT)
			_difficultyItemIndex = 0;
	}
}


void SceneSelectDifficulty::RenderDifficultiesAndAnnotation() {

	int colors[] = { 1,1,1,1 };
	colors[_difficultyItemIndex] = -1;

	const char* difficulties[] =
	{ "Easy", "Normal", "Hard", "Lunatic" };

	const char* annotations[] =
	{ "�����Ƃ��Ղ�����Փx�ł�", "�����Ƃ��W���̓�Փx�ł�", "������ƍ��߂̓�Փx�ł�", "����ȓ�Փx�ł�" };

	for (int i = 0; i < _DIFFICULTY_COUNT; i++) {

		// ��Փx�\��--------------------------------------------
		SetFontSize(66);

		DrawStringEx(
			_DIFFICULTY_TEXT_POS_X - (_TEXT_OFFSET_X * i),
			_DIFFICULTY_TEXT_POS_Y + (_TEXT_OFFSET_Y * i),
			colors[i],
			difficulties[i]
		);
		// ���ߕ\��----------------------------------------------
		SetFontSize(DEFAULT_FONT_SIZE);

		DrawStringEx(
			_ANNOTATION_POS_X - (_TEXT_OFFSET_X * i),
			_ANNOTATION_POS_Y + (_TEXT_OFFSET_Y * i),
			colors[i],
			annotations[i]
		);
	}
}


void SceneSelectDifficulty::DecideSelectedLevel_ByInput() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {

		ScoreManager::GetInstance().InitScoreInstance();

		int startStage = 1;

		if (_difficultyItemIndex == 0) {

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Easy", startStage));
		}
		if (_difficultyItemIndex == 1) {

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Normal", startStage));
		}
		if (_difficultyItemIndex == 2) {
			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Hard", startStage));
		}
		if (_difficultyItemIndex == 3) {

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new ScenePlay("Lunatic", startStage));
		}
	}
}


void SceneSelectDifficulty::RenderBackGround() {
	DrawRotaGraph(600, 360, 1, 0, _backGround_hdl, false);
}


void SceneSelectDifficulty::Render() {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _BACKGROUND_ALPHA);
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