#include "../../ScenePlay/Score/Score.h"
#include "ScoreManager.h"

void ScoreManager::InitScoreInstance() {
	hitScore = std::make_shared<inl::Score>();
	scoreItemScore = std::make_shared<inl::Score>();
	comboScore = std::make_shared<inl::Score>();
	killBonus = std::make_shared<inl::Score>();
	spellCardBonus = std::make_shared<inl::Score>();
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

void ScoreManager::RenderTotalScore(const int color) {

	SetFontSize(25);

	std::string score_str = std::to_string(GetTotalScore());

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
	DrawBox(995, 80, 1250, 110, GetColor(100, 100, 100), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawFormatString(1000, 80, color, "Score %s", score_str.c_str());
	SetFontSize(DEFAULT_FONT_SIZE);
}