#include <random>
#include "./../../DxLibEngine.h"
#include "../../../library/tnl_sequence.h"
// �}�l�[�W���[----------------------------------------------------
#include "EnemyManager.h"
#include "../Item/ItemManager.h"
#include "../Sound/SoundManager.h"
#include "../../Manager/Score/ScoreManager.h"
#include "../../Manager/Scene/SceneManager.h"
// �e--------------------------------------------------------------
#include "../../ScenePlay/Bullet/Player/PlayerBullet.h"
#include "../../ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../../ScenePlay/Bullet/Enemy/HomingBullet.h"
#include "../../ScenePlay/Bullet/Enemy/BulletHell.h"
#include "../../ScenePlay/Bullet/Enemy/EnemyBullet.h"
// �V�[��----------------------------------------------------------
#include "../../ScenePlay/ScenePlay.h"
#include "../../SceneResult/SceneResult.h"
// �G--------------------------------------------------------------
#include "../../ScenePlay/Character/Enemy/EnemyZako/EnemyZakoBox.h"
#include "../../ScenePlay/Character/Enemy/EnemyZako/EnemyZakoDome.h"
#include "../../ScenePlay/Character/Enemy/EnemyZako/EnemyZakoCylinder.h"
#include "../../ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../../ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "../../ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"
#include "../../ScenePlay/Character/Enemy/EnemyBossBase.h"
#include "../../ScenePlay/Character/Enemy/EnemyZakoBase.h"
// ���̑�-----------------------------------------------------------
#include "../../ScenePlay/EventMessage/EventNoticeText.h"
#include"../../ScenePlay/Item/ItemBase.h"
#include "../../Utility/InputFuncTable.h"
#include "../game/Utility/CustomException.h"
#include "../../ScenePlay/Character/Player/Player.h"
#include "../../ScenePlay/Collision/Collision.h"


namespace inl {


	// ����������-----------------------------------------------------------------------------------------------
	EnemyManager::EnemyManager(
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision)
		: _player_ref(player), _mainCamera_ref(camera), _collision_ref(collision)
	{

		Shared<CustomException> cus = std::make_shared<CustomException>();
		_alertSE_hdl = cus->TryLoadSound("sound/se/bossAppears.mp3", "inl::EnemyManager::EnemyManager()");

		//�@CSV�t�@�C������G�f�[�^������Ă���
		LoadEnemyDataFromCsv();

		//�@�U�R�G�̂P�x�ɐ����\�ȍő吔���Փx���Ƃɐݒ�
		SetMaxEnemySpawnCount();

		//�@�G�f�[�^�擾
		InitEnemyZakoInfo();
		InitEnemyBossInfo();
		SetSpawnEnemyBoss();

		_isInitializedBossInfo = false;

		SoundManager::GetInstance().LoadStageBGM(ScenePlay::GetStageID());
		SoundManager::GetInstance().PlayStageBGM(false);       // �{�X(true)���G��(false)��

		_itemManager = std::make_shared<ItemManager>(_player_ref, _collision_ref);
		this->AttachItemManagerInstance(_itemManager);
	}


	void EnemyManager::SetMaxEnemySpawnCount() noexcept
	{
		if (ScenePlay::GetGameDifficulty() == "Easy")         _maxEnemySpawnCount_PerInterval = 1;
		else if (ScenePlay::GetGameDifficulty() == "Normal")  _maxEnemySpawnCount_PerInterval = 2;
		else if (ScenePlay::GetGameDifficulty() == "Hard")    _maxEnemySpawnCount_PerInterval = 3;
		else if (ScenePlay::GetGameDifficulty() == "Lunatic") _maxEnemySpawnCount_PerInterval = 4;
	}



	void EnemyManager::LoadEnemyDataFromCsv()
	{
		// CSV����G�f�[�^�̃��[�h
		if (!_enemyZakoData_map.empty() && !_enemyBossData_map.empty())
			return;

		using namespace std;

		Shared<CustomException> cus = std::make_shared<CustomException>();
		auto zakoCsv = cus->TryLoadCsvFile("csv/EnemyZakoInfos.csv", "inl::EnemyManager::LoadEnemyDataFromCsv()");
		auto bossCsv = cus->TryLoadCsvFile("csv/EnemyBossInfos.csv", "inl::EnemyManager::LoadEnemyDataFromCsv()");

		_csvLoader = make_shared<CsvLoader>();
		_enemyZakoData_map = _csvLoader->LoadEnemyZakoInfos(zakoCsv);
		_enemyBossData_map = _csvLoader->LoadEnemyBossInfos(bossCsv);
	}



	void EnemyManager::InitEnemyZakoInfo() {

		// ���[�h�����G�l�~�[�f�[�^�����[�v
		for (auto enemy = _enemyZakoData_map.begin(); enemy != _enemyZakoData_map.end(); enemy++) {

			switch ((*enemy).first) // id
			{
			case 0: // EnemyZakoBox�f�[�^�擾

				//�@�ړ����x
				_sEnemy_zakoBox_info._enemyMoveSpeed = (*enemy).second._enemyMoveSpeed;
				//�@�S�Ă̓G�̐�
				_sEnemy_zakoBox_info._maxTotalEnemy_spawnCount = (*enemy).second._maxTotalEnemy_spawnCount;
				//�@���O
				_sEnemy_zakoBox_info._name = (*enemy).second._name;
				//�@�傫��
				_sEnemy_zakoBox_info._scale = (*enemy).second._scale;
				//�@�X�e�[�WID
				_sEnemy_zakoBox_info._stageID = (*enemy).second._stageID;
				//�@�e�̃����[�h�Ԋu����
				_sEnemy_zakoBox_info._bulletReloadTimeInterval = (*enemy).second._bulletReloadTimeInterval;
				//�@�e�̌��Ԋu
				_sEnemy_zakoBox_info._bulletFireInterval = (*enemy).second._bulletFireInterval;
				//�@�e�̑���
				_sEnemy_zakoBox_info._bulletMoveSpeed = (*enemy).second._bulletMoveSpeed;
				//�@�G�̎c��
				_remainingEnemyZako_spawnCount = _sEnemy_zakoBox_info._maxTotalEnemy_spawnCount;
				break;
			case 1: // EnemyZakoDome�f�[�^�擾
				_sEnemy_zakoDome_info._enemyMoveSpeed = (*enemy).second._enemyMoveSpeed;
				_sEnemy_zakoDome_info._maxTotalEnemy_spawnCount = (*enemy).second._maxTotalEnemy_spawnCount;
				_sEnemy_zakoDome_info._name = (*enemy).second._name;
				_sEnemy_zakoDome_info._scale = (*enemy).second._scale;
				_sEnemy_zakoDome_info._stageID = (*enemy).second._stageID;
				_sEnemy_zakoDome_info._bulletReloadTimeInterval = (*enemy).second._bulletReloadTimeInterval;
				_sEnemy_zakoDome_info._bulletFireInterval = (*enemy).second._bulletFireInterval;
				_sEnemy_zakoDome_info._bulletMoveSpeed = (*enemy).second._bulletMoveSpeed;

				_remainingEnemyZako_spawnCount = _sEnemy_zakoDome_info._maxTotalEnemy_spawnCount;
				break;
			case 2: // EnemyZakoCylinder�f�[�^�擾
				_sEnemy_zakoCylinder_info._enemyMoveSpeed = (*enemy).second._enemyMoveSpeed;
				_sEnemy_zakoCylinder_info._maxTotalEnemy_spawnCount = (*enemy).second._maxTotalEnemy_spawnCount;
				_sEnemy_zakoCylinder_info._name = (*enemy).second._name;
				_sEnemy_zakoCylinder_info._scale = (*enemy).second._scale;
				_sEnemy_zakoCylinder_info._stageID = (*enemy).second._stageID;
				_sEnemy_zakoCylinder_info._bulletReloadTimeInterval = (*enemy).second._bulletReloadTimeInterval;
				_sEnemy_zakoCylinder_info._bulletFireInterval = (*enemy).second._bulletFireInterval;
				_sEnemy_zakoCylinder_info._bulletMoveSpeed = (*enemy).second._bulletMoveSpeed;

				_remainingEnemyZako_spawnCount = _sEnemy_zakoCylinder_info._maxTotalEnemy_spawnCount;
				break;
			}
		}
	}


	void EnemyManager::InitEnemyBossInfo() {

		for (auto boss = _enemyBossData_map.begin(); boss != _enemyBossData_map.end(); boss++) {

			switch ((*boss).first) // id
			{
			case 0: // �p�`�����[ �f�[�^�擾
				//�@�ړ����x
				_sBoss_PatchouliKnowledge_info._enemyMoveSpeed = (*boss).second._enemyMoveSpeed;
				//�@���O
				_sBoss_PatchouliKnowledge_info._name = (*boss).second._name;
				//�@�傫��
				_sBoss_PatchouliKnowledge_info._scale = (*boss).second._scale;
				//�@�X�e�[�WID
				_sBoss_PatchouliKnowledge_info._stageID = (*boss).second._stageID;
				break;
			case 1: // �`���m �f�[�^�擾
				_sBoss_Cirno_info._enemyMoveSpeed = (*boss).second._enemyMoveSpeed;
				_sBoss_Cirno_info._name = (*boss).second._name;
				_sBoss_Cirno_info._scale = (*boss).second._scale;
				_sBoss_Cirno_info._stageID = (*boss).second._stageID;
				break;
			case 2: // �k��z�K�q �f�[�^�擾
				_sBoss_MoriyaSuwako_info._enemyMoveSpeed = (*boss).second._enemyMoveSpeed;
				_sBoss_MoriyaSuwako_info._name = (*boss).second._name;
				_sBoss_MoriyaSuwako_info._scale = (*boss).second._scale;
				_sBoss_MoriyaSuwako_info._stageID = (*boss).second._stageID;
				break;
			}
		}
	}


	void EnemyManager::SetSpawnEnemyBoss() {

		// �X�e�[�W�P
		if (ScenePlay::GetStageID() == _sBoss_PatchouliKnowledge_info._stageID) {

			auto boss_patchouli = // �C���X�^���X����
				std::make_shared<EnemyBoss_PatchouliKnowledge>(
					_enemyBossData_map[0], _player_ref, _mainCamera_ref, _collision_ref
				);

			//�@���S�t���O
			boss_patchouli->_isDead = false;
			//�@�ړ����x�@
			boss_patchouli->_enemyMoveSpeed = _sBoss_PatchouliKnowledge_info._enemyMoveSpeed;
			//�@���O
			boss_patchouli->_name = _sBoss_PatchouliKnowledge_info._name;
			//�@�傫��
			boss_patchouli->_scale = _sBoss_PatchouliKnowledge_info._scale;

			_enemyBossList.push_back(boss_patchouli);
		}
		// �X�e�[�W�Q
		else if (ScenePlay::GetStageID() == _sBoss_Cirno_info._stageID) {

			auto boss_cirno =
				std::make_shared<EnemyBoss_Cirno>(
					_enemyBossData_map[1], _player_ref, _mainCamera_ref, _collision_ref
				);

			boss_cirno->_isDead = false;
			boss_cirno->_enemyMoveSpeed = _sBoss_Cirno_info._enemyMoveSpeed;
			boss_cirno->_name = _sBoss_Cirno_info._name;
			boss_cirno->_scale = _sBoss_Cirno_info._scale;

			_enemyBossList.push_back(boss_cirno);
		}
		// �X�e�[�W�R
		else if (ScenePlay::GetStageID() == _sBoss_MoriyaSuwako_info._stageID) {

			auto boss_suwako =
				std::make_shared<EnemyBoss_MoriyaSuwako>(
					_enemyBossData_map[2], _player_ref, _mainCamera_ref, _collision_ref
				);

			boss_suwako->_isDead = false;
			boss_suwako->_enemyMoveSpeed = _sBoss_MoriyaSuwako_info._enemyMoveSpeed;
			boss_suwako->_name = _sBoss_MoriyaSuwako_info._name;
			boss_suwako->_scale = _sBoss_MoriyaSuwako_info._scale;

			_enemyBossList.push_back(boss_suwako);
		}
	}


	// -------------------------------------------------------------------------------------------------------------
	void EnemyManager::CheckDoSpawnEnemy() {

		switch (ScenePlay::GetStageID())
		{
		case 1:  // �X�e�[�W�P
		{
			// �c��̓G�X�|�[����
			if (_remainingEnemyZako_spawnCount > 0) {

				// �c��̓G�X�|�[���񐔂ƍő哯�����������r���A���Ȃ������g�p
				int enemiesToSpawnNow = min(_remainingEnemyZako_spawnCount, _maxEnemySpawnCount_PerInterval);

				for (int i = 0; i < enemiesToSpawnNow; i++) {

					auto enemy_box = std::make_shared<EnemyZakoBox>(
						_enemyZakoData_map[0], 
						_player_ref,
						_mainCamera_ref,
						_collision_ref
					);

					enemy_box->_isDead = false;
					enemy_box->SetMeshInfo();
					enemy_box->InitBulletFactoryInstance();

					_enemyZakoList.push_back(enemy_box);	// EnemyZakoBox ����
				}

				// �X�|�[�������������c��̐����\�񐔂�������Z
				_remainingEnemyZako_spawnCount -= enemiesToSpawnNow;
				_zakoEnemyTotalLeftCount = _remainingEnemyZako_spawnCount + enemiesToSpawnNow;
			}
			else
				// �U�R�G��S�ē|������
				SummonBoss();
			break;
		}
		case 2:	 // �X�e�[�W2
		{
			if (_remainingEnemyZako_spawnCount > 0) {

				int enemiesToSpawnNow = min(_remainingEnemyZako_spawnCount, _maxEnemySpawnCount_PerInterval);

				for (int i = 0; i < enemiesToSpawnNow; i++) {

					auto enemy_dome = std::make_shared<EnemyZakoDome>(
						_enemyZakoData_map[1],
						_player_ref,
						_mainCamera_ref,
						_collision_ref
					);

					enemy_dome->_isDead = false;
					enemy_dome->SetMeshInfo();
					enemy_dome->InitBulletFactoryInstance();

					_enemyZakoList.push_back(enemy_dome);	// EnemyZakoDome ����
				}

				_remainingEnemyZako_spawnCount -= enemiesToSpawnNow;
				_zakoEnemyTotalLeftCount = _remainingEnemyZako_spawnCount + enemiesToSpawnNow;
			}
			else
				SummonBoss();
			break;
		}
		case 3:	 // �X�e�[�W3
		{
			if (_remainingEnemyZako_spawnCount > 0) {

				int enemiesToSpawnNow = min(_remainingEnemyZako_spawnCount, _maxEnemySpawnCount_PerInterval);

				for (int i = 0; i < enemiesToSpawnNow; i++) {

					auto enemy_cylinder = std::make_shared<EnemyZakoCylinder>(
						_enemyZakoData_map[2], 
						_player_ref,
						_mainCamera_ref,
						_collision_ref
					);

					enemy_cylinder->_isDead = false;
					enemy_cylinder->SetMeshInfo();
					enemy_cylinder->InitBulletFactoryInstance();

					_enemyZakoList.push_back(enemy_cylinder); // EnemyZakoCylinder ���� 
				}

				_remainingEnemyZako_spawnCount -= enemiesToSpawnNow;
				_zakoEnemyTotalLeftCount = _remainingEnemyZako_spawnCount + enemiesToSpawnNow;
			}
			else
				SummonBoss();
			break;
		}
		}
	}


	void EnemyManager::SummonBoss()
	{
		if (!_isInitializedBossInfo) {

			auto boss = _enemyBossList.begin();
			(*boss)->SetMeshInfo();
			(*boss)->InitBulletHellInstance();

			PlaySoundMem(_alertSE_hdl, DX_PLAYTYPE_BACK, TRUE);

			// �����̓{�X�o�g�����ۂ����w��Btrue �Ń{�X�̏����̂ݎ��s
			SoundManager::GetInstance().DestroyStageBGM(true);
			SoundManager::GetInstance().PlayStageBGM(true);

			_isShowBossAppearanceText = true;

			_isInitializedBossInfo = true;
		}
		else
			_isSummonBoss = true;
	}


	//�����蔻��----------------------------------------------------------------------------------------------------------
	void EnemyManager::EnemyZakoCollisionPairLists()
	{
		for (auto& it : _enemyZakoList) {

			if (it->_mesh != nullptr) {

				// �v���C���[��Zako�G�l�~�[�e��
				_collision_ref->CheckCollision_PlayerAndEnemyZako(
					_player_ref, it, _player_ref->GetPos(), it->_mesh->pos_
				);
			}
			// �v���C���[�̒e��Zako�G�l�~�[�e��
			for (Shared<PlayerBullet> pb : _player_ref->_straightBullets_player) {

				if (_collision_ref->CheckCollision_PlayerBulletAndEnemyZako(pb, it)) {

					pb->_isActive = false;

					if (it->DecreaseHP(_player_ref->GetAT(), _mainCamera_ref)) {

						// �X�R�A���Z
						ScoreManager::GetInstance().AddHitBulletScore(100);
						// �v���C���[����
						EnemyZakoBase::_isNoticedPlayer = true;
						//�@�v���C���[�̒e�̈З̓|�C���g�㏸
						PlayerBullet::_bulletPowerRate += 0.01f;
						// �v���C���[�ɋC�t���Ă���̎��Ԃɏ����l��ݒ�
						it->_timeCountFrom_noticedPlayer = 0.0f;
					}
					else {

						EventNotify_OnEnemyKilled(it->_name); // ���O��ʒm
					}
				}
			}

			// Zako�G�l�~�[���m�̓����蔻��
			for (auto& it2 : _enemyZakoList) {

				if (it == it2) 
					continue;

				_collision_ref->CheckCollision_EnemyAndEnemy(
					it,
					it2,
					it->_mesh->pos_,
					it2->_mesh->pos_
				);
			}
		}
	}


	void EnemyManager::EnemyBossCollisionPairLists()
	{
		for (auto& it : _enemyBossList) {

			if (it->_mesh != nullptr) {

				// �v���C���[��Boss�G�l�~�[�e��
				_collision_ref->CheckCollision_PlayerAndEnemyBoss(
					_player_ref,
					it, 
					_player_ref->GetPos(),
					it->_mesh->pos_
				);
			}

			// �v���C���[�̒e��Boss�G�l�~�[�e��
			for (Shared<inl::PlayerBullet> pb : _player_ref->_straightBullets_player) {

				if (_collision_ref->CheckCollision_PlayerBulletAndEnemyBoss(pb, it)) {

					pb->_isActive = false;
					it->DecreaseBossHP(_player_ref->GetAT());
					ScoreManager::GetInstance().AddHitBulletScore(100);

					inl::PlayerBullet::_bulletPowerRate += 0.01f;
				}
				else {
					static bool isNotify = false;

					if (it->_isDead && !isNotify) {

						EventNotify_OnEnemyKilled(it->_name); // ���O��ʒm
						isNotify = true;
					}
				}
			}
		}
	}

	//�@�{�X�̈ʒu���W�̒ʒm------------------------------------------------------------------------------------------
	void EnemyManager::AttachItemManagerInstance(const Shared<ItemManager>& observer) {
		_observerItems.push_back(observer);
	}


	void EnemyManager::NotifyEnemyPosition_ToItemManager() {

		for (const auto& observer : _observerItems) {
			observer->SpawnItemsOnEnemyDeath(_enemyZako_position, _isEnemyZako_dead);
		}
	}


	void EnemyManager::SendEnemyPosition(const tnl::Vector3& newPosition, const bool isEnemyDead) {

		_enemyZako_position = newPosition;
		_isEnemyZako_dead = isEnemyDead;
		NotifyEnemyPosition_ToItemManager();
	}


	//�@-------------------------------------------------------------------------------------------------------------------
	const std::vector<tnl::Vector3>& EnemyManager::GetEnemyZakoPosition() {

		_enemyPosList.clear();

		for (const auto& zako : _enemyZakoList) {

			try {

				// �U�R�̈ʒu���擾
				_enemyPosList.push_back(zako->_mesh->pos_);

				//�@�擾�����l���󂾂������O
				if (_enemyPosList.empty()) {

					throw CustomException("inl::EnemyManager::GetEnemyZakoPosition()");
				}
			}
			catch (const CustomException& e) {
				std::cerr << e.what() << std::endl << e.getFunctionName();
			}
		}

		return _enemyPosList;
	}


	const tnl::Vector3& EnemyManager::GetEnemyBossPosition() {

		for (const auto& it_boss : _enemyBossList) {

			if (!it_boss->_mesh) {

				try {
					// �{�X�̈ʒu���擾
					_enemyBossPos = it_boss->_mesh->pos_;

					//�@�擾�����l���O���������O
					if (_enemyBossPos.length() == 0) {

						throw CustomException("inl::EnemyManager::GetEnemyBossPosition()");
					}
				}
				catch (const CustomException& e) {
					std::cerr << e.what() << std::endl << e.getFunctionName();
				}
			}
		}

		return _enemyBossPos;
	}


	bool EnemyManager::IsKilledStageBoss() {

		if (_enemyZakoList.empty() && _enemyBossList.empty()) {
			return true;
		}
		return false;
	}


	void EnemyManager::ShowBossAppearanceText() noexcept {

		SetFontSize(80);
		DrawFormatString(390, 200, GetColor(255, 0, 0), "Boss Appears");
		SetFontSize(DEFAULT_FONT_SIZE);
	}


	void EnemyManager::UpdateBossAppearanceTextTimer(const float deltaTime) noexcept
	{
		if (_isShowBossAppearanceText) {

			_bossAppearanceTextTimer += deltaTime;

			ShowBossAppearanceText();

			//�@���Ԍo�߂Ńe�L�X�g�폜
			if (_bossAppearanceTextTimer > 3.f) {

				_bossAppearanceTextTimer = 0.0f;
				_isShowBossAppearanceText = false;
			}
		}
	}

	// �G�E���C�x���g�ʒm--------------------------------------------------------------------------------------------
	void EnemyManager::EventNotify_OnEnemyKilled(const std::string enemy_name) {

		std::string event = enemy_name + "�����j ";

		Shared<inl::EventNoticeText> msg =
			std::make_shared<inl::EventNoticeText>(
				event,
				GetColor(255, 0, 0),  // �F
				16,					  // �����̑傫��
				30					  // �����̊Ԋu
			);

		inl::EventNoticeText::_messageQueue.push_back(msg);
	}


	void EnemyManager::RenderEventHitText() const
	{
		int index = 0;

		for (const auto msg : inl::EventNoticeText::_messageQueue) {
			msg->Render(index);
			index++;
		}
	}


	void EnemyManager::UpdateEventHitText(const float deltaTime)
	{
		// �C�x���g�ʒm
		for (const auto msg : inl::EventNoticeText::_messageQueue) {
			msg->Update(deltaTime);
		}

		// �\�����Ԃ��؂ꂽ���b�Z�[�W���폜
		auto it = inl::EventNoticeText::_messageQueue.begin();

		while (it != inl::EventNoticeText::_messageQueue.end()) {

			if ((*it)->IsExpired()) {

				it = inl::EventNoticeText::_messageQueue.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// �X�e�[�W�ړ�-------------------------------------------------------------------------------------------------
	bool EnemyManager::SeqMoveToNextStage(const float deltaTime) {

		SeqMoveToResult(deltaTime);

		// �{�X��|�������A�܂��S�ẴX�e�[�W�̓G��|���Ă��Ȃ����
		if (IsKilledStageBoss() && !_isDefeatedAllStageEnemy) {

			SetFontSize(80);
			DrawString(450, 200, "Stage Clear", GetColor(0, 255, 0));
			SetFontSize(40);
			DrawString(480, 450, "Move to Next Stage!", GetColor(0, 255, 0));
			SetFontSize(DEFAULT_FONT_SIZE);

			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				SoundManager::GetInstance().DestroyStageBGM(false);  // BGM�폜

				ScenePlay* sp = new ScenePlay();
				sp->MoveToNextStage(ScenePlay::GetStageID(), ScenePlay::GetGameDifficulty());
				delete sp; sp = nullptr;

				return true;
			}
		}

		return false;
	}


	bool EnemyManager::SeqMoveToResult(const float deltaTime) {

		// �{�X��|���A�X�e�[�W�R���N���A������
		if (IsKilledStageBoss() && ScenePlay::GetStageID() == 3) {

			_isDefeatedAllStageEnemy = true;

			SetFontSize(60);
			DrawString(470, 250, "Accomplished", GetColor(0, 255, 0));
			SetFontSize(30);
			DrawString(535, 450, "Move to Result!", GetColor(0, 255, 0));

			// ����{�^������
			if (InputFuncTable::IsButtonTrigger_ENTER()) {

				inl::PlayerBullet::ResetBulletPowerRate();          // �v���C���[�̒e�З͏�����

				SoundManager::GetInstance().DestroyStageBGM(false); // BGM�폜

				ScenePlay::DestroyThirdStageBulletHellLists();      // ��R�X�e�[�W�{�X�̒e�����폜

				MoveToResult();

				return true;
			}
		}

		return false;
	}


	void EnemyManager::MoveToResult()
	{
		auto mgr = SceneManager::GetInstance();
		int total = ScoreManager::GetInstance().GetTotalScore();

		if (ScenePlay::GetGameDifficulty() == "Easy")
			mgr->ChangeScene(new SceneResult("Easy", total));

		else if (ScenePlay::GetGameDifficulty() == "Normal")
			mgr->ChangeScene(new SceneResult("Normal", total));

		else if (ScenePlay::GetGameDifficulty() == "Hard")
			mgr->ChangeScene(new SceneResult("Hard", total));

		else if (ScenePlay::GetGameDifficulty() == "Lunatic")
			mgr->ChangeScene(new SceneResult("Lunatic", total));
	}

	// �`��-----------------------------------------------------------------------------------------------------------
	void EnemyManager::Render(const Shared<dxe::Camera>& camera) const {

		_itemManager->Render(camera);

		RenderEventHitText();

		// �U�R
		for (const auto& enemy : _enemyZakoList) {

			if (enemy) 
				enemy->Render(_mainCamera_ref);
		}

		// �{�X
		if (_enemyZakoList.empty() && !_enemyBossList.empty()) {

			if (_isSummonBoss) {

				for (const auto& boss : _enemyBossList) {
					if (boss) 
						boss->Render(_mainCamera_ref);
				}
			}
		}
	}

	// �X�V---------------------------------------------------------------------------------------------------------
	void EnemyManager::UpdateEnemyBossList(const float deltaTime)
	{
		if (!_enemyZakoList.empty() || _enemyBossList.empty()) 
			return;

		if (!_isSummonBoss)
			return;

		for (auto it_boss = _enemyBossList.begin(); it_boss != _enemyBossList.end();) {

			if ((*it_boss)->Update(deltaTime) == false) {

				//�@�폜
				it_boss = _enemyBossList.erase(it_boss);
				_isSummonBoss = false;
			}
			else {
				//�@�X�V
				it_boss++;
			}
		}
	}


	void EnemyManager::UpdateEnemyZakoList(const float deltaTime)
	{
		if (_enemyZakoList.empty()) 
			return;

		for (auto it_zako = _enemyZakoList.begin(); it_zako != _enemyZakoList.end();) {

			if ((*it_zako)->Update(deltaTime) == false) {

				// �֐�����ItemManager�N���X�ɒʒm
				SendEnemyPosition((*it_zako)->_mesh->pos_, (*it_zako)->_isDead);

				//�@�폜
				it_zako = _enemyZakoList.erase(it_zako);
			}
			else {

				//�@�X�V
				it_zako++;
			}
		}
	}


	float EnemyManager::_bossAppearanceTextTimer;

	void EnemyManager::Update(const float deltaTime) {

		_sequence.update(deltaTime);

		_itemManager->Update(deltaTime);

		if (_enemyZakoList.empty()) {

			CheckDoSpawnEnemy();
		}

		//�@�C�x���g�e�L�X�g
		UpdateEventHitText(deltaTime);

		//�@�{�X�o���e�L�X�g
		UpdateBossAppearanceTextTimer(deltaTime);

		// �U�R�E�{�X�����蔻��
		EnemyZakoCollisionPairLists();
		EnemyBossCollisionPairLists();

		// �U�R�E�{�X�X�V
		UpdateEnemyZakoList(deltaTime);
		UpdateEnemyBossList(deltaTime);
	}
}