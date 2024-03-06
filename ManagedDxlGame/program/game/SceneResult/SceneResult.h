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

	bool        _isResultScene{ true };

	// ”wŒi‰æ‘œ
	int         _backGround_hdl{};
	int         _resultSE_hdl{};
	std::string _difficulty{};

	const int   _totalScore{};
};