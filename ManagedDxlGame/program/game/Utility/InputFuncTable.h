#pragma once
#include "../DxLibEngine.h"


namespace inl {


	// �����̊Y������{�^���̑g�ݍ��킹���A���ʂ̓��͂Ƃ��Ă݂Ȃ��A�����ꂽ�� true ��Ԃ�
	class InputFuncTable
	{
	public:

		// UpperArrow or Pad Up
		static const bool IsButtonDownTrigger_UP() {      // ��i�P���j

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
				return true;
			else
				return false;
		};


		// DownArrow or Pad Down
		static const bool IsButtonDownTrigger_DOWN() {     // ���i�P���j

			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))
				return true;
			else
				return false;
		};


		// DownArrow or Pad Down
		static const bool IsButtonDown_DOWN() {            // ���i�A���j

			if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsPadDown(ePad::KEY_DOWN))
				return true;
			else
				return false;
		};


		// tnl::Input::IsPadDown(ePad::KEY_DOWN) ����͂ɕs������邽�߁@IsButtonDown_UP�͖�����


		// RightArrow or Pad Right
		static const bool IsButtonDown_RIGHT() {           // �E�i�A���j

			if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT))
				return true;
			else
				return false;
		};



		// LeftArrow or Pad Left
		static const bool IsButtonDown_LEFT() {            // ���i�A���j

			if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT))
				return true;
			else
				return false;
		};



		// EnterKey or Pad B or Pad �Z
		static const bool IsButtonTrigger_ENTER() {        // ����i�P���j

			if (tnl::Input::IsPadDownTrigger(ePad::KEY_1) || tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN))
				return true;
			else
				return false;
		};



		// BackKey or Pad A or Pad X
		static const bool IsButtonTrigger_CANCEL() {       // �L�����Z���i�P���j

			if (tnl::Input::IsPadDownTrigger(ePad::KEY_0) || tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK))
				return true;
			else
				return false;
		}
	};
}