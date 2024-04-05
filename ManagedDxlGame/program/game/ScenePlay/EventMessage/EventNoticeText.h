#pragma once


namespace inl {


	//　敵を倒したときや、アイテムを取得したときなどに画面に表示するイベント通知を担当
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

		int          _color{};               // 色
		int          _fontSize{};		     // フォントサイズ
		int          _margin{};			     // 行間
		int          _startTextPos_X{};	     // X座標
		int          _startTextPos_Y{};	     // Y座標

		float        _duration{};		     // 表示時間

		std::string  _messageText{};
	};
}