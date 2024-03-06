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

	// �X�R�A���Z---------------------------------------------------------------------
	void AddHitBulletScore(int points);  // �e�q�b�g
	void AddScoreItemScore(int points);  // �A�C�e���Q�b�g
	void AddComboScore(int points);      // �R���{
	void AddKillBonus(int points);       // �L���{�[�i�X
	void AddSpellCardBonus(int points);  // �X�y���J�[�h�{�[�i�X

	// ����̃X�R�A�擾--------------------------------------------------------------
	int GetHitBulletScore() const;       // �e�q�b�g
	int GetScoreItemScore() const;	     // �A�C�e���Q�b�g
	int GetComboScore() const;		     // �R���{
	int GetKillBonus() const;		     // �L���{�[�i�X
	int GetSpellCardBonus() const;	     // �X�y���J�[�h�{�[�i�X

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