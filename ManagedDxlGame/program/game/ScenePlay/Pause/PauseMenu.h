#pragma once

class ScenePlay;

namespace inl {

	class Player;


	class PauseMenu
	{
	public:

		PauseMenu(){}

		void Update();
		void Render();

	private:

		//�@�|�[�Y���j���[�I����́i�㉺�j
		void UpdatePauseMenuCursor_ByInput() noexcept;

		//�@���j���[���ڕ`��
		void RenderPauseMenuItems() noexcept;

		//�@��������`��i�摜�ł͂Ȃ�������̂݁j
		void RenderConfigStateInfo() noexcept;

		//�@����i���ڂ��ƂɈقȂ鏈�������s�����j
		void PickPauseMenuItemByInput();

		//  BGM�n���h���̍폜�ƃv���C���[HP�̃��Z�b�g
		void ResetGame();

	public:

		static bool      _isShowPauseOption;         // �\���̃I���I�t

	private:

		int              _MENU_INDEX_COUNT{ 4 };     // ���j���[�̍��ڐ�
		int              _menuIndex{};               // ���j���[�I���C���f�b�N�X

		bool             _isShowConfigInfo{ false }; // �R���t�B�O
	};
}