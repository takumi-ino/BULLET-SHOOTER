#pragma once
#include "ItemBase.h"

namespace inl {

	class CustomException;

	// �v���C���[�����A�C�e��
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

		// �R���X�g���N�^�E�f�X�g���N�^------------------------------------
		PowerUpItem() {}
		explicit PowerUpItem(const PowerUpItem::TYPE itemType);

		~PowerUpItem() override {}


		// ������------------------------------------------------------------
		void InitPowerUpItem(
			const Shared<inl::CustomException>& cus,
			const float meshSize,
			const std::string texturePath,
			const std::string funcName,
			PowerUpItem::TYPE type
		);

		// �X�V------------------------------------------------------------
		bool Update(Shared<PowerUpItem>& item);

	public:

		PowerUpItem::TYPE _type = PowerUpItem::TYPE::Default;
	};
}