#include "../../DxLibEngine.h"
#include "../../SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "../../ScenePlay/ScenePlay.h"
#include "../Character/Player/Player.h"
#include "../../Manager/Scene/SceneManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "PauseMenu.h"


bool PauseMenu::_isShowPauseOption = false;


PauseMenu::PauseMenu(const int stage_id, const std::string level) {

	_scenePlay = std::make_shared<ScenePlay>();
	_currentStage = stage_id;
	_selectedLevel = level;
}


void PauseMenu::UpdatePauseMenuCursor_ByInput() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
	{
		_menuIndex--;
		if (_menuIndex < 0) _menuIndex = MENU_INDEX_COUNT - 1;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))
	{
		_menuIndex++;
		if (_menuIndex >= MENU_INDEX_COUNT) _menuIndex = 0;
	}
}

void PauseMenu::PickPauseMenuItemByInput() {

	if (MENU_INDEX_COUNT == 4) {
		// �Q�[���ĊJ
		if (_menuIndex == 0) {

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN))
				_isShowPauseOption = false;
		}
		// ����m�F��ʕ\��
		if (_menuIndex == 1) {

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN))
				_isShowConfigInfo = true;
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK))
				_isShowConfigInfo = false;
		}
		// ���X�^�[�g
		if (_menuIndex == 2) {

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				SoundManager::GetInstance().DestroyStageBGM(false);

				Player::_hp = Player::_MAX_HP;

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(new ScenePlay(_selectedLevel, _currentStage));
			}
		}
		// ��Փx�I���ɖ߂�
		if (_menuIndex == 3) {

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				SoundManager::GetInstance().DestroyStageBGM(false);

				Player::_hp = Player::_MAX_HP;

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(new SceneSelectDifficulty());
			}
		}
	}
	else {
		if (_menuIndex == 0) {
		// ����m�F��ʕ\��
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN))
				_isShowConfigInfo = true;
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK))
				_isShowConfigInfo = false;
		}
		if (_menuIndex == 1) {
			// ���X�^�[�g
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				SoundManager::GetInstance().DestroyStageBGM(false);

				Player::_hp = Player::_MAX_HP;

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(new ScenePlay(_selectedLevel,_currentStage));
			}
		}
		if (_menuIndex == 2) {
			// ��Փx�I���ɖ߂�
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

				SoundManager::GetInstance().DestroyStageBGM(false);

				Player::_hp = Player::_MAX_HP;

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(new SceneSelectDifficulty());
			}
		}
	}
}



void PauseMenu::RenderConfigStateInfo() {

	int color = -1;
	int upSide = 200;
	int leftSide = 700;
	int y_offset = 30;

	SetFontSize(22);
	DrawStringEx(leftSide, upSide, color, "�ړ��@�@�L�[�{�[�hWASD�L�[�A�Q�[���p�b�h�\���L�[");
	DrawStringEx(leftSide, upSide + y_offset, color, "�O�i�@�@�L�[�{�[�hSpace�L�[�A�Q�[���p�b�hL1�ALB");
	DrawStringEx(leftSide, upSide + (y_offset * 2), color, "�X���[�ړ��@�@�L�[�{�[�hShift�L�[�A�Q�[���p�b�h���AY");
	DrawStringEx(leftSide, upSide + (y_offset * 3), color, "�e���ˁ@�@�}�E�X���A�Q�[���p�b�h�Z�AB");
	DrawStringEx(leftSide, upSide + (y_offset * 4), color, "�J�����Œ�@�@�}�E�X�E�A�Q�[���p�b�hR1�ARB");
	DrawStringEx(leftSide, upSide + (y_offset * 5), color, "���_����@�@�}�E�X�A�Q�[���p�b�h���X�e�B�b�N");
	DrawStringEx(leftSide, upSide + (y_offset * 6), color, "�{���@�@�}�E�X�����A");

	DrawStringEx(leftSide + 450, upSide + (y_offset * 7), color, "Back");
}


void PauseMenu::RenderPauseMenuItems() {

	int baseColor = 150;

	int a = GetColor(baseColor, baseColor, baseColor),
		b = GetColor(baseColor, baseColor, baseColor),
		c = GetColor(baseColor, baseColor, baseColor),
		d = GetColor(baseColor, baseColor, baseColor);

	if (MENU_INDEX_COUNT == 4) {
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

	if (Player::_hp != 0) {

		MENU_INDEX_COUNT = 4;
		DrawStringEx(leftSide, upSide, a, "�ĊJ����");
		upSide += 30;
	}
	else {
		MENU_INDEX_COUNT = 3;
	}

	DrawStringEx(leftSide, upSide, b, "����m�F");
	DrawStringEx(leftSide, upSide + upSideOffset, c, "���݂̃X�e�[�W����蒼��");
	DrawStringEx(leftSide, upSide + upSideOffset * 2, d, "�Q�[�����I������");
}


void PauseMenu::Render() {

	RenderPauseMenuItems();

	if (_isShowConfigInfo) RenderConfigStateInfo();
}


void PauseMenu::Update() {

	UpdatePauseMenuCursor_ByInput();
	PickPauseMenuItemByInput();
}