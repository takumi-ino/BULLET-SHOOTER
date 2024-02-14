#pragma once
#include "../../DxLibEngine.h"

class Score;

class ScoreManager {

public:

	static ScoreManager& GetInstance() {

		static ScoreManager instance;
		return instance;
	}

	// �e�I�u�W�F�N�g����
	void InitScoreInstance();

	// �X�R�A���Z
	void AddHitBulletScore(int points);
	void AddScoreItemScore(int points);
	void AddComboScore(int points);
	void AddKillBonus(int points);
	void AddSpellCardBonus(int points);

	// ����̃X�R�A�擾
	int GetHitBulletScore() const;
	int GetScoreItemScore() const;
	int GetComboScore() const;
	int GetKillBonus() const;
	int GetSpellCardBonus() const;

	// �S�ẴX�R�A�̍��v
	void RenderTotalScore();
	int GetTotalScore() const;

private:

	Shared<Score> hitScore = nullptr;
	Shared<Score> scoreItemScore = nullptr;
	Shared<Score> comboScore = nullptr;
	Shared<Score> killBonus = nullptr;
	Shared<Score> spellCardBonus = nullptr;
};