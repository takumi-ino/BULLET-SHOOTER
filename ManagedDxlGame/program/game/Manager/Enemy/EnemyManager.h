#pragma once
#include "../../Loader/CsvLoader.h"

class ItemManager;

namespace inl {

	class Collision;
	class EnemyZakoBase;
	class EnemyBossBase;

	// �����E�폜�E�X�V���Ǘ�
	class EnemyManager
	{
	public:

		EnemyManager() {}
		EnemyManager(
			const Shared<Player>& player,
			const Shared<dxe::Camera>& camera,
			const Shared<Collision>& collision
		);

		virtual ~EnemyManager() {
			_enemyZakoList.clear();
			_enemyBossList.clear();
		}

		// �G���W�擾------------------------------------------------------------
		const std::vector<tnl::Vector3>& GetEnemyZakoPosition();
		const tnl::Vector3& GetEnemyBossPosition();

		// �c��̓G���擾------------------------------------------------------------
		const int GetRemainingEnemyCount() const noexcept { return _zakoEnemyTotalLeftCount; }

		//------------------------------------------------------------
		void Render(const Shared<dxe::Camera>& camera) const;
		void Update(const float deltaTime);

	private:

		// �U�R�G�݂̂̏���----------------------------------------------------------------------------------------
		void InitEnemyZakoInfo();                          // ������
		void UpdateEnemyZakoList(const float deltaTime);   // �X�V
		void SetMaxEnemySpawnCount() noexcept;             // 1�x�ɐ����\�ȓG����ݒ�i��Փx���Ƃɒ����j

		// �{�X�݂̂̏���------------------------------------------------------------------------------------------
		void InitEnemyBossInfo();                          // ������
		void UpdateEnemyBossList(const float deltaTime);   // �X�V

		// �����͂��邪�A�U�R���S�ł���܂Ń{�X�͔�A�N�e�B�u
		void SetSpawnEnemyBoss();

		// �{�X���o�������Ƃ��Ɉ�莞�ԕ\������e�L�X�g
		void ShowBossAppearanceText() noexcept;

		// ShowBossAppearanceText �֐��̗L�����Ԃ��v��
		void UpdateBossAppearanceTextTimer(const float deltaTime) noexcept;

		// �X�V�����J�n
		void SummonBoss();

		// �U�R�E�{�X����-------------------------------------------------------------------------------------------------
		void CheckDoSpawnEnemy();           // ��萔�̃U�R�G��|�����ƂŁA�{�X�������͐V���ȃU�R�G������
		void LoadEnemyDataFromCsv();        // CSV�t�@�C������G�������ɕK�v�ȏ����擾
		bool IsKilledStageBoss(); 	        // �U�R���X�g�y�у{�X���X�g�̃I�u�W�F�N�g���ǂ�����O�ɂȂ����� true

		// �����蔻��-----------------------------------------------------------------------------------------------------
		void EnemyZakoCollisionPairLists();
		void EnemyBossCollisionPairLists();

		// �A�C�e���擾�C�x���g�ʒm---------------------------------------------------------------------------------------
		void AttachItemManagerInstance(const Shared<ItemManager>& observer);

		// �A�C�e���X�|�[���ɕK�v�ȓG�̈ʒu����ʒB
		void NotifyEnemyPosition_ToItemManager();                 

		void SendEnemyPosition(const tnl::Vector3& newPosition, const bool isEnemyActive);

		// �G�E���C�x���g�ʒm---------------------------------------------------------------------------------------------
		void EventNotify_OnEnemyKilled(const std::string enemyName);    // �G�̌��j����ʒB
		void RenderEventHitText() const;								// �e�L�X�g�`��
		void UpdateEventHitText(const float deltaTime);					// �e�L�X�g�X�V

		// �X�e�[�W�ړ�---------------------------------------------------------------------------------------------------
		tnl::Sequence<EnemyManager> _sequence = tnl::Sequence<EnemyManager>(this, &EnemyManager::SeqMoveToNextStage);
		bool SeqMoveToNextStage(const float deltaTime);

		// ���U���g�֔�ԑO�ɕK�v�ȏ������܂Ƃ߂Ď��s
		bool SeqMoveToResult(const float deltaTime);    

		void MoveToResult(); // �����ȃ��U���g�ڍs�@�\

	public:

		std::vector<Shared<EnemyZakoBase>>     _enemyZakoList{};
		std::vector<Shared<EnemyBossBase>>     _enemyBossList{};

		std::vector<Shared<EnemyZakoBase>>::iterator _itZako{};
		std::vector<Shared<EnemyBossBase>>::iterator _itBoss{};

	private:

		Shared<ItemManager>                    _itemManager = nullptr;
		Shared<CsvLoader>                      _csvLoader = nullptr;

		// �Q��				                   
		Shared<Player>                         _player_ref = nullptr;
		Shared<dxe::Camera>                    _mainCamera_ref = nullptr;
		Shared<Collision>                      _collision_ref = nullptr;

		// Zako
		std::unordered_map<int, EnemyZakoInfo> _enemyZakoData_map{};
		EnemyZakoInfo                          _sEnemy_zakoBox_info{};
		EnemyZakoInfo                          _sEnemy_zakoDome_info{};
		EnemyZakoInfo                          _sEnemy_zakoCylinder_info{};

		// Boss
		std::unordered_map<int, EnemyBossInfo> _enemyBossData_map{};
		EnemyBossInfo                          _sBoss_PatchouliKnowledge_info{};
		EnemyBossInfo                          _sBoss_Cirno_info{};
		EnemyBossInfo                          _sBoss_MoriyaSuwako_info{};

	private:

		// �I�u�U�[�o�[�p�^�[���B�G�̈ʒu����A�C�e�����X�|�[��
		std::vector<Shared<ItemManager>>       _observerItems;
		tnl::Vector3                           _enemyZako_position{};
		bool                                   _isEnemyZako_dead{};

	private:

		//�@�U�R---------------------------------------------------------------------------------------------------

		// �G�̎c�萶���\���i_remainingEnemyZako_spawnCount - �����������j
		int                        _remainingEnemyZako_spawnCount{};

		// PlayScene��Getter�œn���A�v���C���[�Ƃ܂��퓬�\�ȓG�̑����i�����ς݁E�������������܂ށj
		int                        _zakoEnemyTotalLeftCount{};

		// 1�x�ɐ������\�ȍő吔
		int                        _maxEnemySpawnCount_PerInterval{};

		std::vector<tnl::Vector3>  _enemyPosList{};

		//�@�{�X---------------------------------------------------------------------------------------------------
		static float   _bossAppearanceTextTimer;            // �{�X�o���x���e�L�X�g�̃^�C�}�[
		bool           _isShowBossAppearanceText{};         // �{�X�o���x���e�L�X�g�̃t���O

		bool           _isSummonBoss{ false };              // �{�X�����t���O
		bool           _isInitializedBossInfo{ false };	    // �{�X�������t���O

		bool           _isDefeatedAllStageEnemy{};          // �G��S�ł��������̃t���O

		tnl::Vector3   _enemyBossPos{};

		//�@SE�EBGM�n���h��	---------------------------------------------------------------------------------------------------
		int            _alertSE_hdl{};
		int            _bossBattleBGM_hdl{};
		int            _battleBGM_hdl{};

		//�@�X�R�A---------------------------------------------------------------------------------------------------
		int            _CURRENT_SCORE_REF{};	// PlayerScene����̃X�R�A�Q�ƕϐ�
	};
}