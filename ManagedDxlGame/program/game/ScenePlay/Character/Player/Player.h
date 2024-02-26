#pragma once
#include "../../../DxLibEngine.h"

class PlayerBullet;
class Gunport;
class EnemyManager;
class FreeLookCamera;
class CsvLoader;


class Player
{
public:

	Player() {}
	explicit Player(const Shared<FreeLookCamera> camera_ref);

	~Player() { DeleteSoundMem(_getDamageSE_hdl); }
		

	// Init--------------------------------------------------------------------------------------------
	void InitBombCount(const int count) { _currentBomb_stockCount = count; }

	// Getter--------------------------------------------------------------------------------------------
	int GetHP() const { return _hp; }
	int GetMaxHP() const { return _MAX_HP; }
	int GetAT() const { return _at; }
	int GetDEF() const { return _def; }
	const tnl::Vector3 GetPos() const { return _mesh->pos_; }
	bool GetIsTriggeredBombEffect() const { return _isTriggered_playersBombEffect; }

	// Setter--------------------------------------------------------------------------------------------
	void SetHP(int val) { _hp = val; }
	void SetIsInvincible(const bool flag) { _isInvincible = flag; }
	void SetEnemyManagerRef(const Shared<EnemyManager>& enemyMgr) { _enemyManager_ref = enemyMgr; };
	void SetPlayerRef(const Shared<Player>& player_ref) { _player_ref = player_ref; }

	// UpdateStatus--------------------------------------------------------------------------------------------
	void HealHP(int heal) { _hp += heal; }
	bool DecreaseHP(int damage);
	void AddAT(int val) { _at += val; }
	void AddDEF(int val) { _def += val; }
	void AddBombStockCount() { _currentBomb_stockCount++; }
	void AddSpeed(int val) { _moveSpeed += val; }

	// Others--------------------------------------------------------------------------------------------		
	void PlayDamageHitSE();

	void Update(const float delta_time);
	void Render(const Shared<FreeLookCamera> playerCamera);

private:

	// プレイヤー－-----------------------－-----------------------－------------------------------
	void InitPlayerStatus(const std::string difficulty);       // HP、AT、DEFなどのステータス初期化
	void RenderPlayerHp();
	void WatchInvincibleTimer(const float delta_time);         // 無敵時間
	void TriggerInvincible(const Shared<FreeLookCamera>& camera);

	void ControlPlayerMoveByInput(const float delta_time);   // 移動操作
	void AdjustPlayerVelocity();                             // 速度調整
	void ControlRotationByPadOrMouse();                      // 視点操作

	bool IsShooting() {
		return 
			tnl::Input::IsMouseDown(eMouse::LEFT) ||
			tnl::Input::IsPadDown(ePad::KEY_1);
	}

	void ShotPlayerBullet(const float deltaTime);            // 弾
	void UpdateStraightBullet(float delta_time);
	void RenderBulletPowerRate();
	const tnl::Vector3& GetBulletMoveDirection();

	void RenderGunport(const Shared<FreeLookCamera> camera); // 連装砲
	void UpdateGunport();
	void UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords);
	void ShotGunportBullet();

	void UseBomb();                                          // ボム
	void ValidateBombEffect();
	void InvalidateBombEffect(const float delta_time);
	void RenderBombRemainCount();

	void ChangeTarget_ByMouseWheel();	                     // 敵位置
	void RenderFollowPointer();
	bool IsEnemyInCapturableRange();
	void AssignEnemyPosition(tnl::Vector3& enemy_pos);

	// カメラ－-----------------------－-----------------------－----------------------－-----------------------－--
	void ActivateDarkSoulsCamera();
	void ControlCameraWithEnemyFocus(tnl::Vector3& player_pos, tnl::Vector3& target_enemy_pos);
	void ControlCameraWithoutEnemyFocus();
	void NormalizeCameraSpeed(const float speed);

public:

	Shared<dxe::Mesh>               _mesh = nullptr;

	std::list<Shared<PlayerBullet>> _straightBullets_player{};

	std::vector<Shared<Gunport>>    _gunportVec{};

	static Shared<dxe::Particle>    _bombParticle;

private:

	Shared<EnemyManager>            _enemyManager_ref = nullptr;

	Shared<Gunport>                 _playerGunport = nullptr;

	Shared<CsvLoader>               _csvLoader = nullptr;

	Shared<Player>                  _player_ref = nullptr;

	Shared<FreeLookCamera>          _mainCamera_ref = nullptr;

public:

	tnl::Vector3 _collideSize{ 20, 20, 20 };

private:

	// プレイヤーステータス
	int                _hp{};                      // CSV
	int                _MAX_HP{};
	int                _at{};                      // CSV
	int                _def{};                     // CSV
	float              _moveSpeed{ 0.4f };
	float              _forwardVelocity{ 150.0f };
	float              _hp_posX{ 60 };
	float              _hp_posY{ 50 };
	const tnl::Vector3 _START_POSITION{ 0, 100, -300 };
	bool               _isDead{};

	// ボム
	int          _currentBomb_stockCount{};
	float        _bombTimer{};
	const float  _BOMBEFFECT_TIME_LIMIT{ 3.0f };
	bool         _isTriggered_playersBombEffect{}; // 描画フラグ

	// 無敵時間
	float        _invincibleTimer{};
	const float  _INVINCIBLE_TIME_LIMIT{ 3.0f };
	bool         _isInvincible = false;

	// ダメージSE	
	int          _getDamageSE_hdl{};

	// 敵情報　
	int          _enemyIndex{}; // レーダーポインター使用時に使用
	float        _capturableEnemyRange{ 500.0f };

	// カメラ
	const tnl::Vector3 _DEFAULT_CAMERA_POSITION{ 0, 100, -150 };
	const tnl::Vector3 _CAMERA_OFFSET{ 0, -50, 20 };

	// 視点操作
	float _viewpoint_lerpRate_h{ 0.05f };
	float _viewpoint_lerpRate_v{ 0.01f };

	float _cameraMove_delayRate{ 0.05f };

	// プレイヤーとの距離のオフセット
	float _distance_offset{ 300.0f };

	// プレイヤー操作
	float centroid_radius_ = 100; // 重心
	float mass_ = 100;            // 質量
	float friction_ = 0.6f;       // 摩擦
	tnl::Vector3 move_vel_{};
	tnl::Vector3 past_move_vel_{};
	tnl::Vector3 center_of_gravity_{};
	tnl::Quaternion rot_y_{};
	tnl::Quaternion rot_x_{};
	tnl::Quaternion rot_xz_{};
};