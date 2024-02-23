#pragma once
#include "../../../DxLibEngine.h"

class PlayerBullet;
class Gunport;
class EnemyZakoBase;
class EnemyBossBase;
class FreeLookCamera;

class Player
{
public:

	Player() {}
	explicit Player(const Shared<FreeLookCamera> camera_ref);

	// ÉvÉåÉCÉÑÅ[ä÷åW
	void SetPlayerRef(const Shared<Player>& player_ref) { _player_ref = player_ref; }
	void SetBombCount(const int count) { _currentBomb_stockCount = count; }
	bool DecreaseHP(const int damage);
	const tnl::Vector3 GetPos() const { return _mesh->pos_; }
	void SetPos(const tnl::Vector3 pos) { _mesh->pos_ = pos; }
	static void PlayDamageHitSE();

	const tnl::Vector3& GetPlayerPosition() const { return _player_ref->_mesh->pos_; }

	// ìGä÷åW
	void SetEnemyZakoListRef(const std::vector<Shared<EnemyZakoBase>>& enemy_list_ref);
	void SetEnemyBossListRef(const std::vector<Shared<EnemyBossBase>>& enemyBoss_ref);
	void EraseEnemyZakoListRef(Shared<EnemyZakoBase>& enemy_list_ref);
	void EraseEnemyBossListRef(Shared<EnemyBossBase>& enemyBoss_ref);

	void Update(const float delta_time);
	void Render(const Shared<FreeLookCamera> playerCamera);

private:

	// ÉvÉåÉCÉÑÅ[ä÷åW
	void ControlPlayerMoveByInput(const float delta_time);
	void AdjustPlayerVelocity();
	void ControlRotationByPadOrMouse();
	void WatchInvincibleTimer(const float delta_time);
	void ShotPlayerBullet(const float deltaTime);
	void ShotGunportBullet();
	void UseBomb();
	void ValidateBombEffect();
	void InvalidateBombEffect(const float delta_time);
	void RenderBombRemainCount();
	void RenderBulletPowerRate();
	void RenderPlayerHp();
	void RenderGunport(const Shared<FreeLookCamera> camera);
	void UpdateGunport();
	void UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords);
	void UpdateStraightBullet(float delta_time);
	bool IsShooting();

	// ìGä÷åW
	const tnl::Vector3 GetTargetsScreenCoordinates(const float& x, const float& y, const float& z);
	void ChangeTarget_ByMouseWheel();
	void RenderFollowPointer();
	bool IsEnemyInCapturableRange();
	const tnl::Vector3& GetEnemyPosition();


	// ÉJÉÅÉâä÷åW
	void AssignTargetEnemy_ForDarkSoulsCamera(tnl::Vector3& target_enemy_pos);
	void ActivateDarkSoulsCamera();
	void ControlCameraWithEnemyFocus(tnl::Vector3& player_pos, tnl::Vector3& target_enemy_pos);
	void ControlCameraWithoutEnemyFocus();
	void NormalizeCameraSpeed(const float speed);


public:

	Shared<dxe::Mesh>                  _mesh = nullptr;

	std::list<Shared<PlayerBullet>>    _straightBullets_player{};

	std::vector<Shared<Gunport>>       _gunportVec{};

	static Shared<dxe::Particle>       _bombParticle;

private:

	Shared<Gunport>                    _playerGunport = nullptr;

	Shared<Player>                     _player_ref = nullptr;
	Shared<FreeLookCamera>             _mainCamera_ref = nullptr;

	Shared<EnemyZakoBase>              _enemyZako_ref = nullptr;
	Shared<EnemyBossBase>              _enemyBoss_ref = nullptr;

	std::vector<Shared<EnemyZakoBase>> _enemyZakoList_ref{};
	std::vector<Shared<EnemyBossBase>> _enemyBossList_ref{};

public:

	tnl::Vector3 _collideSize{};
	static bool  _isInvincible;

	static int   _hp;
	static int   _MAX_HP;
	static int   _at;
	static int   _def;
	static int   _currentBomb_stockCount;
	static float _moveSpeed;

private:



	std::vector<Shared<EnemyZakoBase>>::iterator _it_zako_ref{};
	std::vector<Shared<EnemyBossBase>>::iterator _it_boss_ref{};
	tnl::Vector3 _enemyPos_ref{};

	int          _enemyIndex{};
	static int   _getDamageSE_hdl;

	float        _forwardVelocity = 1.0f;

	bool         _isDead{};

	static float _invincibleTimer;
	const float  _INVINCIBLE_TIME_LIMIT = 3.0f;

	static float _bombTimer;


	float centroid_radius_ = 100; // èdêS
	float mass_ = 100;            // éøó 
	float friction_ = 0.6f;       // ñÄéC
	tnl::Vector3 move_vel_{};
	tnl::Vector3 past_move_vel_{};
	tnl::Vector3 center_of_gravity_{};
	tnl::Quaternion rot_y_{};
	tnl::Quaternion rot_x_{};
	tnl::Quaternion rot_xz_{};
};