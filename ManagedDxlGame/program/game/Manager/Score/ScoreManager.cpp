#include "ScoreManager.h"
#include "../../ScenePlay/Score/Score.h"

void ScoreManager::InitScoreInstance() {
	hitScore = std::make_shared<Score>();
	scoreItemScore = std::make_shared<Score>();
	comboScore = std::make_shared<Score>();
	killBonus = std::make_shared<Score>();
	spellCardBonus = std::make_shared<Score>();
}

void ScoreManager::AddHitBulletScore(int points) {
	hitScore->Add(points);
}

void ScoreManager::AddScoreItemScore(int points) {
	scoreItemScore->Add(points);
}

void ScoreManager::AddComboScore(int points) {
	comboScore->Add(points);
}

void ScoreManager::AddKillBonus(int points) {
	killBonus->Add(points);
}

void ScoreManager::AddSpellCardBonus(int points) {
	spellCardBonus->Add(points);
}

int ScoreManager::GetHitBulletScore() const {
	return hitScore->Get();
}

int ScoreManager::GetScoreItemScore() const {
	return  scoreItemScore->Get();
}

int ScoreManager::GetComboScore() const {
	return comboScore->Get();
}

int ScoreManager::GetKillBonus() const {
	return killBonus->Get();
}

int ScoreManager::GetSpellCardBonus() const {
	return spellCardBonus->Get();
}

int ScoreManager::GetTotalScore() const {

	return
		ScoreManager::GetHitBulletScore() +
		ScoreManager::GetScoreItemScore() +
		ScoreManager::GetComboScore() +
		ScoreManager::GetKillBonus() +
		ScoreManager::GetSpellCardBonus();
}

void ScoreManager::RenderTotalScore() {

	SetFontSize(25);

	std::string score_str = std::to_string(GetTotalScore());
	DrawFormatString(1000, 80, -1, "Score %s", score_str.c_str());
	SetFontSize(20);
}