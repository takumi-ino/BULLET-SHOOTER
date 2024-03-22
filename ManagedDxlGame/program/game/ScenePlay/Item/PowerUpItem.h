#pragma once
#include "ItemBase.h"

namespace inl {

	class CustomException;

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

		// コンストラクタ・デストラクタ------------------------------------
		PowerUpItem() {}
		explicit PowerUpItem(const PowerUpItem::TYPE itemType);

		~PowerUpItem() override {}


		// 初期化------------------------------------------------------------
		void InitPowerUpItem(
			const Shared<inl::CustomException>& cus,
			const float meshSize,
			const std::string texturePath,
			const std::string funcName,
			PowerUpItem::TYPE type
		);

		// 更新------------------------------------------------------------
		bool Update(Shared<PowerUpItem>& item);

	public:

		PowerUpItem::TYPE _type = PowerUpItem::TYPE::Default;
	};
}