#pragma once
#include "../Manager/Scene/SceneBase.h"

class ItemManager;

namespace inl {

	class SkyBox;
	class FreeLookCamera;
	class PauseMenu;
	class EnemyBullet;
	class BulletHellFactory;
	class StraightBullet;
	class HomingBullet;
	class Collision;
	class EnemyManager;
	class Score;
	class Player;
	class CustomException;
}


// �C���Q�[���n�_�B�����郂�W���[���������Ŏg�p�\
class ScenePlay : public SceneBase
{
public:
	
	ScenePlay() {}
	ScenePlay(const std::string selectedDifficulty, const int stage);

	~ScenePlay() override {}

	// �X�e�[�W�ړ�----------------------------------------------------------------------
	void MoveToNextStage(const int stage, const std::string difficulty);

	// �e���������i�v���C�J�n�O�ɊY���̃X�e�[�W�̃{�X�̒e���̂ݐ����j--------------------
	void CheckDoInit_FirstStageBulletHellLists();   // �X�e�[�W�P
	void CheckDoInit_SecondStageBulletHellLists();  // �X�e�[�W�Q
	void CheckDoInit_ThirdStageBulletHellLists();   // �X�e�[�W�R

	// �v���C���[�̃{�����ʂɂ��e�̖������E�L����--------------------------------------
	static void ReactivateEnemyBullets();
	static void DeactivateAllEnemyBullets();

	// ���U���g�ɔ�ԂƂ��ɍŏI�X�e�[�W�̒e�������--------------------------------------
	static void DestroyThirdStageBulletHellLists();

	// Getter ----------------------------------------------------------------------
	// �X�e�[�WID
	static const int GetStageID() noexcept { return _STAGE_ID; }

	// �I���Փx
	static const std::string GetGameDifficulty() noexcept { return _GAME_DIFFICULTY; }

	// �f���^�^�C��
	static const float GetDeltaTime() noexcept { return _deltaTime; };

private:

	// �p�[�e�B�N�� ----------------------------------------------------
	void InitWeatherParticle(const Shared<inl::CustomException>& cus);

	// Setter ----------------------------------------------------------
	void SetDeltaTime(const float deltaTime) { _deltaTime = deltaTime; };

	//�@�e�e���̃A�N�e�B�u��Ԃ� false �Ɉꊇ���Z�b�g-------------------
	void TurnOff_FirstStageBulletHellLists();
	void TurnOff_SecondStageBulletHellLists();
	void TurnOff_ThirdStageBulletHellLists();

	//�@�e���`��B�Y���X�e�[�W�̂��̂̂ݎ��s----------------------------
	void RenderFirstStageBulletHellLists();
	void RenderSecondStageBulletHellLists();
	void RenderThirdStageBulletHellLists();

	//�@�e���X�V�B�Y���X�e�[�W�̂��̂̂ݎ��s----------------------------
	void UpdateFirstStageBulletHellLists();
	void UpdateSecondStageBulletHellLists();
	void UpdateThirdStageBulletHellLists();

	//�@�e�������------------------------------------------------------
	void DestroyFirstStageBulletHellLists();
	void DestroySecondStageBulletHellLists();

	// Begin�e�L�X�g----------------------------------------------------
	void RenderBeginText() noexcept;
	void UpdateShowBeginTextTimer(const float deltaTime) noexcept;

	// �{��-------------------------------------------------------------
	void InitPlayersBombCount(const std::string selectedDifficulty) noexcept;

	// �~�j�}�b�v-------------------------------------------------------
	void RenderEnemyRadarOnMiniMap();
	void RenderStageGrindGround() noexcept;

	// �|�[�Y���j���[---------------------------------------------------
	void RenderPauseMenu();

	// �`��E�X�V�[---------------------------------------------------
	void Render() override;
	void Update(const float deltaTime) override;

public:

	static Shared<dxe::Particle>   _weatherParticle;

private:

	Shared<inl::FreeLookCamera>    _mainCamera = nullptr;

	Shared<inl::Player>            _player = nullptr;

	Shared<inl::EnemyManager>      _enemyManager = nullptr;

	Shared<inl::SkyBox>            _skyBox = nullptr;

	Shared<inl::Collision>         _collision = nullptr;

	Shared<inl::Score>             _score = nullptr;

	Shared<inl::BulletHellFactory> _bltHellFactory = nullptr;

	Shared<inl::PauseMenu>         _pauseMenu = nullptr;

	Shared<dxe::ScreenEffect>      _screenEffect = nullptr;

private:

	// Getter�Ŏg�p����l----------------------------------------------
	static int          _STAGE_ID;
	static std::string  _GAME_DIFFICULTY;
	static float        _deltaTime;

	// �|�[�Y��ʕ\�����ɒ�������w�i�̖��Óx---------------------------
	int                 _bgAlpha_whenCall_pauseMenu{ 255 };

	// ��ʍ����̃~�j�}�b�v --------------------------------------------
	int                 _miniMap_hdl{};

	// �Q�[���J�n���ɕ\������u Begin �v�e�L�X�g-------------------------
	float               _beginTextTimer{};
	bool                _isShowGameBeginText{};
};