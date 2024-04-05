#pragma once

class SoundManager
{
public:

	static SoundManager& GetInstance() {

		static SoundManager instance;
		return instance;
	}

	// ScenePlay専用（通常エネミー戦とボス戦で流すBGMを使い分ける目的があるため）----------------------------
	void LoadStageBGM(const int stageId);
	void PlayStageBGM(const bool isBossBattle) noexcept;
	void DestroyStageBGM(const bool isBossBattle) noexcept;

	// その他クラス専用（BGMが固定なため通常機能のみの実装）-----------------------------------------
	void LoadBGM(const std::string path);
	void PlayBGM() noexcept;
	void DestroyBGM() noexcept;

private:

	SoundManager() {}

private:

	int _BGM_hdl{};
	int _BGM_boss_hdl{};
};