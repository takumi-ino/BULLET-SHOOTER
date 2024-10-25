#pragma once
#include "../Manager/Scene/SceneBase.h"

class ItemManager;

namespace inl {

	class SkyBox;
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
	class SceneNowLoading;
}


// �C���Q�[���n�_�B�����郂�W���[���������Ŏg�p�\
class ScenePlay : public SceneBase
{
private:

	static ScenePlay* instance;

public:

	ScenePlay() {}
	~ScenePlay() override {}

	ScenePlay(const std::string difficulty, const int stage);

	static ScenePlay* GetInstance() {

		return instance;
	}

	static void CreateInstance() {

		if (instance == nullptr) {

			instance = new ScenePlay();
			instance->Initialize();
		}
	}

	static void Destroy() {

		if (instance != nullptr) {

			delete instance;
			instance = nullptr;
		}
	}

	void Initialize();

	// �X�e�[�W�ړ�----------------------------------------------------------------------
	void MoveToNextStage(const int stage, const std::string difficulty);

	// �e���������i�v���C�J�n�O�ɊY���̃X�e�[�W�̃{�X�̒e���̂ݐ����j--------------------
	void InitFirstStageBulletHellLists();   // �X�e�[�W�P
	void InitSecondStageBulletHellLists();  // �X�e�[�W�Q
	void InitThirdStageBulletHellLists();   // �X�e�[�W�R

	//�@�e�e���̃A�N�e�B�u��Ԃ� false �Ɉꊇ���Z�b�g-------------------
	static void TurnOffFirstStageBulletHellLists();
	static void TurnOffSecondStageBulletHellLists();
	static void TurnOffThirdStageBulletHellLists();

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

	// �X�e�[�WID
	static void SetStageID(const int next) noexcept { _STAGE_ID = next; }


	// �I���Փx
	static const std::string GetGameDifficulty() noexcept { return _GAME_DIFFICULTY; }

	// �f���^�^�C��
	static const float GetDeltaTime() noexcept { return _deltaTime; };

	bool GetInitializationComplete()
	{
		return ScenePlay::GetInstance()->_initComplete;
	}

	// Setter ---------------------------------------------------------------------------
	void SetInitializationComplete()
	{
		ScenePlay::GetInstance()->_initComplete = true;
	}

	void SetWeatherParticle(const Shared<dxe::Particle>& particle)
	{
		_weatherParticle = particle;
	}

	void SetMainCamera(const Shared<dxe::Camera>& camera)
	{
		_mainCamera = camera;
	}

	void SetPlayer(const Shared<inl::Player>& player)
	{
		_player = player;
	}

	void SetSkyBox(Shared<inl::SkyBox>& skyBox)
	{
		_skyBox = std::move(skyBox);
	}

	void SetWall(Shared<inl::Wall>& wall)
	{
		_wall = std::move(wall);
	}

	void SetCollision(Shared<inl::Collision>& collision)
	{
		_collision = std::move(collision);
	}

	void SetEnemyManager(const Shared<inl::EnemyManager>& enemyManager)
	{
		_enemyManager = enemyManager;
	}

	void SetBulletHellFactory(Shared<inl::BulletHellFactory>& bulletHellFactory)
	{
		_bltHellFactory = std::move(bulletHellFactory);
	}

	void SetPauseMenu(Shared<inl::PauseMenu>& pauseMenu)
	{
		_pauseMenu = std::move(pauseMenu);
	}

	void SetPauseMenu(Shared<dxe::ScreenEffect>& screenEffect)
	{
		_screenEffect = std::move(screenEffect);
	}


private:

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

	// �~�j�}�b�v-------------------------------------------------------
	void RenderEnemyRadarOnMiniMap();
	void RenderStageGrindGround() noexcept;

	// �|�[�Y���j���[---------------------------------------------------
	void RenderPauseMenu();

	// �`��E�X�V�[---------------------------------------------------
	void Render() override;
	void Update(const float deltaTime) override;

public:

	static Shared<dxe::Particle>   _weatherParticle;  // �E�F�U�[�p�[�e�B�N��

	Shared<inl::SceneNowLoading>   _nowLoading;

	Shared<dxe::Camera>			   _mainCamera;       // �J����

	Shared<inl::Player>            _player;			  // �v���C���[

	Shared<inl::EnemyManager>      _enemyManager;	  // �G�l�~�[�}�l�[�W���[

	Shared<inl::SkyBox>            _skyBox;			  // �X�J�C�{�b�N�X

	Shared<inl::Collision>         _collision;		  // �R���W�����i�����蔻��j

	Shared<inl::Score>			   _score = nullptr;  // �X�R�A

	Shared<inl::BulletHellFactory> _bltHellFactory;	  // �e��

	Shared<inl::PauseMenu>         _pauseMenu;		  // �|�[�Y���j���[

	Shared<dxe::ScreenEffect>      _screenEffect;	  // �X�N���[���G�t�F�N�g

	Shared<inl::Wall>              _wall;			  // ��

public:

	bool				_initComplete{};

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