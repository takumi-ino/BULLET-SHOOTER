#include "../../DxLibEngine.h"
#include "EventNoticeText.h"


namespace inl {


	std::deque<Shared<EventNoticeText>> EventNoticeText::_messageQueue;

	EventNoticeText::EventNoticeText(const std::string text, const int color, const int fontSize, const int margin)
		: _startTextPos_X(60), _startTextPos_Y(400), _duration(10.0f) {

		_color = color;
		_fontSize = fontSize;
		_messageText = text;
		_margin = margin;
	}


	void EventNoticeText::Render(const int index) {

		SetFontSize(_fontSize);
		int y = _startTextPos_Y - index * _margin;

		DrawFormatString(_startTextPos_X, y, _color, _messageText.c_str());
	}


	void EventNoticeText::Update(const float deltaTime) {

		_duration -= deltaTime;
	}
}