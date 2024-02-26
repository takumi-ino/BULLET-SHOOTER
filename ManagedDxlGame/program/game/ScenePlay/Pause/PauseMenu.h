#pragma once
#include "../game/DxLibEngine.h"

class ScenePlay;
class Player;

class PauseMenu
{
public:

	PauseMenu(){}
	PauseMenu(const Shared<Player>& player);

	void Update();
	void Render();

private:

	void UpdatePauseMenuCursor_ByInput();

	void RenderPauseMenuItems();

	void RenderConfigStateInfo();

	void PickPauseMenuItemByInput();

	void ResetGame();

private:

	Shared<ScenePlay> _scenePlay = nullptr;
	Shared<Player>    _player_ref = nullptr;

public:

	static bool _isShowPauseOption;

private:

	int         _MENU_INDEX_COUNT = 4;		        
	int         _menuIndex{};
		        
	bool        _isShowConfigInfo = false;
};