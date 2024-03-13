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

	//�@�|�[�Y���j���[�I����́i�㉺�j
	void UpdatePauseMenuCursor_ByInput();

	//�@���j���[���ڕ`��
	void RenderPauseMenuItems();

	//�@��������`��i�摜�ł͂Ȃ�������̂݁j
	void RenderConfigStateInfo();

	//�@����i���ڂ��ƂɈقȂ鏈�������s�����j
	void PickPauseMenuItemByInput();

	//  BGM�n���h���̍폜�ƃv���C���[HP�̃��Z�b�g
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