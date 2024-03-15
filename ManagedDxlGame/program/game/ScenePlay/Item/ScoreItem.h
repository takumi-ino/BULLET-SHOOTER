#pragma once
#include "ItemBase.h"

namespace inl {

	// �G���G��|�����Ƃ��Ɉ��m���Ŕ�������X�R�A�A�C�e��
	class ScoreItem : public ItemBase
	{
	public:

		enum class TYPE {

			Small,
			Medium,
			Large
		};


		// �R���X�g���N�^�E�f�X�g���N�^------------------------------------
		ScoreItem() {}
		explicit ScoreItem(const ScoreItem::TYPE itemType);

		~ScoreItem() override {}

		// �X�V------------------------------------------------------------
		bool Update(Shared<ScoreItem>& item);

	public:

		std::string _sizeName{};
	};
}