#pragma once

namespace inl {

	class Score;
}

class ScoreManager 
{
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
	void RenderTotalScore(const int color);
	int GetTotalScore() const;

private:

	Shared<inl::Score> hitScore = nullptr;
	Shared<inl::Score> scoreItemScore = nullptr;
	Shared<inl::Score> comboScore = nullptr;
	Shared<inl::Score> killBonus = nullptr;
	Shared<inl::Score> spellCardBonus = nullptr;
};