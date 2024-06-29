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
	class Wall;
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
	void InitFirstStageBulletHellLists();   // �X�e�[�W�P
	void InitSecondStageBulletHellLists();  // �X�e�[�W�Q
	void InitThirdStageBulletHellLists();   // �X�e�[�W�R

	//�@�e�e���̃A�N�e�B�u��Ԃ� false �Ɉꊇ���Z�b�g-------------------
	void TurnOffFirstStageBulletHellLists();
	void TurnOffSecondStageBulletHellLists();
	void TurnOffThirdStageBulletHellLists();

	// �v���C���[�̃{�����ʂɂ��e�̖������E�L����--------------------------------------
	static void ReactivateEnemyBullets();
	static void DeactivateAllEnemyBullets();

	//�@�e�������------------------------------------------------------
	static void DestroyFirstStageBulletHellLists();
	static void DestroySecondStageBulletHellLists();
	static void DestroyThirdStageBulletHellLists();

	// Getter ---------------------------------------------------------------------------
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

	//�@�e���`��B�Y���X�e�[�W�̂��̂̂ݎ��s----------------------------
	void RenderFirstStageBulletHellLists();
	void RenderSecondStageBulletHellLists();
	void RenderThirdStageBulletHellLists();

	//�@�e���X�V�B�Y���X�e�[�W�̂��̂̂ݎ��s----------------------------
	void UpdateFirstStageBulletHellLists();
	void UpdateSecondStageBulletHellLists();
	void UpdateThirdStageBulletHellLists();

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

	static Shared<dxe::Particle>   _weatherParticle;              // �E�F�U�[�p�[�e�B�N��

private:

	Shared<inl::FreeLookCamera>    _mainCamera = nullptr;         // �J����

	Shared<inl::Player>            _player = nullptr;			  // �v���C���[

	Shared<inl::EnemyManager>      _enemyManager = nullptr;		  // �G�l�~�[�}�l�[�W���[

	Shared<inl::SkyBox>            _skyBox = nullptr;			  // �X�J�C�{�b�N�X

	Shared<inl::Collision>         _collision = nullptr;		  // �R���W�����i�����蔻��j

	Shared<inl::Score>             _score = nullptr;			  // �X�R�A

	Shared<inl::BulletHellFactory> _bltHellFactory = nullptr;	  // �e��

	Shared<inl::PauseMenu>         _pauseMenu = nullptr;		  // �|�[�Y���j���[

	Shared<dxe::ScreenEffect>      _screenEffect = nullptr;		  // �X�N���[���G�t�F�N�g

	Shared<inl::Wall>              _wall = nullptr;				  // ��

private:

	// Getter�Ŏg�p����l-----------------------------------
	static int          _STAGE_ID;                                // �X�e�[�WID
	static std::string  _GAME_DIFFICULTY;						  // ��Փx������
	static float        _deltaTime;								  // �f���^�^�C��

	// �|�[�Y��ʕ\�����ɒ�������w�i�̖��Óx---------------
	int                 _bgAlpha_whenCall_pauseMenu{ 255 };		  // �w�i�̃A���t�@�l

	// ��ʍ����̃~�j�}�b�v --------------------------------
	int                 _miniMap_hdl{};							  // �~�j�}�b�v�摜�̃n���h��

	// �Q�[���J�n���ɕ\������u Begin �v�e�L�X�g------------
	float               _beginTextTimer{};						  // �^�C�}�[
	bool                _isShowGameBeginText{};					  // �\���Ǘ��t���O
};