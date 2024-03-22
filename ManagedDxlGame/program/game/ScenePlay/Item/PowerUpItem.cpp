#include "PowerUpItem.h"
#include "../../ScenePlay/ScenePlay.h"
#include "../game/Utility/CustomException.h"


namespace inl {


	PowerUpItem::PowerUpItem(const PowerUpItem::TYPE itemType) {

		Shared<CustomException> cus = std::make_shared<CustomException>();
		const std::string functionName = "inl::PowerUpItem::PowerUpItem()";

		float meshSize = 0.f;

		switch (itemType)
		{
		//　回復
		case PowerUpItem::TYPE::Heal:
		{
			meshSize = 10.f;
			
			//　生成
			_mesh = dxe::Mesh::CreateTorusMV(meshSize, 1.f);

			std::string texturePath = "graphics/itemTexture/prism.jpg";
			InitPowerUpItem(cus, 15.f, texturePath, functionName, PowerUpItem::TYPE::Heal);
			break;
		}
		//　攻撃
		case PowerUpItem::TYPE::Attack:
		{
			meshSize = 10.f;
			_mesh = dxe::Mesh::CreateBarrelMV(meshSize, 5.f, 3.f);

			std::string texturePath = "graphics/itemTexture/sapphire.jpg";
			InitPowerUpItem(cus, 15.f, texturePath, functionName, PowerUpItem::TYPE::Attack);
			break;
		}
		//　防御
		case PowerUpItem::TYPE::Defense:
		{
			meshSize = 15.f;
			_mesh = dxe::Mesh::CreateConeMV(meshSize, 10.f);

			std::string texturePath = "graphics/itemTexture/gold.jpg";
			InitPowerUpItem(cus, 15.f, texturePath, functionName, PowerUpItem::TYPE::Defense);
			break;
		}
		//　スピード
		case PowerUpItem::TYPE::Speed:
		{
			meshSize = 15.f;
			_mesh = dxe::Mesh::CreateCubeMV(meshSize);

			std::string texturePath = "graphics/itemTexture/gold.jpg";
			InitPowerUpItem(cus, 15.f, texturePath, functionName, PowerUpItem::TYPE::Speed);
			break;
		}
		//　ボム
		case PowerUpItem::TYPE::Bomb:
		{
			meshSize = 15.f;
			_mesh = dxe::Mesh::CreateDiskMV(meshSize);

			std::string texturePath = "graphics/itemTexture/gold.jpg";
			InitPowerUpItem(cus, 15.f, texturePath, functionName, PowerUpItem::TYPE::Bomb);
			break;
		}
		}

		_lifeTime = 20.0f;
	}


	void PowerUpItem::InitPowerUpItem(
		const Shared<inl::CustomException>& cus,
		const float meshSize,
		const std::string texturePath,
		const std::string funcName,
		PowerUpItem::TYPE type)
	{
		// テクスチャ取得
		auto textureHandle = cus->TryLoadTexture(texturePath, funcName);
		_mesh->setTexture(textureHandle);

		//　当たり判定サイズ
		_collisionSize = { meshSize * 2.0f,meshSize * 2.0f,meshSize * 2.0f };

		//　アイテムタイプ
		_type = type;
	}


	bool PowerUpItem::Update(Shared<PowerUpItem>& item) {

		if (item->_isActive) {

			// ライフタイマー計測
			item->_lifeTimer += ScenePlay::GetDeltaTime();

			//　重力を適用
			_velocity += _gravity * ScenePlay::GetDeltaTime() * 2.0f;
			item->_mesh->pos_ += _velocity * ScenePlay::GetDeltaTime();

			//　リセット
			if (item->_lifeTimer > _lifeTime) {

				item->_lifeTimer = 0.f;
				item->_isActive = false;
				return false;
			}
		}

		return true;
	}
}