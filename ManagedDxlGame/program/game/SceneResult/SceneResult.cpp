#include "../DxLibEngine.h"
#include "../Manager/Scene/SceneBase.h"
#include "SceneResult.h"
#include "../SceneTitle/SceneTitle.h"
#include "../ScenePlay/ScenePlay.h"
#include "../Manager/Scene/SceneManager.h"
#include "../ScenePlay/Bullet/Player/PlayerBullet.h"
#include "../Utility/InputFuncTable.h"
#include "../Utility/CustomException.h"


namespace {

	const int   _backGround_posX{ 640 };
	const int   _backGround_posY{ 380 };
	const int   _backGround_baseAlpha{ 220 };
	const float _backGround_extendRate{ 0.8f };

	// Result
	const int   _resultText_posX{ 530 };
	const int   _resultText_posY{ 150 };

	// 難易度
	const int   _difficultyText_posX{ 320 };
	const int   _difficultyText_posY{ 250 };

	// TotalScore
	const int   _totalScoreText_posX{ 800 };
	const int   _totalScoreText_posY{ 270 };
}


SceneResult::SceneResult(const std::string difficulty, const int totalScore)
	: _TOTAL_SCORE(totalScore), _DIFFICULTY(difficulty)
{
	Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

	// 画像ハンドルロード
	int graph = cus->TryLoadGraph("graphics/Scene/resultBackGround.png", "SceneResult::SceneResult()");
	// SEハンドルロード
	int sound = cus->TryLoadSound("sound/se/result.mp3", "SceneResult::SceneResult()");

	_backGround_hdl = graph;
	_resultSE_hdl = sound;

	PlaySoundMem(_resultSE_hdl, DX_PLAYTYPE_BACK);

	inl::PlayerBullet::ResetBulletPowerRate();
}


void SceneResult::RenderLiteralResult() noexcept
{
	//　Result　という文字列
	SetFontSize(70);
	DrawString(
		_resultText_posX,
		_resultText_posY,
		"Result",
		GetColor(0, 200, 100)
	);

	//　選択した難易度の文字列
	SetFontSize(40);
	DrawFormatString(
		_difficultyText_posX,
		_difficultyText_posY,
		GetColor(0, 200, 100),
		"%s", _DIFFICULTY.c_str()
	);

	//　トータルスコア
	SetFontSize(30);
	DrawFormatString(
		_totalScoreText_posX,
		_totalScoreText_posY,
		GetColor(0, 200, 100),
		"TotalScore  %d", _TOTAL_SCORE
	);
}


void SceneResult::RenderBackGround() noexcept
{
	// 背景画像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _backGround_baseAlpha);
	DrawRotaGraph(
		_backGround_posX,
		_backGround_posY,
		_backGround_extendRate,
		0,
		_backGround_hdl,
		true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void SceneResult::Render() {

	RenderBackGround();

	RenderLiteralResult();

	SetFontSize(DEFAULT_FONT_SIZE);
}


void SceneResult::MoveToSceneTitle()
{
	if (inl::InputFuncTable::IsButtonTrigger_ENTER()) {

		SceneManager::GetInstance()->ChangeScene(new SceneTitle());

		ScenePlay::Destroy();

		ScenePlay::CreateInstance();
	}
}


void SceneResult::Update(const float deltaTime)
{
	MoveToSceneTitle();
}