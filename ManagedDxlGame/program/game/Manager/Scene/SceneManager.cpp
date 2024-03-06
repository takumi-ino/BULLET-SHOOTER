#include "SceneManager.h"
#include "../../ScenePlay/ScenePlay.h"


SceneManager::SceneManager(SceneBase* startScene) : _nowScene(startScene) {

	_transGraph_hdl = LoadGraph("graphics/colorTexture/black.bmp");
}


SceneManager* SceneManager::GetInstance(SceneBase* startScene) {

	static SceneManager* instance = nullptr;

	if (instance == nullptr) {

		instance = new SceneManager(startScene);
	}

	return instance;
}


void SceneManager::ChangeScene(SceneBase* nextScene, float transTime) {

	_nextScene = nextScene;
	_transTime = transTime;
	_sequence.change(&SceneManager::SeqTransOut);
}


void SceneManager::Update(const float deltaTime) {

	if (_nowScene)
	{
		_nowScene->Render();
		_nowScene->Update(deltaTime);
	}

	_sequence.update(deltaTime);
}


bool SceneManager::SeqTransIn(const float deltaTime) {

	int alpha = _MAX_ALPHA - (_sequence.getProgressTime() / _transTime * 255.0f);

	if (alpha <= 0)
	{
		_sequence.change(&SceneManager::SeqRunScene);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, _transGraph_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _MAX_ALPHA);

	return true;
}


bool SceneManager::SeqTransOut(const float deltaTime) {

	int alpha = (_sequence.getProgressTime() / _transTime * 255.0f);

	if (alpha >= _MAX_ALPHA)
	{
		_sequence.change(&SceneManager::SeqTransIn);

		// Œ»Ý‚ÌƒV[ƒ“‚ð‰ð•ú
		_nowScene->ReleaseMem();
		_nowScene = _nextScene;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, _transGraph_hdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _MAX_ALPHA);

	return true;
}


bool SceneManager::SeqRunScene(const float deltaTime) {
	return true;
}


void SceneManager::ReleaseMem() {
	delete GetInstance();
}