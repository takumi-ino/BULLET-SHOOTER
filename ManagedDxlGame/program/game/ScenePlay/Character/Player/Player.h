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

		// 初期化--------------------------------------------------------------------------------------------
		void InitBombCount(const int count) { _currentBomb_stockCount = count; }			// ボムカウント

		// ゲッター--------------------------------------------------------------------------------------------
		const int GetHP() const noexcept { return ScenePlay::GetInstance()->_player->_hp; }          // HP
		const int GetMaxHP() const noexcept { return ScenePlay::GetInstance()->_player->_MAX_HP; }	 // 最大HP
		const int GetAT() const noexcept { return ScenePlay::GetInstance()->_player->_at; }			 // 攻撃力
		const int GetDEF() const noexcept { return ScenePlay::GetInstance()->_player->_def; }		 // 防御力
		const tnl::Vector3 GetPos() const { return ScenePlay::GetInstance()->_player->_mesh->pos_; } // 位置

		// ボムエフェクト
		const bool GetIsTriggeredBombEffect() const noexcept { return _isTriggered_playersBombEffect; }

		// セッター------------------------------------------------------------------------------------------------
		void SetHP(int val) { ScenePlay::GetInstance()->_player->_hp = val; }						 // HP

		void ResetPositionAndRotation() {

			ScenePlay::GetInstance()->_player->_mesh->pos_ = { 0, 100, -300 };
			ScenePlay::GetInstance()->_player->_mesh->rot_ = { 0, 0,0,0 };
		}

		// ステータス更新-----------------------------------------------------------------------------------------
		bool DecreaseHP(int damage);																 // HP減少

		void HealHP(int heal) {		 // HP回復
			ScenePlay::GetInstance()->_player->_hp += heal;
			PlaySoundMem(_healSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddAT(int val) {		 // 攻撃力上昇
			ScenePlay::GetInstance()->_player->_at += val;
			PlaySoundMem(_enhanceSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddDEF(int val) {		// 防御力上昇
			ScenePlay::GetInstance()->_player->_def += val;
			PlaySoundMem(_enhanceSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddSpeed(float val) {  // スピード上昇
			ScenePlay::GetInstance()->_player->_playerMoveSpeed += val;
			PlaySoundMem(_enhanceSE_hdl, DX_PLAYTYPE_BACK, TRUE);
		}
		void AddBombStockCount() {  // ボムカウント増加
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

		// カメラ-------------------------------------------------------------------------------------------------
		void CameraTrigger();

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

		// プレイヤー---------------------------------------------------------------------------------------------

		// HP、AT、DEFなどのステータス初期化
		void InitPlayerStatus(std::vector<std::vector<tnl::CsvCell>>);

		// 無敵時間
		void WatchInvincibleTimer(const float deltaTime) noexcept;
		void TriggerInvincible();

		// HP 
		void RenderPlayerHp(const int color);

		// 移動 
		void ControlPlayerMoveByInput(const float deltaTime);  // 移動操作
		void AdjustPlayerVelocity();                           // 速度調整
		void ControlRotationByPadOrMouse();                    // 視点操作

		// 弾 -------------------------------------------------------------------------------------------------------
		void ShotPlayerBullet(const tnl::Vector3 spawnPos, const tnl::Vector3 moveDir); // 弾を撃つ処理
		void UpdateStraightBullet(const float deltaTime);      // プレイヤーの弾の更新処理
		void UpdateGunportBullet(const float deltaTime);       // 連装砲の

		const tnl::Vector3& GetBulletMoveDirection();          // 弾の移動方向取得

		bool IsShooting() noexcept {
			return
				tnl::Input::IsMouseDown(eMouse::LEFT) ||
				tnl::Input::IsPadDown(ePad::KEY_1);
		}

		// 連装砲-----------------------------------------------------------------------------------------------------
		void ShotGunportBullet(tnl::Vector3 spawnPos, const tnl::Vector3 moveDir);                                  // 発射処理
		void RenderBulletPowerRate(const int color);               // 弾の現在のパワーを表示
		void RenderGunport();									   // 描画
		void UpdateGunport();                                      // 更新

		// DRY原則につき、同じ処理をまとめて実行
		void UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords);

		// ボム-------------------------------------------------------------------------------------------------------
		void UseBomb();                                            // 使用
		void ValidateBombEffect();                                 // エフェクト有効化
		void InvalidateBombEffect(const float deltaTime);          // エフェクト無効化
		void RenderBombRemainCount(const int color) noexcept;      // ボム残数描画

		// 敵位置-----------------------------------------------------------------------------------------------------
		void ChangeTarget_ByMouseWheel();                          // マウスホイールでターゲット変更処理
		void RenderFollowPointer();                                // ターゲット位置に合わせて描画するポインター
		void AssignEnemyPosition(tnl::Vector3& enemyPos);          // ローカル変数に敵座標位置を割り当てる
		bool IsEnemyInCapturableRange();                           // 敵がカメラ固定機能が使用可能な範囲内にいるか
		// -----------------------------------------------------------------------------------------------------
		void RenderPlayerParticle();

	public:

		std::vector<Shared<PlayerBullet>> _straightBullet{};			 // プレイヤーの直行弾

		std::vector<std::vector<Shared<PlayerBullet>>> _gunportBullet{};				 // 連装砲用の弾

		std::vector<Shared<Gunport>>    _gunportVec{};                   // 連装砲（本体）ベクター

		static Shared<dxe::Particle>    _bombParticle;                   // ボムパーティクル

		Shared<dxe::Particle>			_playerParticle = nullptr;       // プレイヤーの前方に配置するパーティクル

	private:

		Shared<Gunport>                 _playerGunport = nullptr;	     // 連装砲

		Shared<CsvLoader>               _csvLoader = nullptr;			 // CSV

	public:

		bool               _isInvincible{ false };    // 透明化フラグ

	private:

		// プレイヤーステータス -------------------------------------------------
		int                _hp{};                       // HP。CSVからロード
		float              _playerMoveSpeed{ 0.4f };    // スピード

		// ボム------------------------------------------------------------------
		int                _currentBomb_stockCount{};         // ボムカウント
		float              _bombTimer{};					  // ボムタイマー
		bool               _isTriggered_playersBombEffect{};  // ボムフラグ

		// 無敵時間--------------------------------------------------------------
		float              _invincibleTimer{};        // 透明化タイマー

		// SE -----------------------------------------------------------
		int				   _getBombSE_hdl{};
		int                _getDamageSE_hdl{};        // ダメージ音SE
		int				   _healSE_hdl{};
		int				   _enhanceSE_hdl{};
		static int		   _increaseGunport;
		static int		   _getScoreSE_hdl;

		int				   _shotSe{};

		// 敵情報　--------------------------------------------------------------
		// 　　　　　通常エネミー参照インデックス（レーダーポインター使用時に使用）
		int                _enemyIndex{};

		// 銃　--------------------------------------------------------------
		int				  _bulletFireInterval{4};
		int				  _straightBulletCount{};
		int				  _gunportBulletCount[5];


		// 弾の連射をするために必要となるインデックス
		int				  _bulletOrderIdx{};
		int				  _gunportBulletOrderIdx[5];


		// プレイヤー操作-------------------------------------------------------
		tnl::Vector3       _moveVelocity{};           // 移動ベクトル
		tnl::Vector3       _past_moveVelocity{};      // 前回の移動ベクトル
		tnl::Vector3       _centerOfGravity{};        // 重心座標
		tnl::Quaternion    _rotY{};                   // Y軸回転
		tnl::Quaternion    _rotX{};				      // X軸回転
		tnl::Quaternion    _rotXZ{};			   	  // XZ軸回転
	};
}