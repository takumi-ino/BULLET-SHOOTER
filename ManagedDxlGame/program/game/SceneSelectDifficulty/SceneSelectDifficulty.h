#pragma once
#include "../Manager/Scene/SceneBase.h"


class SceneSelectDifficulty : public SceneBase
{
public:

	SceneSelectDifficulty();
	~SceneSelectDifficulty() {
		DeleteGraph(_backGround_hdl); 
	}

	void Render() override;
	void Update(const float deltaTime) override;

private:

	//�@��Փx�I����́i�㉺�j
	void UpdateSelectDifficultyCursor_ByInput();
	//�@��Փx���ڕ`��
	void RenderDifficultiesAndAnnotation();
	//�@�w�i�`��
	void RenderBackGround();
	//�@��Փx����iScenePlay�֔�ԁj
	void DecideSelectedLevel_ByInput();

private:

	// �w�i
	int         _backGround_hdl{};
	int         _difficultyItemIndex{};
};