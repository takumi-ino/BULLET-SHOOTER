#pragma once


class SceneResult : public SceneBase 
{
public:

	SceneResult() {}
	SceneResult(const std::string difficulty, const int totalScore);

	virtual ~SceneResult() {
		DeleteGraph(_backGround_hdl);
		DeleteSoundMem(_resultSE_hdl);
	}

	void Render() override;
	void Update(const float deltaTime) override;

private:
	 
	void RenderBackGround() noexcept;  // �w�i
	void RenderResult() noexcept;      // �e�L�X�g

	void MoveToSceneTitle();

private:

	int         _backGround_hdl{};	// �w�i�摜
	int         _resultSE_hdl{};   // SE
	const int   _TOTAL_SCORE{};

	bool        _isResultScene{ true };

	std::string _DIFFICULTY{};
};