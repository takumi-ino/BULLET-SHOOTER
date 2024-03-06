#pragma once
#include "./../../DxLibEngine.h"
#include "../../../library/tnl_sequence.h"
#include "../../Loader/CsvLoader.h"

class Collision;
class Score;
class EnemyZakoBox;
class EnemyZakoDome;
class EnemyZakoCylinder;
class EnemyZakoBase;
class EnemyBossBase;
class Player;
class ItemManager;
class StraightBullet;
class HomingBullet;
class EnemyBullet;
class EventNoticeText;

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
	int GetRemainingEnemyCount() const;

	//------------------------------------------------------------
	void Render(const Shared<dxe::Camera>& camera) const;
	void Update(const float& deltaTime);

private:

	// �U�R�̂�------------------------------------------------------------------------------------------------------
	void InitEnemyZakoInfo();                          // ������
	void UpdateEnemyZakoList(const float& deltaTime);  // �X�V
	void SetMaxEnemySpawnCount();                      // 1�x�ɐ����\�ȓG����ݒ�i��Փx���Ƃɒ����j

	// �{�X�̂�------------------------------------------------------------------------------------------------------
	void InitEnemyBossInfo();                          // ������
	void UpdateEnemyBossList(const float& deltaTime);  // �X�V

	// �����͂��邪�A�U�R���S�ł���܂Ń{�X�͔�A�N�e�B�u
	void SetSpawnEnemyBoss();

	// �{�X���o�������Ƃ��Ɉ�莞�ԕ\������e�L�X�g
	void ShowBossAppearanceText();

	// ShowBossAppearanceText �֐��̗L�����Ԃ��v��
	void UpdateBossAppearanceTextTimer(const float& deltaTime);

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
	void NotifyEnemyPosition_ToItemManager();                             // �A�C�e���X�|�[���ɕK�v�ȓG�̈ʒu����ʒB
	void SendEnemyPosition(const tnl::Vector3& new_position, const bool isEnemyActive);

	// �G�E���C�x���g�ʒm---------------------------------------------------------------------------------------------
	void EventNotify_OnEnemyKilled(const std::string enemy_name);         // �G�̌��j����ʒB
	void RenderEventHitText() const;
	void UpdateEventHitText(const float& deltaTime);

	// �X�e�[�W�ړ�---------------------------------------------------------------------------------------------------
	tnl::Sequence<EnemyManager> _sequence = tnl::Sequence<EnemyManager>(this, &EnemyManager::SeqMoveToNextStage);
	bool SeqMoveToNextStage(const float deltaTime);
	bool SeqMoveToResult(const float deltaTime);

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
	static float   _showBossAppearanceText_timer;

	bool           _isShowBossAppearanceText{};
	bool           _isSummonBoss{ false };
	bool           _isInitializedBossInfo{ false };

	bool           _isDefeatedAllStageEnemy{};

	tnl::Vector3   _enemyBossPos{};

	//�@SE�EBGM�n���h��	---------------------------------------------------------------------------------------------------
	int            _alertSE_hdl{};
	int            _bossBattleBGM_hdl{};
	int            _battleBGM_hdl{};

	//�@�X�R�A---------------------------------------------------------------------------------------------------
	int            _CURRENT_SCORE_REF{};	// PlayerScene����̃X�R�A�Q�ƕϐ�
};