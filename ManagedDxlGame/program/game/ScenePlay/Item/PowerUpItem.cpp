#include "PowerUpItem.h"
#include "../../ScenePlay/ScenePlay.h"
#include "../game/Utility/CustomException.h"


namespace inl {


	PowerUpItem::PowerUpItem(const PowerUpItem::TYPE itemType) {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		switch (itemType)
		{
		case PowerUpItem::TYPE::Heal:
		{
			float size_small = 10;
			_mesh = dxe::Mesh::CreateTorusMV(size_small, 1);

			auto textureHandle = cus->TryLoadTexture("graphics/itemTexture/prism.jpg", "inl::PowerUpItem::PowerUpItem()");
			_mesh->setTexture(textureHandle);

			_collisionSize = { size_small * 2.0f,size_small * 2.0f,size_small * 2.0f };
			type = PowerUpItem::TYPE::Heal;
			break;
		}
		case PowerUpItem::TYPE::Attack:
		{
			float size_medium = 15;
			_mesh = dxe::Mesh::CreateBarrelMV(size_medium, 5, 3);

			auto textureHandle = cus->TryLoadTexture("graphics/itemTexture/sapphire.jpg", "inl::PowerUpItem::PowerUpItem()");
			_mesh->setTexture(textureHandle);

			_collisionSize = { size_medium * 2.0f,size_medium * 2.0f,size_medium * 2.0f };
			type = PowerUpItem::TYPE::Attack;
			break;
		}
		case PowerUpItem::TYPE::Defense:
		{
			float size_large = 20;
			_mesh = dxe::Mesh::CreateConeMV(size_large, 10);

			auto textureHandle = cus->TryLoadTexture("graphics/itemTexture/gold.jpg", "inl::PowerUpItem::PowerUpItem()");
			_mesh->setTexture(textureHandle);

			_collisionSize = { size_large * 2.0f,size_large * 2.0f,size_large * 2.0f };
			type = PowerUpItem::TYPE::Defense;
			break;
		}
		case PowerUpItem::TYPE::Speed:
		{
			float size_large = 20;
			_mesh = dxe::Mesh::CreateCubeMV(size_large);

			auto textureHandle = cus->TryLoadTexture("graphics/itemTexture/gold.jpg", "inl::PowerUpItem::PowerUpItem()");
			_mesh->setTexture(textureHandle);

			_collisionSize = { size_large * 2.0f,size_large * 2.0f,size_large * 2.0f };
			type = PowerUpItem::TYPE::Speed;
			break;
		}
		case PowerUpItem::TYPE::Bomb:
		{
			float size_large = 20;
			_mesh = dxe::Mesh::CreateDiskMV(size_large);

			auto textureHandle = cus->TryLoadTexture("graphics/itemTexture/gold.jpg", "inl::PowerUpItem::PowerUpItem()");
			_mesh->setTexture(textureHandle);

			_collisionSize = { size_large * 2.0f,size_large * 2.0f,size_large * 2.0f };
			type = PowerUpItem::TYPE::Bomb;
			break;
		}
		}

		_lifeTime = 20.0f;
	}


	bool PowerUpItem::Update(Shared<PowerUpItem>& item) {

		if (item->_isActive) {
			item->_lifeTimer += ScenePlay::GetDeltaTime();

			_velocity += _gravity * ScenePlay::GetDeltaTime() * 2.0f;
			item->_mesh->pos_ += _velocity * ScenePlay::GetDeltaTime();


			if (item->_lifeTimer > _lifeTime) {
				item->_lifeTimer = 0;
				item->_isActive = false;
				return false;
			}
		}

		return true;
	}
}