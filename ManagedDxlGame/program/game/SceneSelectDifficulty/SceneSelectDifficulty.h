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
	void UpdateSelectDifficultyCursor_ByInput() noexcept;
	//�@��Փx���ڕ`��
	void RenderDifficultiesAndAnnotation() noexcept;
	//�@�w�i�`��
	void RenderBackGround() noexcept;
	//�@��Փx����iScenePlay�֔�ԁj �@�J�n�X�e�[�W�����R�ɕύX�ł��܂�
	void DecideSelectedLevel_ByInput();

private:

	// �w�i
	int         _backGround_hdl{};
	int         _difficultyItemIndex{};
};