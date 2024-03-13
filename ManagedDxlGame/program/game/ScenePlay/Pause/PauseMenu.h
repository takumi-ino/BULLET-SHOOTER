#pragma once

class ScenePlay;
class Player;

class PauseMenu
{
public:

	PauseMenu() {}
	PauseMenu(const Shared<Player>& player);

	void Update();
	void Render();

private:

	//　ポーズメニュー選択入力（上下）
	void UpdatePauseMenuCursor_ByInput();

	//　メニュー項目描画
	void RenderPauseMenuItems();

	//　操作説明描画（画像ではなく文字列のみ）
	void RenderConfigStateInfo();

	//　決定（項目ごとに異なる処理が実行される）
	void PickPauseMenuItemByInput();

	//  BGMハンドルの削除とプレイヤーHPのリセット
	void ResetGame();

private:

	Shared<ScenePlay> _scenePlay = nullptr;
	Shared<Player>    _player_ref = nullptr;

public:

	static bool _isShowPauseOption;

private:

	int         _MENU_INDEX_COUNT{ 4 };
	int         _menuIndex{};

	bool        _isShowConfigInfo{ false };
};