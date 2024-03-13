#pragma once
#include "ItemBase.h"


// プレイヤー強化アイテム
class PowerUpItem : public ItemBase
{
public:

	enum class TYPE {

		Default,
		Heal,
		Attack,
		Defense,
		Speed,
		Bomb
	};

	// コンストラクタ-------------------------------------------------
	PowerUpItem() {}
	explicit PowerUpItem(const PowerUpItem::TYPE item_type);

	// 更新------------------------------------------------------------
	bool Update(Shared<PowerUpItem>& item);

public:

	PowerUpItem::TYPE type = PowerUpItem::TYPE::Default;
};