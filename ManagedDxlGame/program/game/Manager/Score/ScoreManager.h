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

	// スコア加算---------------------------------------------------------------------
	void AddHitBulletScore(int points);  // 弾ヒット
	void AddScoreItemScore(int points);  // アイテムゲット
	void AddComboScore(int points);      // コンボ
	void AddKillBonus(int points);       // キルボーナス
	void AddSpellCardBonus(int points);  // スペルカードボーナス

	// 現状のスコア取得--------------------------------------------------------------
	int GetHitBulletScore() const;       // 弾ヒット
	int GetScoreItemScore() const;	     // アイテムゲット
	int GetComboScore() const;		     // コンボ
	int GetKillBonus() const;		     // キルボーナス
	int GetSpellCardBonus() const;	     // スペルカードボーナス

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