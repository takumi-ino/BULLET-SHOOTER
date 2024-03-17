#include "../../DxLibEngine.h"
#include "SoundManager.h"
#include "../game/Utility/CustomException.h"


void SoundManager::LoadBGM(const std::string path) {

	_BGM_hdl = LoadSoundMem(path.c_str());
}


void SoundManager::LoadStageBGM(const int stageId) {

	Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

	if (stageId == 1) {

		_BGM_hdl = cus->TryLoadSound("sound/bgm/stage1.mp3", "inl::EnemyManager::EnemyManager()");
		_BGM_boss_hdl = cus->TryLoadSound("sound/bgm/stage1_boss.mp3", "inl::EnemyManager::EnemyManager()");
	}
	else if (stageId == 2) {

		_BGM_hdl = cus->TryLoadSound("sound/bgm/stage2.mp3", "inl::EnemyManager::EnemyManager()");
		_BGM_boss_hdl = cus->TryLoadSound("sound/bgm/stage2_boss.mp3", "inl::EnemyManager::EnemyManager()");
	}
	else if (stageId == 3) {

		_BGM_hdl = cus->TryLoadSound("sound/bgm/stage3.mp3", "inl::EnemyManager::EnemyManager()");
		_BGM_boss_hdl = cus->TryLoadSound("sound/bgm/stage3_boss.mp3", "inl::EnemyManager::EnemyManager()");
	}
}


void SoundManager::PlayStageBGM(const bool isBossBattle) noexcept {

	if (isBossBattle)
		PlaySoundMem(_BGM_boss_hdl, DX_PLAYTYPE_LOOP);
	else
		PlaySoundMem(_BGM_hdl, DX_PLAYTYPE_LOOP);
}


void SoundManager::DestroyStageBGM(const bool isBossBattle) noexcept {

	if (isBossBattle) {
		DeleteSoundMem(_BGM_hdl);
	}
	else {
		DeleteSoundMem(_BGM_hdl);
		DeleteSoundMem(_BGM_boss_hdl);
	}
}


void SoundManager::PlayBGM() noexcept {
	PlaySoundMem(_BGM_hdl, DX_PLAYTYPE_LOOP);
}


void SoundManager::DestroyBGM() noexcept {
	DeleteSoundMem(_BGM_hdl);
}