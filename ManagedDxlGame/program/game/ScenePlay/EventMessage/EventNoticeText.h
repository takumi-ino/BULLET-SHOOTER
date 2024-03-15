#pragma once


class EventNoticeText
{
public:

	EventNoticeText() {}
	EventNoticeText(const std::string text, const int color, const int fontSize, const int margin);

	bool IsExpired() const { return _duration <= 0.0f; };

	void Render(const int index);
	void Update(const float deltaTime);

public:

	static std::deque<Shared<EventNoticeText>> _messageQueue;

private:

	int          _color{};
	int          _fontSize{};
	int          _margin{};
	int          _startTextPos_X{};
	int          _startTextPos_Y{};

	float        _duration{};

	std::string  _messageText{};
};