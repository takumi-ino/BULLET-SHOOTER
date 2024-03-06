#pragma once

class SoundManager
{
public:

	static SoundManager& GetInstance() {

		static SoundManager instance;
		return instance;
	}

	// ScenePlay専用（ザコ戦とボス戦で流すBGMを使い分ける目的があるため）----------------------------
	void LoadStageBGM(const int stage_id);
	void PlayStageBGM(const bool isBossBattle);
	void DestroyStageBGM(const bool isBossBattle);

	// その他クラス専用（BGMが固定なため通常機能のみの実装）-----------------------------------------
	void LoadBGM(const std::string path);
	void PlayBGM();
	void DestroyBGM();

private:

	SoundManager() {}

private:

	int _BGM_hdl{};
	int _BGM_boss_hdl{};
};