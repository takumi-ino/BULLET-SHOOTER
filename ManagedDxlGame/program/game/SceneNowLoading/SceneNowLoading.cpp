#include "../game/DxLibEngine.h"
#include "../game/Manager/Scene/SceneBase.h"
#include "SceneNowLoading.h"
#include "../game/ScenePlay/Character/Player/Player.h"
#include "../game/ScenePlay/Camera/FreeLookCamera.h"
#include "../game/ScenePlay/Sky/SkyBox.h"
#include "../game/ScenePlay/Wall/Wall.h"
#include "../game/ScenePlay/Collision/Collision.h"
#include "../game/ScenePlay/Bullet/Enemy/BulletHellFactory.h"
#include "../game/ScenePlay/Pause/PauseMenu.h"
#include "../game/ScenePlay/Bullet/Player/PlayerBullet.h"
// ���@�\-----------------------------------------------------------
#include "../Utility/CustomException.h"
#include "../game/Utility/InputFuncTable.h"
// �}�l�[�W���[-----------------------------------------------------------
#include "../game/Manager/Item/ItemManager.h"
#include "../game/Manager/Sound/SoundManager.h"

namespace inl {

	namespace {

		const tnl::Vector3    _START_POSITION{ 0, 100, -300 };
	}

	SceneNowLoading::SceneNowLoading(const std::string selectedDifficulty, const int stage)
		: _selectedDifficulty(selectedDifficulty), _stage(stage)
	{
		ScenePlay::CreateInstance();

		_gh = LoadGraph("graphics/Scene/nowLoading.jpg");

		ScenePlay::GetInstance()->SetStageID(stage);

		ScenePlay::GetInstance()->_initComplete = false;

		PushInitTasksToQueue();
	}

	void SceneNowLoading::PushInitTasksToQueue()
	{
		/*	[this] �́A�����_���̃L���v�`�����X�g
			���g�iSceneNowLoading�j�̃����o�ɃA�N�Z�X���邱�Ƃ��w���@*/

			// ���̊e�I�u�W�F�N�g�̐��������͕ς��Ă͂Ȃ�Ȃ��i�G���[���o��j

		_initObjects.push({ [this] { LoadWeatherParticle(); } });

		//�@����ۂȂ琶���@�ȉ����l
		if (!ScenePlay::GetInstance()->_mainCamera) {

			_initObjects.push({ [this] { LoadMainCamera(); } });
		}

		if (!ScenePlay::GetInstance()->_player) {

			_initObjects.push({ [this] { LoadPlayer(); } });

			_initObjects.push({ [this] { InitPlayerMesh(); } });

			_initObjects.push({ [this] { InitBomb(); } });

			_initObjects.push({ [this] { InitPlayersBombCount(); } });
		}

		_initObjects.push({ [this] { LoadSkyBox(); } });

		_initObjects.push({ [this] { LoadWall(); } });

		if (!ScenePlay::GetInstance()->_collision) {

			_initObjects.push({ [this] { LoadCollision(); } });
		}

		_initObjects.push({ [this] { LoadScreenEffect(); } });

		if (!ScenePlay::GetInstance()->_pauseMenu) {

			_initObjects.push({ [this] { LoadPauseMenu(); } });
		}

		_initObjects.push({ [this] { LoadEnemies(); } });

		if (!ScenePlay::GetInstance()->_bltHellFactory) {

			_initObjects.push({ [this] { LoadBulletHellFactory(); } });
		}

		_initObjects.push({ [this] { LoadItems(); } });
	}


	void SceneNowLoading::ExecuteNextTask() {

		if (!_initObjects.empty()) {

			if (!_initObjects.empty()) {

				_currentTaskObject = std::async(std::launch::async, &SceneNowLoading::ExecuteNextTask, this);
			}
		}
	}

	void SceneNowLoading::LoadWeatherParticle() {

		Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

		if (_stage == 1) {

			auto particle =
				cus->TryLoadParticleBinaryFile("particle/preset/snow.bin", "SceneNowLoading::LoadWeatherParticle()");

			ScenePlay::GetInstance()->SetWeatherParticle(particle);
		}
		else if (_stage == 2) {

			auto particle =
				cus->TryLoadParticleBinaryFile("particle/preset/customRain.bin", "SceneNowLoading::LoadWeatherParticle()");

			ScenePlay::GetInstance()->SetWeatherParticle(particle);
		}
	}


	void SceneNowLoading::LoadMainCamera() {

		_loadedCamera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	}


	void SceneNowLoading::LoadPlayer() {

		// �v���C���[
		_loadedPlayer = std::make_shared<Player>();
	}


	void SceneNowLoading::InitBomb() noexcept {

		Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

		_loadedPlayer->_bombParticle = cus->TryLoadParticleBinaryFile("particle/preset/bombEffect.bin", "SceneNowLoading::InitBomb()");
	}


	void SceneNowLoading::InitPlayerMesh() noexcept {

		Shared<inl::CustomException> cus = std::make_shared<inl::CustomException>();

		auto textureHandle = cus->TryLoadTexture("graphics/prismatic-star.png", "SceneNowLoading::InitBomb()");

		// �v���C���[����
		_loadedPlayer->_mesh = dxe::Mesh::CreateCubeMV(20, 20, 20, false);
		_loadedPlayer->_mesh->setTexture(textureHandle);
		_loadedPlayer->_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
		_loadedPlayer->_mesh->pos_ = _START_POSITION;
	}


	void SceneNowLoading::InitPlayerStraightBullet() {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		//�@���[�h�Ɏ��s�������O����----------------------------------------------------
		auto grayGraphHandle =
			cus->TryLoadTexture("graphics/colorTexture/gray.bmp", "inl::PlayerBullet::PlayerBullet()");

		auto whiteGraphHandle =
			cus->TryLoadTexture("graphics/colorTexture/white.bmp", "inl::PlayerBullet::PlayerBullet()");
		// -------------------------------------------------------------------------------------		

		std::map<PlayerBullet::COLOR, Shared<dxe::Texture>> colorMap = {

		   {PlayerBullet::COLOR::Gray,    grayGraphHandle},
		   {PlayerBullet::COLOR::White,  whiteGraphHandle},
		};

		for (int i = 0; i < 55; i++) {

			auto bullet = std::make_shared<PlayerBullet>();
			bullet->_mesh = dxe::Mesh::CreateSphereMV(10.0f);
			bullet->_mesh->setTexture(colorMap[inl::PlayerBullet::COLOR::Gray]);
			bullet->_mesh->pos_ = ScenePlay::GetInstance()->_player->_mesh->pos_;
			bullet->_id = i;
			bullet->_isActive = false;
			ScenePlay::GetInstance()->_player->_straightBullet.push_back(bullet);
		}

		ScenePlay::GetInstance()->_player->_gunportBullet.resize(5);

		for (int i = 0; i < 5; i++) {

			for (int j = 0; j < 20; j++) {

				auto gunportBullet = std::make_shared<PlayerBullet>();
				gunportBullet->_mesh = dxe::Mesh::CreateSphereMV(6.0f);
				gunportBullet->_mesh->setTexture(colorMap[inl::PlayerBullet::COLOR::Gray]);
				gunportBullet->_mesh->pos_ = ScenePlay::GetInstance()->_player->_mesh->pos_;
				gunportBullet->_id = j;
				gunportBullet->_isActive = false;
				ScenePlay::GetInstance()->_player->_gunportBullet[i].push_back(gunportBullet);
			}
		}
	}


	void SceneNowLoading::InitPlayersBombCount() noexcept
	{
		if (_selectedDifficulty == "Easy")	       _loadedPlayer->InitBombCount(4);
		else if (_selectedDifficulty == "Normal")  _loadedPlayer->InitBombCount(3);
		else if (_selectedDifficulty == "Hard")    _loadedPlayer->InitBombCount(2);
		else if (_selectedDifficulty == "Lunatic") _loadedPlayer->InitBombCount(1);
	}


	void SceneNowLoading::LoadSkyBox() {

		// �X�J�C�{�b�N�X(�V��)
		_loadedSkyBox = std::make_shared<inl::SkyBox>();
	}


	void SceneNowLoading::LoadWall() {

		// ��
		_loadedWall = std::make_shared<inl::Wall>();
	}


	void SceneNowLoading::LoadCollision() {

		// �����蔻��
		_loadedCollision = std::make_shared<inl::Collision>();
	}


	void SceneNowLoading::LoadEnemies() {

		// �G�Ɋւ��邠���鏈�����Ǘ�
		_loadedEnemyManager = std::make_shared<EnemyManager>();
	}


	void SceneNowLoading::LoadBulletHellFactory() {

		// �e�������H��
		_loadedBulletHellFactory = std::make_shared<inl::BulletHellFactory>();
	}

	void SceneNowLoading::LoadItems() {

		// �A�C�e��-----------------------------------------
		ItemManager::GetInstance().DestroyAllItems(); // ������
		ItemManager::GetInstance().CreateScoreItemPool(_selectedDifficulty, _stage);   // ����
		ItemManager::GetInstance().CreatePowerUpItemPool(_selectedDifficulty, _stage); // ����
	}


	void SceneNowLoading::LoadScreenEffect() {

		//�@�X�N���[���G�t�F�N�g
		_loadedScreenEffect = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	}


	void SceneNowLoading::LoadPauseMenu() {

		//�@�|�[�Y���j���[
		_loadedPauseMenu = std::make_shared<PauseMenu>();
	}


	void SceneNowLoading::InitObjectsInOrder()
	{
		// ���݂̃^�X�N���L�����������Ă���ꍇ
		if (_currentTaskObject.valid() && _currentTaskObject.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

			_currentTaskObject.get(); // �^�X�N�̌��ʂ��擾
			_initObjects.pop(); // �L���[����^�X�N���폜

			// ���̃^�X�N�����݂̃^�X�N�ւƍX�V
			if (!_initObjects.empty()) {

				_currentTaskObject = std::async(std::launch::async, _initObjects.front());
			}
		}
		// ���݂̃^�X�N�������ł���A���L���[�Ƀ^�X�N���c���Ă���ꍇ
		else if (!_currentTaskObject.valid() && !_initObjects.empty()) {

			// �擪�̃^�X�N���J�n
			_currentTaskObject = std::async(std::launch::async, _initObjects.front());
		}
	}


	void SceneNowLoading::AssignLoadedObjects()
	{
		if (_loadedSkyBox) {

			ScenePlay::GetInstance()->_skyBox = _loadedSkyBox;
		}

		if (_loadedPlayer) {

			ScenePlay::GetInstance()->_player = _loadedPlayer;

			InitPlayerStraightBullet();
		}

		if (_loadedCamera) {

			ScenePlay::GetInstance()->_mainCamera = _loadedCamera;
		}

		if (_loadedWall) {

			ScenePlay::GetInstance()->_wall = _loadedWall;
		}

		if (_loadedCollision) {

			ScenePlay::GetInstance()->_collision = _loadedCollision;
		}

		if (_loadedEnemyManager) {

			ScenePlay::GetInstance()->_enemyManager = _loadedEnemyManager;
		}

		if (_loadedBulletHellFactory) {

			ScenePlay::GetInstance()->_bltHellFactory = _loadedBulletHellFactory;
		}

		if (_loadedScreenEffect) {

			ScenePlay::GetInstance()->_screenEffect = _loadedScreenEffect;
		}

		if (_loadedPauseMenu) {

			ScenePlay::GetInstance()->_pauseMenu = _loadedPauseMenu;
		}
	}


	void SceneNowLoading::Render() {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);
		DrawRotaGraph(640, 350, 0.8, 0, _gh, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		if (!_initObjects.empty()) {

			SetFontSize(22);
			DrawStringEx(1050, 650, -1, "Now loading ...");
		}
		else {

			SetFontSize(35);
			DrawStringEx(435, 350, -1, "Press Enter to start the game");
		}
	}


	void SceneNowLoading::Update(const float deltaTime) {

		if (!ScenePlay::GetInstance()->_initComplete) {

			InitObjectsInOrder();

			// �G���^�[�ŃQ�[���X�^�[�g
			if (_initObjects.empty() && inl::InputFuncTable::IsButtonTrigger_ENTER()) {

				AssignLoadedObjects();

				ScenePlay::GetInstance()->_player->ResetPositionAndRotation();

				ScenePlay::GetInstance()->_initComplete = true;

				SoundManager::GetInstance().PlayStageBGM(false);       // �{�X(true)���G��(false)��
			}
		}
	}
}