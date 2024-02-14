#pragma once
#include "../../DxLibEngine.h"

class Score;

class ScoreManager {

public:

	static ScoreManager& GetInstance() {

		static ScoreManager instance;
		return instance;
	}

	// 各オブジェクト生成
	void InitScoreInstance();

	// スコア加算
	void AddHitBulletScore(int points);
	void AddScoreItemScore(int points);
	void AddComboScore(int points);
	void AddKillBonus(int points);
	void AddSpellCardBonus(int points);

	// 現状のスコア取得
	int GetHitBulletScore() const;
	int GetScoreItemScore() const;
	int GetComboScore() const;
	int GetKillBonus() const;
	int GetSpellCardBonus() const;

	// 全てのスコアの合計
	void RenderTotalScore();
	int GetTotalScore() const;

private:

	Shared<Score> hitScore = nullptr;
	Shared<Score> scoreItemScore = nullptr;
	Shared<Score> comboScore = nullptr;
	Shared<Score> killBonus = nullptr;
	Shared<Score> spellCardBonus = nullptr;
};