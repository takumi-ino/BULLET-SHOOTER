#include "ScoreItem.h"
#include "../../ScenePlay/ScenePlay.h"
#include "../game/Utility/CustomException.h"


namespace inl {


	ScoreItem::ScoreItem(const ScoreItem::TYPE itemType) {

		Shared<CustomException> cus = std::make_shared<CustomException>();
		const std::string functionName = "inl::ScoreItem::ScoreItem()";

		switch (itemType)
		{
		case ScoreItem::TYPE::Small:
		{
			std::string texturePath = "graphics/itemTexture/prism.jpg";
			InitScoreItem(cus, 10.f, texturePath, functionName, "small");
			break;
		}
		case ScoreItem::TYPE::Medium:
		{
			std::string texturePath = "graphics/itemTexture/sapphire.jpg";
			InitScoreItem(cus, 15.f, texturePath, functionName, "medium");
			break;
		}
		case ScoreItem::TYPE::Large:
		{
			std::string texturePath = "graphics/itemTexture/gold.jpg";
			InitScoreItem(cus, 20.f, texturePath, functionName, "large");
			break;
		}
		}

		_lifeTime = 20.0f;
	}


	void ScoreItem::InitScoreItem(
		const Shared<inl::CustomException>& cus,
		const float meshSize, 
		const std::string texturePath,
		const std::string funcName,
		const std::string size) 
	{
	
		//�@���b�V������
		_mesh = dxe::Mesh::CreateSphereMV(meshSize);

		//�@�e�N�X�`���擾
		auto textureHandle = cus->TryLoadTexture(texturePath, funcName);
		_mesh->setTexture(textureHandle);

		//�@�����蔻��T�C�Y
		_collisionSize = { meshSize * 2.0f,meshSize * 2.0f,meshSize * 2.0f };

		//�@���O
		_sizeName = size;
	}


	bool ScoreItem::Update(Shared<ScoreItem>& item) {

		if (item->_isActive) {
			
			// ���C�t�^�C�}�[�v��
			item->_lifeTimer += ScenePlay::GetDeltaTime();

			//�@�d�͂�K�p
			_velocity += _gravity * ScenePlay::GetDeltaTime() * 2.0f;
			item->_mesh->pos_ += _velocity * ScenePlay::GetDeltaTime();
			
			//�@���Z�b�g
			if (item->_lifeTimer > _lifeTime) {

				item->_lifeTimer = 0.f;
				item->_isActive = false;
				return false;
			}
		}

		return true;
	}
}