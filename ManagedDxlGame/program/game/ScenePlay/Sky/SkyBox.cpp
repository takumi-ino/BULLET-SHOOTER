#include "../../DxLibEngine.h"
#include "SkyBox.h"
#include "../game/ScenePlay/ScenePlay.h"
#include "../Camera/FreeLookCamera.h"
#include "../game/Utility/CustomException.h"


namespace inl {

	SkyBox::SkyBox() {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		auto binaryPath = cus->TryLoadBinaryPath("directional_light_parameter.bin", "inl::SkyBox::SkyBox()");

		// ディレクショナルライト
		ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));

		SetDefaultLightParameter(binaryPath);

		_mesh = dxe::Mesh::CreateCubeMV(50000, 20, 20);

		SetSkyBoxTexture_CurrentStage(cus);
	}



	void SkyBox::SetSkyBoxTexture_CurrentStage(Shared<inl::CustomException>& cus)
	{
		switch (ScenePlay::GetStageID())
		{
		case 1:
		{
			auto textureHandle =
				cus->TryLoadTexture("graphics/skybox/_skybox_c.png", "inl::Player::Player()");

			_mesh->setTexture(textureHandle);
			break;
		}
		case 2:
		{
			auto textureHandle =
				cus->TryLoadTexture("graphics/skybox/_skybox_b.png", "inl::Player::Player()");

			_mesh->setTexture(textureHandle);
			break;

		}
		case 3:
		{
			auto textureHandle =
				cus->TryLoadTexture("graphics/skybox/_skybox_a.png", "inl::Player::Player()");

			_mesh->setTexture(textureHandle);
			break;
		}
		}
	}


	void SkyBox::Render() {

		_mesh->setMtrlDiffuse({ 0, 0, 0 });
		_mesh->setMtrlEmissive({ 0.7f, 0.7f, 0.7f });

		if (ScenePlay::GetInstance()->_mainCamera)
			_mesh->render(ScenePlay::GetInstance()->_mainCamera);
		
	}


	void SkyBox::Update() {

		// 自動更新
		_mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.01f));
	}
}