#pragma once
#include "ItemBase.h"


class ScenePlay;

// �����_�������A�������͎G���G��|�����Ƃ��Ɉ��m���Ŕ�������X�R�A�A�C�e��
class ScoreItem : public ItemBase
{
public:

	enum class TYPE {

		Small,
		Medium,
		Large
	};

	ScoreItem() {}
	explicit ScoreItem(Shared<ScoreItem> item) {}
	explicit ScoreItem(const ScoreItem::TYPE item_type);

	bool Update(Shared<ScoreItem>& item);

public:

	std::string _sizeName{};
};