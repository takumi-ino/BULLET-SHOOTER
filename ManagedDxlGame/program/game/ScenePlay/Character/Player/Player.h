#pragma once
#include "../Character.h"

class CsvLoader;



namespace inl {

	class Gunport;
	class PlayerBullet;
	class EnemyManager;
	class FreeLookCamera;


	class Player : public Character
	{
	public:

		Player() {}
		explicit Player(const Shared<FreeLookCamera> mainCamera);

		~Player() override { DeleteSoundMem(_getDamageSE_hdl); }


		// ������--------------------------------------------------------------------------------------------
		void InitBombCount(const int count)                            // �{���J�E���g
		{ 
			_currentBomb_stockCount = count; 
		}

		// �Q�b�^�[--------------------------------------------------------------------------------------------
		const int GetHP() const noexcept { return _hp; }               // HP
		const int GetMaxHP() const noexcept { return _MAX_HP; }		   // �ő�HP
		const int GetAT() const noexcept { return _at; }			   // �U����
		const int GetDEF() const noexcept { return _def; }			   // �h���
		const tnl::Vector3 GetPos() const { return _mesh->pos_; }	   // �ʒu
		const bool GetIsTriggeredBombEffect() const noexcept 		   // �{���G�t�F�N�g
		{ 
			return _isTriggered_playersBombEffect; 
		}

		// �Z�b�^�[------------------------------------------------------------------------------------------------
		void SetHP(int val) { _hp = val; }                                   // HP

		void SetIsInvincible(const bool flag)                                // �������t���O
		{
			_isInvincible = flag;
		}
		void SetPlayerRef(const Shared<Player>& playerRef)                   // ScenePlay����̃v���C���[�I�u�W�F�N�g�擾
		{ 
			_player_ref = playerRef; 
		}
		void SetEnemyManagerRef(const Shared<EnemyManager>& enemyManager)    // ScenePlay����̃G�l�~�[�}�l�[�W���[�擾
		{ 
			_enemyManager_ref = enemyManager; 
		}

		// �X�e�[�^�X�X�V-----------------------------------------------------------------------------------------
		void HealHP(int heal) { _hp += heal; }                               // HP��
		bool DecreaseHP(int damage);										 // HP����
		void AddAT(int val) { _at += val; }									 // �U���͏㏸
		void AddDEF(int val) { _def += val; }								 // �h��͏㏸
		void AddBombStockCount() { _currentBomb_stockCount++; }				 // �{���J�E���g����
		void AddSpeed(float val) { _playerMoveSpeed += val; }				 // �X�s�[�h�㏸

		// Others-------------------------------------------------------------------------------------------------
		void PlayDamageHitSE() noexcept;				                     // �_���[�W��

		void Update(const float deltaTime);
		void Render(const Shared<FreeLookCamera> playerCamera);


	private:

		// �J����-------------------------------------------------------------------------------------------------

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
		void TriggerInvincible(const Shared<FreeLookCamera>& camera);

		// HP 
		void RenderPlayerHp(const int color);

		// �ړ� 
		void ControlPlayerMoveByInput(const float deltaTime);  // �ړ�����
		void AdjustPlayerVelocity();                           // ���x����
		void ControlRotationByPadOrMouse();                    // ���_����

		// �e -------------------------------------------------------------------------------------------------------
		void ShotPlayerBullet();                               // �e��������
		void UpdateStraightBullet(const float deltaTime);      // �e�̍X�V����
		const tnl::Vector3& GetBulletMoveDirection();          // �e�̈ړ������擾

		bool IsShooting() noexcept {
			return
				tnl::Input::IsMouseDown(eMouse::LEFT) ||
				tnl::Input::IsPadDown(ePad::KEY_1);
		}

		// �A���C-----------------------------------------------------------------------------------------------------
		void ShotGunportBullet();                                  // ���ˏ���
		void RenderBulletPowerRate(const int color);               // �e�̌��݂̃p���[��\��
		void RenderGunport(const Shared<FreeLookCamera> camera);   // �`��
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
		void RenderPlayerParticle(const Shared<inl::FreeLookCamera>& camera);

	public:

		std::list<Shared<PlayerBullet>> _straightBullets_player{};       // �v���C���[�̒��s�e

		std::vector<Shared<Gunport>>    _gunportVec{};                   // �A���C�x�N�^�[

		static Shared<dxe::Particle>    _bombParticle;                   // �{���p�[�e�B�N��

	private:

		Shared<EnemyManager>            _enemyManager_ref = nullptr;    // �G�l�~�[�}�l�[�W���[

		Shared<Gunport>                 _playerGunport = nullptr;		// �A���C

		Shared<CsvLoader>               _csvLoader = nullptr;			// CSV

		Shared<FreeLookCamera>          _playerCamera = nullptr;		// �J����

		Shared<dxe::Particle>			_playerParticle = nullptr;      // �v���C���[�̑O���ɔz�u����p�[�e�B�N��

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
		bool               _isInvincible{ false };    // �������t���O

		// �_���[�WSE -----------------------------------------------------------
		int                _getDamageSE_hdl{};        // �_���[�W��SE

		// �G���@--------------------------------------------------------------
		// �@�@�@�@�@�ʏ�G�l�~�[�Q�ƃC���f�b�N�X�i���[�_�[�|�C���^�[�g�p���Ɏg�p�j
		int                _enemyIndex{};             

		// �v���C���[����-------------------------------------------------------
		tnl::Vector3       _moveVelocity{};           // �ړ��x�N�g��
		tnl::Vector3       _past_moveVelocity{};      // �O��̈ړ��x�N�g��
		tnl::Vector3       _centerOfGravity{};        // �d�S���W
		tnl::Quaternion    _rotY{};                   // Y����]
		tnl::Quaternion    _rotX{};				      // X����]
		tnl::Quaternion    _rotXZ{};			   	  // XZ����]
	};
}