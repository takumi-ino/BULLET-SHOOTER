#pragma once
#include "DxLibEngine.h"

class InputFuncTable 
{
public:

	// UpperArrow or Pad Up
	static const bool IsButtonDownTrigger_UP() {

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
			return true;
		else
			return false;
	};



	// DownArrow or Pad Down
	static const bool IsButtonDownTrigger_DOWN() {

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))
			return true;
		else
			return false;
	};


	// DownArrow or Pad Down
	static const bool IsButtonDown_DOWN() {

		if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsPadDown(ePad::KEY_DOWN))
			return true;
		else
			return false;
	};


	// tnl::Input::IsPadDown(ePad::KEY_DOWN) è„ì¸óÕÇ…ïsãÔçáÇ™Ç†ÇÈÇΩÇﬂÅ@IsButtonDown_UPÇÕñ¢é¿ëï


	// RightArrow or Pad Right
	static const bool IsButtonDown_RIGHT() {

		if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT))
			return true;
		else
			return false;
	};



	// LeftArrow or Pad Left
	static const bool IsButtonDown_LEFT() {

		if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT))
			return true;
		else
			return false;
	};



	// EnterKey or Pad B or Pad ÅZ
	static const bool IsButtonTrigger_ENTER() {

		if (tnl::Input::IsPadDownTrigger(ePad::KEY_1) || tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN))
			return true;
		else 
			return false;
	};



	// BackKey or Pad A or Pad X
	static const bool IsButtonTrigger_CANCEL() {

		if (tnl::Input::IsPadDownTrigger(ePad::KEY_2) || tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK))
			return true;
		else
			return false;
	}
};