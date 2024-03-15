#pragma once
#include "ItemBase.h"


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

	// �X�V------------------------------------------------------------
	bool Update(Shared<PowerUpItem>& item);

public:

	PowerUpItem::TYPE type = PowerUpItem::TYPE::Default;
};