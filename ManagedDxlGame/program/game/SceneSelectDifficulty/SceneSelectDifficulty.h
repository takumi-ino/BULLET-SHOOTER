#pragma once
#include "../Manager/Scene/SceneBase.h"


class SceneSelectDifficulty : public SceneBase
{
public:

	SceneSelectDifficulty();
	~SceneSelectDifficulty() {
		DeleteGraph(_backGround_hdl); 
	}

	void Render() override;
	void Update(const float deltaTime) override;

private:

	//　難易度選択入力（上下）
	void UpdateSelectDifficultyCursor_ByInput();
	//　難易度項目描画
	void RenderDifficultiesAndAnnotation();
	//　背景描画
	void RenderBackGround();
	//　難易度決定（ScenePlayへ飛ぶ）
	void DecideSelectedLevel_ByInput();

private:

	// 背景
	int         _backGround_hdl{};
	int         _difficultyItemIndex{};
};