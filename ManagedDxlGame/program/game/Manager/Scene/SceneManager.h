#pragma once
#include "SceneBase.h"


//�@�u�t�z�z�̐��`�N���X�@�V�[���̈ړ����s��
class SceneManager : public SceneBase
{
public:

	static SceneManager* GetInstance(SceneBase* startScene = nullptr);

	void ChangeScene(SceneBase* nextScene, float transTime = 0.5f);
	void Update(const float deltaTime);

private:

	explicit SceneManager(SceneBase* startScene);

	bool SeqTransIn(const float deltaTime);  // �t�F�[�h�C��
	bool SeqTransOut(const float deltaTime); // �t�F�[�h�A�E�g
	bool SeqRunScene(const float deltaTime);

	void ReleaseMem();

private:

	tnl::Sequence<SceneManager> _sequence = tnl::Sequence<SceneManager>(this, &SceneManager::SeqTransIn);

	SceneBase* _nowScene = nullptr;
	SceneBase* _nextScene = nullptr;

private:

	float     _transTime{ 0.2f };
	int       _transGraph_hdl{};
	const int _MAX_ALPHA{ 255 };
};