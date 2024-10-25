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

		//　ポーズメニュー選択入力（上下）
		void UpdatePauseMenuCursor_ByInput() noexcept;

		//　メニュー項目描画
		void RenderPauseMenuItems() noexcept;

		//　操作説明描画（画像ではなく文字列のみ）
		void RenderConfigStateInfo() noexcept;

		//　決定（項目ごとに異なる処理が実行される）
		void PickPauseMenuItemByInput();

		//  BGMハンドルの削除とプレイヤーHPのリセット
		void ResetGame();

	public:

		static bool      _isShowPauseOption;         // 表示のオンオフ

	private:

		int              _MENU_INDEX_COUNT{ 4 };     // メニューの項目数
		int              _menuIndex{};               // メニュー選択インデックス

		bool             _isShowConfigInfo{ false }; // コンフィグ
	};
}