#pragma once


namespace inl {


	//�@�G��|�����Ƃ���A�A�C�e�����擾�����Ƃ��Ȃǂɉ�ʂɕ\������C�x���g�ʒm��S��
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

		int          _color{};               // �F
		int          _fontSize{};		     // �t�H���g�T�C�Y
		int          _margin{};			     // �s��
		int          _startTextPos_X{};	     // X���W
		int          _startTextPos_Y{};	     // Y���W

		float        _duration{};		     // �\������

		std::string  _messageText{};
	};
}