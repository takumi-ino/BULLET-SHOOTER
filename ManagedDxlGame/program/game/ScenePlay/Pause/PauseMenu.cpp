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
		// ゲーム再開
		if (_menuIndex == 0) {

			if (InputFuncTable::IsButtonTrigger_ENTER())
				_isShowPauseOption = false;
		}
		// 操作確認画面表示
		if (_menuIndex == 1) {

			if (InputFuncTable::IsButtonTrigger_ENTER())
				_isShowConfigInfo = true;
			else if (InputFuncTable::IsButtonTrigger_CANCEL())
				_isShowConfigInfo = false;
		}
		// リスタート
		if (_menuIndex == 2) {

			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				ResetGame();

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(
					new ScenePlay(ScenePlay::GetGameDifficulty(), ScenePlay::GetStageID()));
			}
		}
		// 難易度選択に戻る
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

			// 操作確認画面表示
			if (InputFuncTable::IsButtonTrigger_ENTER())
				_isShowConfigInfo = true;
			else if (InputFuncTable::IsButtonTrigger_CANCEL())
				_isShowConfigInfo = false;
		}
		if (_menuIndex == 1) {

			// リスタート
			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				ResetGame();

				auto mgr = SceneManager::GetInstance();
				mgr->ChangeScene(
					new ScenePlay(ScenePlay::GetGameDifficulty(), ScenePlay::GetStageID()));
			}
		}
		if (_menuIndex == 2) {

			// 難易度選択に戻る
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

		 "移動　      キーボードWASDキー、ゲームパッド十字キー",
		 "前進　      キーボードSpaceキー、ゲームパッドL1、LB",
		 "スロー移動  キーボードShiftキー、ゲームパッド△、Y",
		 "弾発射　    マウス左、ゲームパッド〇、B",
		 "カメラ固定　マウス右、ゲームパッドR1、RB",
		 "視点操作　  マウス、ゲームパッド左スティック",
		 "ボム　　    マウス中央、",
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
		DrawStringEx(leftSide, upSide, a, "再開する");
		upSide += 30;
	}
	else {
		_MENU_INDEX_COUNT = 3;
	}

	DrawStringEx(leftSide, upSide, b, "操作確認");
	DrawStringEx(leftSide, upSide + upSideOffset, c, "現在のステージをやり直す");
	DrawStringEx(leftSide, upSide + upSideOffset * 2, d, "ゲームを終了する");
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