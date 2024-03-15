#include "../../DxLibEngine.h"
#include "PauseMenu.h"
#include "../../Manager/Scene/SceneManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../ScenePlay/ScenePlay.h"
#include "../../SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "../game/ScenePlay/Character/Player/Player.h"
#include "../../InputFuncTable.h"


bool PauseMenu::_isShowPauseOption = false;


PauseMenu::PauseMenu(const Shared<Player>& player) {

	_player_ref = player;
	_scenePlay = std::make_shared<ScenePlay>();
}


void PauseMenu::UpdatePauseMenuCursor_ByInput() noexcept {

	if (InputFuncTable::IsButtonDownTrigger_UP())
	{
		_menuIndex--;
		if (_menuIndex < 0) _menuIndex = _MENU_INDEX_COUNT - 1;
	}
	if (InputFuncTable::IsButtonDownTrigger_DOWN())
	{
		_menuIndex++;
		if (_menuIndex >= _MENU_INDEX_COUNT) _menuIndex = 0;
	}
}


void PauseMenu::PickPauseMenuItemByInput() {

	if (_MENU_INDEX_COUNT == 4) {
		// �Q�[���ĊJ
		if (_menuIndex == 0) {

			if (InputFuncTable::IsButtonTrigger_ENTER())
				_isShowPauseOption = false;
		}
		// ����m�F��ʕ\��
		if (_menuIndex == 1) {

			if (InputFuncTable::IsButtonTrigger_ENTER())
				_isShowConfigInfo = true;
			else if (InputFuncTable::IsButtonTrigger_CANCEL())
				_isShowConfigInfo = false;
		}
		// ���X�^�[�g
		if (_menuIndex == 2) {

			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				ResetGame();

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(
					new ScenePlay(ScenePlay::GetGameDifficulty(), ScenePlay::GetStageID()));
			}
		}
		// ��Փx�I���ɖ߂�
		if (_menuIndex == 3) {

			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				ResetGame();

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(new SceneSelectDifficulty());
			}
		}
	}
	else {
		if (_menuIndex == 0) {

			// ����m�F��ʕ\��
			if (InputFuncTable::IsButtonTrigger_ENTER())
				_isShowConfigInfo = true;
			else if (InputFuncTable::IsButtonTrigger_CANCEL())
				_isShowConfigInfo = false;
		}
		if (_menuIndex == 1) {

			// ���X�^�[�g
			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				ResetGame();

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(
					new ScenePlay(ScenePlay::GetGameDifficulty(), ScenePlay::GetStageID()));
			}
		}
		if (_menuIndex == 2) {

			// ��Փx�I���ɖ߂�
			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				ResetGame();

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(new SceneSelectDifficulty());
			}
		}
	}
}


void PauseMenu::ResetGame()
{
	SoundManager::GetInstance().DestroyStageBGM(false);

	_player_ref->SetHP(_player_ref->GetMaxHP());
}


void PauseMenu::RenderConfigStateInfo() noexcept {

	int color = -1;
	int upSide = 200;
	int leftSide = 700;
	int y_offset = 30;

	const char* descriptions[] = {

		 "�ړ��@      �L�[�{�[�hWASD�L�[�A�Q�[���p�b�h�\���L�[",
		 "�O�i�@      �L�[�{�[�hSpace�L�[�A�Q�[���p�b�hL1�ALB",
		 "�X���[�ړ�  �L�[�{�[�hShift�L�[�A�Q�[���p�b�h���AY",
		 "�e���ˁ@    �}�E�X���A�Q�[���p�b�h�Z�AB",
		 "�J�����Œ�@�}�E�X�E�A�Q�[���p�b�hR1�ARB",
		 "���_����@  �}�E�X�A�Q�[���p�b�h���X�e�B�b�N",
		 "�{���@�@    �}�E�X�����A",
	};

	SetFontSize(22);

	for (int i = 0; i < 7; i++) {

		DrawStringEx(leftSide, upSide + (y_offset * i), color, descriptions[i]);
	}

	DrawStringEx(leftSide + 450, upSide + (y_offset * 7), color, "Back");
}


void PauseMenu::RenderPauseMenuItems() noexcept {

	int baseColor = 150;

	int a = GetColor(baseColor, baseColor, baseColor),
		b = GetColor(baseColor, baseColor, baseColor),
		c = GetColor(baseColor, baseColor, baseColor),
		d = GetColor(baseColor, baseColor, baseColor);

	if (_MENU_INDEX_COUNT == 4) {
		if (_menuIndex == 0) a = -1;
		if (_menuIndex == 1) b = -1;
		if (_menuIndex == 2) c = -1;
		if (_menuIndex == 3) d = -1;
	}
	else {
		if (_menuIndex == 0) b = -1;
		if (_menuIndex == 1) c = -1;
		if (_menuIndex == 2) d = -1;
	}

	int upSide = 200;
	int upSideOffset = 30;

	int leftSide = 120;
	SetFontSize(25);

	if (_player_ref->GetHP() != 0) {

		_MENU_INDEX_COUNT = 4;
		DrawStringEx(leftSide, upSide, a, "�ĊJ����");
		upSide += 30;
	}
	else {
		_MENU_INDEX_COUNT = 3;
	}

	DrawStringEx(leftSide, upSide, b, "����m�F");
	DrawStringEx(leftSide, upSide + upSideOffset, c, "���݂̃X�e�[�W����蒼��");
	DrawStringEx(leftSide, upSide + upSideOffset * 2, d, "�Q�[�����I������");
}


void PauseMenu::Render() {

	RenderPauseMenuItems();

	if (_isShowConfigInfo)
		RenderConfigStateInfo();
}


void PauseMenu::Update() {

	UpdatePauseMenuCursor_ByInput();
	PickPauseMenuItemByInput();
}