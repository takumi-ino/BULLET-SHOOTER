#pragma once
#include "../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"


class SceneResult : public SceneBase {
public:

	SceneResult(){}
	SceneResult(const std::string level, const int totalScore);

	void Render() override;
	void Update(const float deltaTime) override;

private:

	tnl::Sequence<SceneResult> _sequence = tnl::Sequence<SceneResult>(this, &SceneResult::SeqIdle);
	bool SeqIdle(float deltaTime);

private:

	int _backGround_hdl{};
	int _resultSE_hdl{};

	int _totalScore{};

	std::string _difficulty{};

	bool isResultScene = true;
};