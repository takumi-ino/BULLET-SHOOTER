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


		// 初期化--------------------------------------------------------------------------------------------
		void InitBombCount(const int count) { _currentBomb_stockCount = count; }

		// ゲッター--------------------------------------------------------------------------------------------
		const int GetHP() const noexcept { return _hp; }
		const int GetMaxHP() const noexcept { return _MAX_HP; }
		const int GetAT() const noexcept { return _at; }
		const int GetDEF() const noexcept { return _def; }
		const tnl::Vector3 GetPos() const { return _mesh->pos_; }
		const bool GetIsTriggeredBombEffect() const noexcept { return _isTriggered_playersBombEffect; }

		// セッター--------------------------------------------------------------------------------------------
		void SetHP(int val) { _hp = val; }
		void SetIsInvincible(const bool flag) { _isInvincible = flag; }
		void SetEnemyManagerRef(const Shared<EnemyManager>& enemyManager) { _enemyManager_ref = enemyManager; };
		void SetPlayerRef(const Shared<Player>& playerRef) { _player_ref = playerRef; }

		// ステータス更新--------------------------------------------------------------------------------------------
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

		// カメラ--------------------------------------------------------------------------------------------------------------

		// ダークソウルのようなカメラワークに近づけた処理
		void ActivateDarkSoulsCamera();              
		// スピード調整
		void NormalizeCameraSpeed(const float speed); 
		// カメラ固定
		void ControlCameraWithoutEnemyFocus();
		// カメラ非固定
		void ControlCameraWithEnemyFocus(const tnl::Vector3& playerPos, const tnl::Vector3& targetEnemyPos);
		// カメラ固定時プレイヤー操作
		void ControlPlayerMoveWithEnemyFocus(tnl::Quaternion& q, float& y);

		// プレイヤー ----------------------------------------------------------------------------------------------------------

		// HP、AT、DEFなどのステータス初期化----------------------
		void InitPlayerStatus(std::vector<std::vector<tnl::CsvCell>>);

		// 無敵時間-----------------------------------------------
		void WatchInvincibleTimer(const float deltaTime) noexcept;
		void TriggerInvincible(const Shared<FreeLookCamera>& camera);

		// HP -----------------------------------------------------
		void RenderPlayerHp();

		// 移動 ---------------------------------------------------
		void ControlPlayerMoveByInput(const float deltaTime);  // 移動操作
		void AdjustPlayerVelocity();                           // 速度調整
		void ControlRotationByPadOrMouse();                    // 視点操作

		// 弾 ------------------------------------------------------------------------------------------------------------------
		void ShotPlayerBullet();                               // 弾を撃つ処理
		void UpdateStraightBullet(const float deltaTime);      // 弾の更新処理
		const tnl::Vector3& GetBulletMoveDirection();          // 弾の移動方向取得

		bool IsShooting() noexcept {
			return
				tnl::Input::IsMouseDown(eMouse::LEFT) ||
				tnl::Input::IsPadDown(ePad::KEY_1);
		}

		// 連装砲---------------------------------------------------------------------------------------------------------------
		void ShotGunportBullet();                                  // 発射処理
		void RenderBulletPowerRate();                              // 弾の現在のパワーを表示
		void RenderGunport(const Shared<FreeLookCamera> camera);   // 描画
		void UpdateGunport();                                      // 更新

		// DRY原則につき、同じ処理をまとめて実行
		void UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords);

		// ボム-----------------------------------------------------------------------------------------------------------------
		void UseBomb();                                            // 使用
		void ValidateBombEffect();                                 // エフェクト有効化
		void InvalidateBombEffect(const float deltaTime);          // エフェクト無効化
		void RenderBombRemainCount() noexcept;                     // ボム残数描画

		// 敵位置---------------------------------------------------------------------------------------------------------------
		void ChangeTarget_ByMouseWheel();                          // マウスホイールでターゲット変更処理
		void RenderFollowPointer();                                // ターゲット位置に合わせて描画するポインター
		void AssignEnemyPosition(tnl::Vector3& enemyPos);          // ローカル変数に敵座標位置を割り当てる
		bool IsEnemyInCapturableRange();                           // 敵がカメラ固定機能が使用可能な範囲内にいるか

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

		// プレイヤーステータス -------------------------------------------------
		int                _hp{};                    // CSV
		float              _playerMoveSpeed{ 0.4f };

		// ボム------------------------------------------------------------------
		int                _currentBomb_stockCount{};
		float              _bombTimer{};
		bool               _isTriggered_playersBombEffect{}; // 描画フラグ

		// 無敵時間--------------------------------------------------------------
		float              _invincibleTimer{};
		bool               _isInvincible{ false };

		// ダメージSE -----------------------------------------------------------
		int                _getDamageSE_hdl{};

		// 敵情報　--------------------------------------------------------------
		int                _enemyIndex{}; // レーダーポインター使用時に使用

		// プレイヤー操作-------------------------------------------------------
		tnl::Vector3       _moveVelocity{};         // 移動ベクトル
		tnl::Vector3       _past_moveVelocity{};    // 前回の移動ベクトル
		tnl::Vector3       _centerOfGravity{};      // 重心座標
		tnl::Quaternion    _rotY{};
		tnl::Quaternion    _rotX{};
		tnl::Quaternion    _rotXZ{};
	};
}