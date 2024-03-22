#pragma once
#include "ItemBase.h"

namespace inl {

	class CustomException;

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

		// 初期化------------------------------------------------------------
		void InitScoreItem(
			const Shared<inl::CustomException>& cus,
			const float meshSize, 
			const std::string texturePath,
			const std::string funcName, 
			const std::string size
		);

		// 更新------------------------------------------------------------
		bool Update(Shared<ScoreItem>& item);

	public:

		std::string _sizeName{};
	};
}