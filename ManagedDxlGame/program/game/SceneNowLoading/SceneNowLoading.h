#pragma once
#include <queue>
#include <functional>
#include <future>
#include "../ScenePlay/ScenePlay.h"
#include "../game/Manager/Enemy/EnemyManager.h"

class SceneBase;

namespace inl {

	class Player;
	class PlayerBullet;
	class SkyBox;
	class Wall;
	class Collision;
	class EnemyManager;
	class BulletHellFactory;
	class PauseMenu;


	// SceneNowLoading�Ń��[�h����ScenePlay�ɓn��

	class SceneNowLoading : public SceneBase
	{
	public:

		SceneNowLoading(){}
		SceneNowLoading(const std::string selectedDifficulty, const int stage);

		void Update(const float deltaTime) override;
		void Render() override;

	private:

		// ���[�h�i�I�u�W�F�N�g�����j-----------------------------
		void LoadWeatherParticle();			    //�@�E�F�U�[�p�[�e�B�N��
		void LoadMainCamera();				    //�@�J����
		void LoadPlayer();					    //�@�v���C���[
		void LoadSkyBox();					    //�@�X�J�C�{�b�N�X
		void LoadWall();					    //�@��
		void LoadCollision();				    //�@�����蔻��
		void LoadEnemies();					    //�@�G
		void LoadBulletHellFactory();		    //�@�e�H��
		void LoadItems();					    //�@�A�C�e��
		void LoadScreenEffect();			    //�@�X�N���[���G�t�F�N�g
		void LoadPauseMenu();				    //�@�|�[�Y���j���[

		// ������--------------------------------------------------
		void InitBomb() noexcept;
		void InitPlayersBombCount() noexcept;
		void InitPlayerMesh() noexcept;
		void InitObjectsInOrder();
		void InitPlayerStraightBullet();
		void PushInitTasksToQueue();

		// ���̑�-------------------------------------------------
		void AssignLoadedObjects();
		void ExecuteNextTask();

	private:

		/*  ���������I�u�W�F�N�g�����Ԃ� queue�i�����o�@�j �Ɋi�[
			�擪���珇�ԂɃI�u�W�F�N�g�������n�܂�  */

		std::queue<std::function<void()>> _initObjects{};

		/*  ���݂̃I�u�W�F�N�g�B���������������玟�̃I�u�W�F�N�g�������A�������J�n
�@		  �i���݂̃I�u�W�F�N�g�̐�������������܂Ŏ��̃I�u�W�F�N�g�����͑ҋ@��ԁj�@*/

		std::future<void>				  _currentTaskObject;

		// ScenePlay �Ŏg�p����e�I�u�W�F�N�g

		Shared<Player>			  _loadedPlayer = nullptr;
		Shared<dxe::Mesh>		  _loadedPlayerBulletMesh = nullptr;
		Shared<dxe::Camera>		  _loadedCamera = nullptr;
		Shared<SkyBox>			  _loadedSkyBox = nullptr;
		Shared<Wall>			  _loadedWall = nullptr;
		Shared<Collision>		  _loadedCollision = nullptr;
		Shared< EnemyManager>	  _loadedEnemyManager = nullptr;
		Shared<BulletHellFactory> _loadedBulletHellFactory = nullptr;
		Shared<ItemManager>		  _loadedItemManager = nullptr;
		Shared<dxe::ScreenEffect> _loadedScreenEffect = nullptr;
		Shared<PauseMenu>		  _loadedPauseMenu = nullptr;

		// ScenePlay�̈ꕔ�֐����Ăяo�����߂ɃI�u�W�F�N�g��ێ�
		ScenePlay*				  _scenePlay = nullptr;

	private:

		bool _initializationComplete = false;

		std::string _selectedDifficulty{};

		int _stage{};

		int _gh{};
	};
}