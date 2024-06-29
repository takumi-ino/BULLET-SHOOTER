#pragma once
#include "../Manager/Scene/SceneBase.h"


class SceneSelectDifficulty : public SceneBase
{
public:

	SceneSelectDifficulty();

	~SceneSelectDifficulty() override {
		DeleteGraph(_backGround_hdl); 
	}

	void Render() override;
	void Update(const float deltaTime) override;

private:

	//　難易度選択入力（上下）
	void UpdateSelectDifficultyCursor_ByInput() noexcept;

	//　難易度項目描画
	void RenderDifficultiesAndAnnotation() noexcept;

	//　背景描画
	void RenderBackGround() noexcept;

	//　難易度決定（ScenePlayへ飛ぶ） 　開始ステージを自由に変更できます
	void DecideSelectedLevel_ByInput();

private:

	int         _backGround_hdl{};
	int         _difficultyItemIndex{};

	bool        _showNowLoading{};
};