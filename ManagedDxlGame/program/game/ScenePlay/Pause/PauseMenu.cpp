#include "../../DxLibEngine.h"
#include "PauseMenu.h"
// マネージャー---------------------------------------------------
#include "../../Manager/Scene/SceneManager.h"
#include "../../Manager/Sound/SoundManager.h"
// シーン---------------------------------------------------
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

			"弾発射            　  |マウス左、           ゲームパッド〇 or B|",
			 "移動             　    |キーボードWASDキー   ゲームパッド十字キー|",
			 "前進             　    |キーボードSpaceキー  ゲームパッドL1、LB|",
			 "スロー移動        　 |キーボードShiftキー  ゲームパッド△、Y|",
			 "視点操作          　 |マウス               ゲームパッド右スティック|",
			 "カメラ固定・解除 　  |マウス右             ゲームパッドR1 or RB|",
			 "固定対象の切り替え　 |マウスホイール       ゲームパッド右スティック押し込み|",
			 "ボム               |マウス中央           ゲームパッド X or A|",
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
						
			if (_menuIndex == 0) a = clr;  // "再開する"
			if (_menuIndex == 1) b = clr;  // "操作確認"
			if (_menuIndex == 2) c = clr;  // "現在のステージをやり直す"
			if (_menuIndex == 3) d = clr;  // "ゲームを終了する"
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

		if (_player_ref->GetHP() != 0) {  // プレイヤー死亡時のみ  "再開する" 　がなくなる

			_MENU_INDEX_COUNT = 4;
			DrawStringEx(leftSide, upSide, a, "再開する");
			upSide += 30;
		}
		else {
			_MENU_INDEX_COUNT = 3;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
		DrawBox(leftSide - 10, upSide - 50, leftSide + 350, upSide + 100, GetColor(100, 100, 100), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

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
}