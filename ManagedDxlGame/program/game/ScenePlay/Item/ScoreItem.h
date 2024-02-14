#pragma once
#include "ItemBase.h"


class ScenePlay;

// ランダム発生、もしくは雑魚敵を倒したときに一定確率で発生するスコアアイテム
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