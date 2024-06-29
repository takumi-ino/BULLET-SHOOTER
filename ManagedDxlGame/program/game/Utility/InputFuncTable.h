#pragma once
#include "../DxLibEngine.h"


namespace inl {


	// 複数の該当するボタンの組み合わせを、共通の入力としてみなし、押されたら true を返す
	class InputFuncTable
	{
	public:

		// UpperArrow or Pad Up
		static const bool IsButtonDownTrigger_UP() {      // 上（単発）

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
				return true;
			else
				return false;
		};


		// DownArrow or Pad Down
		static const bool IsButtonDownTrigger_DOWN() {     // 下（単発）

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))
				return true;
			else
				return false;
		};


		// DownArrow or Pad Down
		static const bool IsButtonDown_DOWN() {            // 下（連続）

			if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsPadDown(ePad::KEY_DOWN))
				return true;
			else
				return false;
		};


		// tnl::Input::IsPadDown(ePad::KEY_DOWN) 上入力に不具合があるため　IsButtonDown_UPは未実装


		// RightArrow or Pad Right
		static const bool IsButtonDown_RIGHT() {           // 右（連続）

			if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT))
				return true;
			else
				return false;
		};



		// LeftArrow or Pad Left
		static const bool IsButtonDown_LEFT() {            // 左（連続）

			if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT))
				return true;
			else
				return false;
		};



		// EnterKey or Pad B or Pad 〇
		static const bool IsButtonTrigger_ENTER() {        // 決定（単発）

			if (tnl::Input::IsPadDownTrigger(ePad::KEY_1) || tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN))
				return true;
			else
				return false;
		};



		// BackKey or Pad A or Pad X
		static const bool IsButtonTrigger_CANCEL() {       // キャンセル（単発）

			if (tnl::Input::IsPadDownTrigger(ePad::KEY_0) || tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK))
				return true;
			else
				return false;
		}
	};
}