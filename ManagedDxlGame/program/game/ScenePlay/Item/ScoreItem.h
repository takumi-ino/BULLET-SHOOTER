#pragma once
#include "ItemBase.h"

namespace inl {

	// 雑魚敵を倒したときに一定確率で発生するスコアアイテム
	class ScoreItem : public ItemBase
	{
	public:

		enum class TYPE {

			Small,
			Medium,
			Large
		};


		// コンストラクタ・デストラクタ------------------------------------
		ScoreItem() {}
		explicit ScoreItem(const ScoreItem::TYPE itemType);

		~ScoreItem() override {}

		// 更新------------------------------------------------------------
		bool Update(Shared<ScoreItem>& item);

	public:

		std::string _sizeName{};
	};
}