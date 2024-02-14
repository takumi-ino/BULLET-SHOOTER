#pragma once

class ScenePlay;



class PauseMenu
{
public:

	PauseMenu(){}
	PauseMenu(const int stage_id, const std::string level);

	void Update();
	void Render();

private:

	void UpdatePauseMenuCursor_ByInput();

	void RenderPauseMenuItems();

	void RenderConfigStateInfo();

	void PickPauseMenuItemByInput();

private:

	Shared<ScenePlay> _scenePlay = nullptr;

public:

	static bool _isShowPauseOption;

private:

	int MENU_INDEX_COUNT = 4;

	int _menuIndex{};
	int _currentStage{};

	bool _isShowConfigInfo = false;

	std::string _selectedLevel{};
};