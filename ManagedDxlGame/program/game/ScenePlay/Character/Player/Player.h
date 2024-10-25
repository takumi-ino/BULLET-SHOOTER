#pragma once
#include "../Character.h"
#include "../../ScenePlay.h"

class CsvLoader;

namespace inl {

	class Gunport;
	class PlayerBullet;
	class EnemyManager;

	class Player : public Character
	{
	public:

		Player();

		~Player() override { DeleteSoundMem(_getDamageSE_hdl); }

		// ������--------------------------------------------------------------------------------------------
		void InitBombCount(const int count) { _currentBomb_stockCount = count; }			// �{���J�E���g

		// �Q�b�^�[--------------------------------------------------------------------------------------------
		const int GetHP() const noexcept { return ScenePlay::GetInstance()->_player->_hp; }          // HP
		const int GetMaxHP() const noexcept { return ScenePlay::GetInstance()->_player->_MAX_HP; }	 // �ő�HP
		const int GetAT() const noexcept { return ScenePlay::GetInstance()->_player->_at; }			 // �U����
		const int GetDEF() const noexcept { return ScenePlay::GetInstance()->_player->_def; }		 // �h���
		const tnl::Vector3 GetPos() const { return ScenePlay::GetInstance()->_player->_mesh->pos_; } // �ʒu

		// �{���G�t�F�N�g
		const bool GetIsTriggeredBombEffect() const noexcept { return _isTriggered_playersBombEffect; }

		// �Z�b�^�[------------------------------------------------------------------------------------------------
		void SetHP(int val) { ScenePlay::GetInstance()->_player->_hp = val; }						 // HP

		void ResetPositionAndRotation() {

			ScenePlay::GetInstance()->_player->_mesh->pos_ = { 0, 100, -300 };
			ScenePlay::GetInstance()->_player->_mesh->rot_ = { 0, 0,0,0 };
		}

		// �X�e�[�^�X�X�V-----------------------------------------------------------------------------------------
		bool DecreaseHP(int damage);																 // HP����

		void HealHP(int heal) {		 // HP��
			ScenePlay::GetInstance()->_player->_hp += heal;
			PlaySoundMem(_healSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddAT(int val) {		 // �U���͏㏸
			ScenePlay::GetInstance()->_player->_at += val;
			PlaySoundMem(_enhanceSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddDEF(int val) {		// �h��͏㏸
			ScenePlay::GetInstance()->_player->_def += val;
			PlaySoundMem(_enhanceSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddSpeed(float val) {  // �X�s�[�h�㏸
			ScenePlay::GetInstance()->_player->_playerMoveSpeed += val;
			PlaySoundMem(_enhanceSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddBombStockCount() {  // �{���J�E���g����
			ScenePlay::GetInstance()->_player->_currentBomb_stockCount++;
			PlaySoundMem(_getBombSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}

		// Others-------------------------------------------------------------------------------------------------
		void PlayDamageHitSE() noexcept { PlaySoundMem(_getDamageSE_hdl, DX_PLAYTYPE_BACK, TRUE); }
		static void PlayIncreaseGunportSe() noexcept { PlaySoundMem(_increaseGunport, DX_PLAYTYPE_BACK, TRUE); }
		static void PlayGetScoreSE() noexcept { PlaySoundMem(_getScoreSE_hdl, DX_PLAYTYPE_BACK, TRUE); }

		void Update(const float deltaTime);
		void Render();

	private:

		// �J����-------------------------------------------------------------------------------------------------
		void CameraTrigger();

		// �_�[�N�\�E���̂悤�ȃJ�������[�N�ɋ߂Â�������
		void ActivateDarkSoulsCamera();
		// �X�s�[�h����
		void NormalizeCameraSpeed(const float speed);
		// �J�����Œ�
		void ControlCameraWithoutEnemyFocus();
		// �J������Œ�
		void ControlCameraWithEnemyFocus(const tnl::Vector3& playerPos, const tnl::Vector3& targetEnemyPos);
		// �J�����Œ莞�v���C���[����
		void ControlPlayerMoveWithEnemyFocus(tnl::Quaternion& q, float& y);

		// �v���C���[---------------------------------------------------------------------------------------------

		// HP�AAT�ADEF�Ȃǂ̃X�e�[�^�X������
		void InitPlayerStatus(std::vector<std::vector<tnl::CsvCell>>);

		// ���G����
		void WatchInvincibleTimer(const float deltaTime) noexcept;
		void TriggerInvincible();

		// HP 
		void RenderPlayerHp(const int color);

		// �ړ� 
		void ControlPlayerMoveByInput(const float deltaTime);  // �ړ�����
		void AdjustPlayerVelocity();                           // ���x����
		void ControlRotationByPadOrMouse();                    // ���_����

		// �e -------------------------------------------------------------------------------------------------------
		void ShotPlayerBullet(const tnl::Vector3 spawnPos, const tnl::Vector3 moveDir); // �e��������
		void UpdateStraightBullet(const float deltaTime);      // �v���C���[�̒e�̍X�V����
		void UpdateGunportBullet(const float deltaTime);       // �A���C��

		const tnl::Vector3& GetBulletMoveDirection();          // �e�̈ړ������擾

		bool IsShooting() noexcept {
			return
				tnl::Input::IsMouseDown(eMouse::LEFT) ||
				tnl::Input::IsPadDown(ePad::KEY_1);
		}

		// �A���C-----------------------------------------------------------------------------------------------------
		void ShotGunportBullet(tnl::Vector3 spawnPos, const tnl::Vector3 moveDir);                                  // ���ˏ���
		void RenderBulletPowerRate(const int color);               // �e�̌��݂̃p���[��\��
		void RenderGunport();									   // �`��
		void UpdateGunport();                                      // �X�V

		// DRY�����ɂ��A�����������܂Ƃ߂Ď��s
		void UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords);

		// �{��-------------------------------------------------------------------------------------------------------
		void UseBomb();                                            // �g�p
		void ValidateBombEffect();                                 // �G�t�F�N�g�L����
		void InvalidateBombEffect(const float deltaTime);          // �G�t�F�N�g������
		void RenderBombRemainCount(const int color) noexcept;      // �{���c���`��

		// �G�ʒu-----------------------------------------------------------------------------------------------------
		void ChangeTarget_ByMouseWheel();                          // �}�E�X�z�C�[���Ń^�[�Q�b�g�ύX����
		void RenderFollowPointer();                                // �^�[�Q�b�g�ʒu�ɍ��킹�ĕ`�悷��|�C���^�[
		void AssignEnemyPosition(tnl::Vector3& enemyPos);          // ���[�J���ϐ��ɓG���W�ʒu�����蓖�Ă�
		bool IsEnemyInCapturableRange();                           // �G���J�����Œ�@�\���g�p�\�Ȕ͈͓��ɂ��邩
		// -----------------------------------------------------------------------------------------------------
		void RenderPlayerParticle();

	public:

		std::vector<Shared<PlayerBullet>> _straightBullet{};			 // �v���C���[�̒��s�e

		std::vector<std::vector<Shared<PlayerBullet>>> _gunportBullet{};				 // �A���C�p�̒e

		std::vector<Shared<Gunport>>    _gunportVec{};                   // �A���C�i�{�́j�x�N�^�[

		static Shared<dxe::Particle>    _bombParticle;                   // �{���p�[�e�B�N��

		Shared<dxe::Particle>			_playerParticle = nullptr;       // �v���C���[�̑O���ɔz�u����p�[�e�B�N��

	private:

		Shared<Gunport>                 _playerGunport = nullptr;	     // �A���C

		Shared<CsvLoader>               _csvLoader = nullptr;			 // CSV

	public:

		bool               _isInvincible{ false };    // �������t���O

	private:

		// �v���C���[�X�e�[�^�X -------------------------------------------------
		int                _hp{};                       // HP�BCSV���烍�[�h
		float              _playerMoveSpeed{ 0.4f };    // �X�s�[�h

		// �{��------------------------------------------------------------------
		int                _currentBomb_stockCount{};         // �{���J�E���g
		float              _bombTimer{};					  // �{���^�C�}�[
		bool               _isTriggered_playersBombEffect{};  // �{���t���O

		// ���G����--------------------------------------------------------------
		float              _invincibleTimer{};        // �������^�C�}�[

		// SE -----------------------------------------------------------
		int				   _getBombSE_hdl{};
		int                _getDamageSE_hdl{};        // �_���[�W��SE
		int				   _healSE_hdl{};
		int				   _enhanceSE_hdl{};
		static int		   _increaseGunport;
		static int		   _getScoreSE_hdl;

		int				   _shotSe{};

		// �G���@--------------------------------------------------------------
		// �@�@�@�@�@�ʏ�G�l�~�[�Q�ƃC���f�b�N�X�i���[�_�[�|�C���^�[�g�p���Ɏg�p�j
		int                _enemyIndex{};

		// �e�@--------------------------------------------------------------
		int				  _bulletFireInterval{4};
		int				  _straightBulletCount{};
		int				  _gunportBulletCount[5];


		// �e�̘A�˂����邽�߂ɕK�v�ƂȂ�C���f�b�N�X
		int				  _bulletOrderIdx{};
		int				  _gunportBulletOrderIdx[5];


		// �v���C���[����-------------------------------------------------------
		tnl::Vector3       _moveVelocity{};           // �ړ��x�N�g��
		tnl::Vector3       _past_moveVelocity{};      // �O��̈ړ��x�N�g��
		tnl::Vector3       _centerOfGravity{};        // �d�S���W
		tnl::Quaternion    _rotY{};                   // Y����]
		tnl::Quaternion    _rotX{};				      // X����]
		tnl::Quaternion    _rotXZ{};			   	  // XZ����]
	};
}