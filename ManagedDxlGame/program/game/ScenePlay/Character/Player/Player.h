#pragma once
#include "../Character.h"

class CsvLoader;

  //  �G�U�R����ѓG�{�X���܂Ƃ߂ĊǗ�


namespace inl {

	class Gunport;
	class PlayerBullet;
	class EnemyManager;
	class FreeLookCamera;


	class Player : public Character
	{
	public:

		Player() {}
		explicit Player(const Shared<FreeLookCamera> cameraRef);

		~Player() override { DeleteSoundMem(_getDamageSE_hdl); }


		// Init--------------------------------------------------------------------------------------------
		void InitBombCount(const int count) { _currentBomb_stockCount = count; }

		// Getter--------------------------------------------------------------------------------------------
		const int GetHP() const noexcept { return _hp; }
		const int GetMaxHP() const noexcept { return _MAX_HP; }
		const int GetAT() const noexcept { return _at; }
		const int GetDEF() const noexcept { return _def; }
		const tnl::Vector3 GetPos() const { return _mesh->pos_; }
		const bool GetIsTriggeredBombEffect() const noexcept { return _isTriggered_playersBombEffect; }

		// Setter--------------------------------------------------------------------------------------------
		void SetHP(int val) { _hp = val; }
		void SetIsInvincible(const bool flag) { _isInvincible = flag; }
		void SetEnemyManagerRef(const Shared<EnemyManager>& enemyManager) { _enemyManager_ref = enemyManager; };
		void SetPlayerRef(const Shared<Player>& playerRef) { _player_ref = playerRef; }

		// UpdateStatus--------------------------------------------------------------------------------------------
		void HealHP(int heal) { _hp += heal; }
		bool DecreaseHP(int damage);
		void AddAT(int val) { _at += val; }
		void AddDEF(int val) { _def += val; }
		void AddBombStockCount() { _currentBomb_stockCount++; }
		void AddSpeed(float val) { _playerMoveSpeed += val; }

		// Others--------------------------------------------------------------------------------------------		
		void PlayDamageHitSE() noexcept;

		void Update(const float deltaTime);
		void Render(const Shared<FreeLookCamera> playerCamera);

	private:

		// �J����--------------------------------------------------------------------------------------------------------------
		void ActivateDarkSoulsCamera();               // �_�[�N�\�E���̂悤�ȃJ�������[�N�ɋ߂Â�������
		void NormalizeCameraSpeed(const float speed); // �X�s�[�h����
		void ControlCameraWithoutEnemyFocus();
		void ControlCameraWithEnemyFocus(tnl::Vector3& playerPos, tnl::Vector3& targetEnemyPos);
		void ControlPlayerMoveWithEnemyFocus(tnl::Quaternion& q, float& y);

		// �v���C���[ ----------------------------------------------------------------------------------------------------------

		// HP�AAT�ADEF�Ȃǂ̃X�e�[�^�X������----------------------
		void InitPlayerStatus();

		// ���G����-----------------------------------------------
		void WatchInvincibleTimer(const float deltaTime) noexcept;
		void TriggerInvincible(const Shared<FreeLookCamera>& camera);

		// HP -----------------------------------------------------
		void RenderPlayerHp();

		// �ړ� ---------------------------------------------------
		void ControlPlayerMoveByInput(const float deltaTime);    // �ړ�����
		void AdjustPlayerVelocity();                             // ���x����
		void ControlRotationByPadOrMouse();                      // ���_����

		// �e --------------------------------------------------------------------------------------------------------------------
		void ShotPlayerBullet();                          // �e��������
		void UpdateStraightBullet(const float deltaTime); // �e�̍X�V����
		const tnl::Vector3& GetBulletMoveDirection();     // �e�̈ړ������擾

		bool IsShooting() noexcept {
			return
				tnl::Input::IsMouseDown(eMouse::LEFT) ||
				tnl::Input::IsPadDown(ePad::KEY_1);
		}

		// �A���C------------------------------------------------------------------------------------------------------------------
		void ShotGunportBullet();                                  // ���ˏ���
		void RenderBulletPowerRate();                              // �e�̌��݂̃p���[��\��
		void RenderGunport(const Shared<FreeLookCamera> camera);   // �`��
		void UpdateGunport();                                      // �X�V
		void UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords); // DRY�����ɂ��A�����������܂Ƃ߂Ď��s

		// �{��--------------------------------------------------------------------------------------------------------------------
		void UseBomb();                                            // �g�p
		void ValidateBombEffect();                                 // �G�t�F�N�g�L����
		void InvalidateBombEffect(const float deltaTime);          // �G�t�F�N�g������
		void RenderBombRemainCount() noexcept;                     // �{���c���`��

		// �G�ʒu------------------------------------------------------------------------------------------------------------------
		void ChangeTarget_ByMouseWheel();                          // �}�E�X�z�C�[���Ń^�[�Q�b�g�ύX����
		void RenderFollowPointer();                                // �^�[�Q�b�g�ʒu�ɍ��킹�ĕ`�悷��|�C���^�[
		void AssignEnemyPosition(tnl::Vector3& enemyPos);          // ���[�J���ϐ��ɓG���W�ʒu�����蓖�Ă�
		bool IsEnemyInCapturableRange();                           // �G���J�����Œ�@�\���g�p�\�Ȕ͈͓��ɂ��邩

	public:

		std::list<Shared<PlayerBullet>> _straightBullets_player{};

		std::vector<Shared<Gunport>>    _gunportVec{};

		static Shared<dxe::Particle>    _bombParticle;

	private:

		Shared<EnemyManager>            _enemyManager_ref = nullptr;

		Shared<Gunport>                 _playerGunport = nullptr;

		Shared<CsvLoader>               _csvLoader = nullptr;

		Shared<FreeLookCamera>          _playerCamera = nullptr;

	private:

		// �v���C���[�X�e�[�^�X -------------------------------------------------
		int                _hp{};                    // CSV
		float              _playerMoveSpeed{ 0.4f };

		// �{��------------------------------------------------------------------
		int                _currentBomb_stockCount{};
		float              _bombTimer{};
		bool               _isTriggered_playersBombEffect{}; // �`��t���O

		// ���G����--------------------------------------------------------------
		float              _invincibleTimer{};
		bool               _isInvincible{ false };

		// �_���[�WSE -----------------------------------------------------------
		int                _getDamageSE_hdl{};

		// �G���@--------------------------------------------------------------
		int                _enemyIndex{}; // ���[�_�[�|�C���^�[�g�p���Ɏg�p

		// �v���C���[����-------------------------------------------------------
		tnl::Vector3       _moveVelocity{};         // �ړ��x�N�g��
		tnl::Vector3       _past_moveVelocity{};    // �O��̈ړ��x�N�g��
		tnl::Vector3       _centerOfGravity{};      // �d�S���W
		tnl::Quaternion    _rotY{};
		tnl::Quaternion    _rotX{};
		tnl::Quaternion    _rotXZ{};
	};
}