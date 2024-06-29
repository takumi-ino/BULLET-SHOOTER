#include "../../DxLibEngine.h"
#include "PauseMenu.h"
// �}�l�[�W���[---------------------------------------------------
#include "../../Manager/Scene/SceneManager.h"
#include "../../Manager/Sound/SoundManager.h"
// �V�[��---------------------------------------------------
#include "../../ScenePlay/ScenePlay.h"
#include "../../SceneSelectDifficulty/SceneSelectDifficulty.h"
// ---------------------------------------------------
#include "../game/ScenePlay/Character/Player/Player.h"
#include "../../Utility/InputFuncTable.h"


namespace inl {

	bool PauseMenu::_isShowPauseOption = false;

	PauseMenu::PauseMenu(const Shared<Player>& player) {

		_player_ref = player;
		_scenePlay = std::make_shared<ScenePlay>();
	}


	void PauseMenu::UpdatePauseMenuCursor_ByInput() noexcept {

		if (InputFuncTable::IsButtonDownTrigger_UP() && !_isShowConfigInfo)
		{
			_menuIndex--;

			if (_menuIndex < 0)
				_menuIndex = _MENU_INDEX_COUNT - 1;
		}
		if (InputFuncTable::IsButtonDownTrigger_DOWN() && !_isShowConfigInfo)
		{
			_menuIndex++;

			if (_menuIndex >= _MENU_INDEX_COUNT)
				_menuIndex = 0;
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
		_scenePlay->TurnOffFirstStageBulletHellLists();
		_scenePlay->TurnOffSecondStageBulletHellLists();
		_scenePlay->TurnOffThirdStageBulletHellLists();

		ScenePlay::DestroyFirstStageBulletHellLists();
		ScenePlay::DestroySecondStageBulletHellLists();
		ScenePlay::DestroyThirdStageBulletHellLists();

		SoundManager::GetInstance().DestroyStageBGM(false);

		_player_ref->SetHP(_player_ref->GetMaxHP());
	}


	void PauseMenu::RenderConfigStateInfo() noexcept {

		int color = -1;

		switch (ScenePlay::GetStageID())
		{
		case 1:	color = 1;	break;
		case 2:	color = GetColor(0, 220, 0); break;
		}

		int upSide = 200;
		int leftSide = 700;
		int y_offset = 30;

		const char* descriptions[] = {

			"�e����            �@  |�}�E�X���A           �Q�[���p�b�h�Z or B|",
			 "�ړ�             �@    |�L�[�{�[�hWASD�L�[   �Q�[���p�b�h�\���L�[|",
			 "�O�i             �@    |�L�[�{�[�hSpace�L�[  �Q�[���p�b�hL1�ALB|",
			 "�X���[�ړ�        �@ |�L�[�{�[�hShift�L�[  �Q�[���p�b�h���AY|",
			 "���_����          �@ |�}�E�X               �Q�[���p�b�h�E�X�e�B�b�N|",
			 "�J�����Œ�E���� �@  |�}�E�X�E             �Q�[���p�b�hR1 or RB|",
			 "�Œ�Ώۂ̐؂�ւ��@ |�}�E�X�z�C�[��       �Q�[���p�b�h�E�X�e�B�b�N��������|",
			 "�{��               |�}�E�X����           �Q�[���p�b�h X or A|",
		};

		SetFontSize(22);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
		DrawBox(leftSide - 10, upSide - 10, leftSide + 580, upSide + 40 + (y_offset * 7), 1, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		SetFontSize(18);

		for (int i = 0; i < 7; i++) {

			DrawStringEx(leftSide, upSide + (y_offset * i), color, descriptions[i]);
		}

		DrawStringEx(leftSide + 450, upSide + (y_offset * 7), color, "Back");
	}


	void PauseMenu::RenderPauseMenuItems() noexcept {

		int baseColor = 255;

		int clr = -1;
		switch (ScenePlay::GetStageID())
		{
		case 1: 
		{

			clr = 1;
			baseColor = 88;
			break;
		}
		case 2:	
			
			clr = GetColor(0, 220, 0); 
			baseColor = GetColor(0, 255, 0);
			break;
		case 3:

			clr = GetColor(100, 100, 100);
			break;
		}

		int a = GetColor(baseColor, baseColor, baseColor),
			b = GetColor(baseColor, baseColor, baseColor),
			c = GetColor(baseColor, baseColor, baseColor),
			d = GetColor(baseColor, baseColor, baseColor);

		if (_MENU_INDEX_COUNT == 4) {
						
			if (_menuIndex == 0) a = clr;  // "�ĊJ����"
			if (_menuIndex == 1) b = clr;  // "����m�F"
			if (_menuIndex == 2) c = clr;  // "���݂̃X�e�[�W����蒼��"
			if (_menuIndex == 3) d = clr;  // "�Q�[�����I������"
		}
		else {

			if (_menuIndex == 0) b = clr;
			if (_menuIndex == 1) c = clr;
			if (_menuIndex == 2) d = clr;
		}

		int upSide = 200;
		int upSideOffset = 30;

		int leftSide = 120;
		SetFontSize(25);

		if (_player_ref->GetHP() != 0) {  // �v���C���[���S���̂�  "�ĊJ����" �@���Ȃ��Ȃ�

			_MENU_INDEX_COUNT = 4;
			DrawStringEx(leftSide, upSide, a, "�ĊJ����");
			upSide += 30;
		}
		else {
			_MENU_INDEX_COUNT = 3;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
		DrawBox(leftSide - 10, upSide - 50, leftSide + 350, upSide + 100, GetColor(100, 100, 100), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

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
}