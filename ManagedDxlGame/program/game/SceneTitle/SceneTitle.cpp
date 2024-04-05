#include "../DxLibEngine.h"
#include "../Manager/Scene/SceneBase.h"
#include "SceneTitle.h"
#include "../SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "../Manager/Scene/SceneManager.h"
#include "../Manager/Sound/SoundManager.h"
#include "../Utility/InputFuncTable.h"
#include "../Utility/CustomException.h"


namespace {

	// �w�i�摜
	const int _BACKGROUND_POS_X{ 650 };
	const int _BACKGROUND_POS_Y{ 250 };
	const int _BACKGROUND_ALPHA{ 80 };
	const int _BACKGROUND_EFFECT_OSCILLATE_SPEED{ 30 };

	// ���S
	const int   _TITLELOGO_POS_X{ 635 };
	const int   _TITLELOGO_POS_Y{ 230 };
	const int   _TITLELOGO_EFFECT_OSCILLATE_SPEED{ 70 };
	const float _TITLELOGO_EXTEND_RATE{ 1.4f };

	// �G�t�F�N�g�̐U�����x
	const float _EFFECT_TRANS_OSCILLATE_RATE{ 1.3f };
}


	SceneTitle::SceneTitle() 
	{
		Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

		SetupAssetsData(cus);

		std::string sound = cus->TryLoadSoundPath("sound/bgm/title.mp3", "SceneTitle::SceneTitle()");

		SoundManager::GetInstance().LoadBGM(sound);
		SoundManager::GetInstance().PlayBGM();
	}


	void SceneTitle::SetupAssetsData(const Shared<inl::CustomException>& cus) 
	{
		// �摜�n���h���擾
		_backGround_hdl = cus->TryLoadGraph("graphics/Scene/titleBackGround.jpg", "SceneTitle::SetupAssetsData()");
		_titleLogo_hdl = cus->TryLoadGraph("graphics/Scene/titleLogo_star.png", "SceneTitle::SetupAssetsData()");

		// SE�n���h���擾
		_tapSE_hdl = cus->TryLoadSound("sound/se/tap.mp3", "SceneTitle::SetupAssetsData()");

		// �o�C�i���[�t�@�C�����[�h
		std::string binary = cus->TryLoadBinaryPath("screenEffect/titleSceneEffect.bin", "SceneTitle::SetupAssetsData()");

		// �X�N���[���G�t�F�N�g����
		_screenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		_screenEffect->loadStatus(binary);

		// �V���h�E�}�b�v����
		_shadowMap = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);
	}


	void SceneTitle::MakeFlushEffect_TitleLogo(const float deltaTime) noexcept
	{
		_transTime_logoLights += deltaTime;
		
		// tnl::SingleOscillationy �֐����ŐU���J�n�ʒu�� + 1.5f�̕ύX�������Ă���
		float bloom = 
			tnl::SingleOscillationy(
				tnl::eOscStart::STOK,
				0,
				_EFFECT_TRANS_OSCILLATE_RATE, 
				_transTime_logoLights
			);

		_screenEffect->setBloomThreshold(bloom * _TITLELOGO_EFFECT_OSCILLATE_SPEED);
	}


	void SceneTitle::MakeMonoTransition_BackGround(const float deltaTime) noexcept
	{
		_transTime_cb += deltaTime;
		_transTime_cr -= deltaTime;

		// �n��
		float cb = 
			tnl::SingleOscillationy(
				tnl::eOscStart::CENTER,
				0,
				_EFFECT_TRANS_OSCILLATE_RATE,
				_transTime_cb
			);

		// �Ԍn��
		float cr = 
			tnl::SingleOscillationy(
				tnl::eOscStart::CENTER, 
				0,
				_EFFECT_TRANS_OSCILLATE_RATE,
				_transTime_cr
			);

		_screenEffect->setMonoCb(cb * _BACKGROUND_EFFECT_OSCILLATE_SPEED);
		_screenEffect->setMonoCr(cr * _BACKGROUND_EFFECT_OSCILLATE_SPEED);
	}


	void SceneTitle::MoveToSceneSelectDifficulty()
	{
		if (inl::InputFuncTable::IsButtonTrigger_ENTER()) {

			PlaySoundMem(_tapSE_hdl, DX_PLAYTYPE_BACK);
			SoundManager::GetInstance().DestroyBGM();

			auto mgr = SceneManager::GetInstance();
			mgr->ChangeScene(new SceneSelectDifficulty());
		}
	}


	void SceneTitle::RenderBackGroundAndLogo() noexcept
	{
		// �w�i�摜
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _BACKGROUND_ALPHA);
		DrawRotaGraph(_BACKGROUND_POS_X, _BACKGROUND_POS_Y, 1, 0, _backGround_hdl, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ���S
		DrawRotaGraph(_TITLELOGO_POS_X, _TITLELOGO_POS_Y, _TITLELOGO_EXTEND_RATE, 0, _titleLogo_hdl, true);
	}


	void SceneTitle::Render() {

		_shadowMap->reserveBegin();
		_screenEffect->renderBegin();

		RenderBackGroundAndLogo();

		_screenEffect->renderEnd();
		_shadowMap->reserveEnd();

		// �f�o�b�O���`��
		//DrawDefaultLightGuiController();
		//_screenEffect->drawGuiController({ 0, 0 });
	}


	void SceneTitle::Update(const float deltaTime) {

		MoveToSceneSelectDifficulty();

		MakeMonoTransition_BackGround(deltaTime);

		MakeFlushEffect_TitleLogo(deltaTime);
	}