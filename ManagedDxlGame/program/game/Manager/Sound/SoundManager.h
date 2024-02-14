#pragma once

class SoundManager
{
public:

	static SoundManager& GetInstance() {

		static SoundManager instance;
		return instance;
	}

	// プレイシーンは特殊なため関数を分ける
	void LoadStageBGM(const int stage_id);
	void PlayStageBGM(const bool isBossBattle);
	void DestroyStageBGM(const bool isBossBattle);

	void LoadBGM(const std::string path);
	void PlayBGM();
	void DestroyBGM();

private:

	SoundManager(){}

private:

	int _BGM_hdl{};
	int _BGM_boss_hdl{};
};