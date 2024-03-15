#pragma once
#include "../Manager/Scene/SceneBase.h"

class Player;
class SkyBox;
class HeightMap;
class Collision;
class ItemManager;
class EnemyManager;
class Score;
class PauseMenu;
class FreeLookCamera;
class EnemyBullet;
class BulletHellFactory;
class StraightBullet;
class HomingBullet;


class ScenePlay : public SceneBase
{
public:

	ScenePlay() {}
	ScenePlay(const std::string selectedDifficulty, const int stage);

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
	static const int GetStageID() noexcept { return _STAGE_ID; }                       // �X�e�[�WID
	static const std::string GetGameDifficulty() noexcept { return _GAME_DIFFICULTY; } // �I���Փx
	static const float GetDeltaTime() noexcept { return _deltaTime; };                 // �f���^�^�C��

private:

	// �p�[�e�B�N�� ----------------------------------------------------
	void InitWeatherParticle();

	// Setter ----------------------------------------------------------
	void SetDeltaTime(const float deltaTime) { _deltaTime = deltaTime; };

	//�@�e�e���̃A�N�e�B�u��Ԃ� false �Ɉꊇ���Z�b�g-------------------
	void TurnOff_FirstStageBulletHellLists();
	void TurnOff_SecondStageBulletHellLists();
	void TurnOff_ThirdStageBulletHellLists();

	//�@�e���`��B���ݎg�p���̂��̂̂�----------------------------------
	void CheckDoRender_FirstStageBulletHellLists();
	void CheckDoRender_SecondStageBulletHellLists();
	void CheckDoRender_ThirdStageBulletHellLists();

	//�@�e���X�V�B���ݎg�p���̂��̂̂�----------------------------------
	void CheckDoUpdate_FirstStageBulletHellLists();
	void CheckDoUpdate_SecondStageBulletHellLists();
	void CheckDoUpdate_ThirdStageBulletHellLists();

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

	static Shared<dxe::Particle> _weatherParticle;

private:

	Shared<FreeLookCamera>    _mainCamera = nullptr;

	Shared<Player>            _player = nullptr;

	Shared<EnemyManager>      _enemyManager = nullptr;

	Shared<SkyBox>            _skyBox = nullptr;

	Shared<Collision>         _collision = nullptr;

	Shared<Score>             _score = nullptr;

	Shared<dxe::ScreenEffect> _screenEffect = nullptr;

	Shared<BulletHellFactory> _bltHellFactory = nullptr;

	Shared<PauseMenu>         _pauseMenu = nullptr;

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