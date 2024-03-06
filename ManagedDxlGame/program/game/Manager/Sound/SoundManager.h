#pragma once

class SoundManager
{
public:

	static SoundManager& GetInstance() {

		static SoundManager instance;
		return instance;
	}

	// ScenePlay��p�i�U�R��ƃ{�X��ŗ���BGM���g��������ړI�����邽�߁j----------------------------
	void LoadStageBGM(const int stage_id);
	void PlayStageBGM(const bool isBossBattle);
	void DestroyStageBGM(const bool isBossBattle);

	// ���̑��N���X��p�iBGM���Œ�Ȃ��ߒʏ�@�\�݂̂̎����j-----------------------------------------
	void LoadBGM(const std::string path);
	void PlayBGM();
	void DestroyBGM();

private:

	SoundManager() {}

private:

	int _BGM_hdl{};
	int _BGM_boss_hdl{};
};