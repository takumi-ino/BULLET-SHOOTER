#pragma once

class SoundManager
{
public:

	static SoundManager& GetInstance() {

		static SoundManager instance;
		return instance;
	}

	// ScenePlay��p�i�ʏ�G�l�~�[��ƃ{�X��ŗ���BGM���g��������ړI�����邽�߁j----------------------------
	void LoadStageBGM(const int stageId);
	void PlayStageBGM(const bool isBossBattle) noexcept;
	void DestroyStageBGM(const bool isBossBattle) noexcept;

	// ���̑��N���X��p�iBGM���Œ�Ȃ��ߒʏ�@�\�݂̂̎����j-----------------------------------------
	void LoadBGM(const std::string path);
	void PlayBGM() noexcept;
	void DestroyBGM() noexcept;

private:

	SoundManager() {}

private:

	int _BGM_hdl{};
	int _BGM_boss_hdl{};
};