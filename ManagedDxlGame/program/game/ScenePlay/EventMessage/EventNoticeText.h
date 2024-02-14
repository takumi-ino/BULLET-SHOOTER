#pragma once
#include "../../DxLibEngine.h"

class EventNoticeText
{
public:

	EventNoticeText() {}
	EventNoticeText(const std::string text, const int color, const int fontSize);

	bool IsExpired() const;

	void Render(int index);
	void Update(const float deltaTime);

public:

	static std::deque<Shared<EventNoticeText>> _message_queue;

private:

	int          _color{};
	int          _fontSize{};
	float        _duration = 9.0f;
	std::string  _messageText{};
};