#pragma once
#include "../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"


class SceneResult : public SceneBase {
public:

	SceneResult() {}
	SceneResult(const std::string difficulty, const int totalScore);

	virtual ~SceneResult() {
		DeleteGraph(_backGround_hdl);
		DeleteSoundMem(_resultSE_hdl);
	}

	void Render() override;
	void Update(const float deltaTime) override;

private:

	tnl::Sequence<SceneResult> _sequence = tnl::Sequence<SceneResult>(this, &SceneResult::SeqIdle);
	bool SeqIdle(float deltaTime);

private:

	bool        _isResultScene = true;

	// ”wŒi‰æ‘œ
	int         _backGround_hdl{};
	const int   _backGround_posX = 640;
	const int   _backGround_posY = 380;
	const int   _backGround_baseAlpha = 220;
	const float _backGround_extendRate = 0.8f;

	// Result
	const int   _resultText_posX = 530;
	const int   _resultText_posY = 150;
	int         _resultSE_hdl{};

	// “ïˆÕ“x
	const int   _difficultyText_posX = 320;
	const int   _difficultyText_posY = 250;
	std::string _difficulty{};

	// TotalScore
	const int   _totalScoreText_posX = 800;
	const int   _totalScoreText_posY = 270;
	const int   _totalScore{};
};