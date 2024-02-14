#include "EventNoticeText.h"

std::deque<Shared<EventNoticeText>> EventNoticeText::_message_queue;


EventNoticeText::EventNoticeText(const std::string text, const int color, const int fontSize) {

	_color = color;
	_fontSize = fontSize;
	_messageText = text;
}

bool EventNoticeText::IsExpired() const {

	return _duration <= 0.0f;
}

void EventNoticeText::Render(int index) {

	SetFontSize(_fontSize);
	int y = 250 - index * 30;

	DrawFormatString(90, y, _color, _messageText.c_str());
}

void EventNoticeText::Update(const float deltaTime) {

	_duration -= deltaTime;
}