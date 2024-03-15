#include "Player.h"
#include <sstream>
#include <iomanip>
#include "../../Bullet/Player/PlayerBullet.h"
#include "../../Sky/SkyBox.h"
#include "../../ScenePlay.h"
#include "../../Camera/FreeLookCamera.h"
#include "../../Character/Enemy/EnemyZakoBase.h"
#include "../../Character/Enemy/EnemyBossBase.h"
#include "../game/Manager/Enemy/EnemyManager.h"
#include "../../Pause/PauseMenu.h"
#include "../game/ScenePlay/Bullet/Player/Gunport.h"
#include "../game/Loader/CsvLoader.h"
#include "../../../InputFuncTable.h"

namespace inl {

	Shared<dxe::Particle> Player::_bombParticle;


	namespace {

		// プレイヤーステータス -------------------------------------------------
		const float           _forwardVelocity{ 150.0f };
		const float           _HP_POS_X{ 60 };
		const float           _HP_POS_Y{ 50 };
		const tnl::Vector3    _START_POSITION{ 0, 100, -300 };

		// ボム------------------------------------------------------------------
		const float           _BOMBEFFECT_TIME_LIMIT{ 3.0f };

		// 無敵時間--------------------------------------------------------------

		const float           _INVINCIBLE_TIME_LIMIT{ 3.0f };

		// カメラ固定における敵補足可能範囲--------------------------------------
		const float           _CAPTURABLE_RANGE_ENEMY{ 500.0f };

		// カメラ----------------------------------------------------------------
		const tnl::Vector3    _DEFAULT_CAMERA_POSITION{ 0, 100, -150 };
		const tnl::Vector3    _CAMERA_OFFSET{ 0, -50, 20 };

		// 視点操作--------------------------------------------------------------
		const float           _VIEWPOINT_LERP_RATE_H{ 0.05f };
		const float           _VIEWPOINT_LERP_RATE_V{ 0.01f };
		const float           _CAMERAMOVE_DELAY_RATE{ 0.05f };

		// プレイヤーとの距離のオフセット---------------------------------------
		const float          _DISTANCE_OFFSET{ 300.0f };

		// プレイヤー操作-------------------------------------------------------
		const float          _CENTROID_RADIUS{ 100 };  // 重心
		const float          _MASS{ 100 };             // 質量
		const float          _FRICTION{ 0.6f };        // 摩擦
	}


	Player::Player(const Shared<inl::FreeLookCamera> camera_ref) {

		_mesh = dxe::Mesh::CreateSphereMV(20, 10, 10, false);
		_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/prismatic-star.png"));
		_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
		_mesh->pos_ = _START_POSITION;

		InitPlayerStatus();

		_getDamageSE_hdl = LoadSoundMem("sound/se/getHit.mp3");

		_playerCamera = camera_ref;

		_bombParticle = std::make_shared<dxe::Particle>("particle/preset/bombEffect.bin");
		_playerGunport = std::make_shared<inl::Gunport>();
	}


	void Player::InitPlayerStatus() {

		_csvLoader = std::make_shared<CsvLoader>();
		auto status = _csvLoader->LoadPlayerStatus("csv/PlayerStatus.csv");

		_hp = status._hp;
		_MAX_HP = _hp;
		_at = status._at;
		_def = status._def;

		_collideSize = { 20, 20, 20 };
	}

	// 機能－-----------------------－-----------------------－-----------------------－-----------------------－---------------------
	bool Player::DecreaseHP(int damage) {

		if (_hp > 0) {

			if (!_isInvincible) {

				damage = max(damage, 1);

				_hp -= damage;
				return true;
			}

			return false;
		}
		else if (_hp <= 0) {
			// ゲームオーバー
			_hp = 0;
			inl::PauseMenu::_isShowPauseOption = true;
			return false;
		}
		return false;
	}


	void Player::PlayDamageHitSE() noexcept {

		PlaySoundMem(_getDamageSE_hdl, DX_PLAYTYPE_BACK, TRUE);
	}


	void Player::NormalizeCameraSpeed(const float speed) {

		tnl::Vector3 zero = { 0,0,0 };

		if ((_moveVelocity - zero).length() > 0.0f) {

			// ベクトル正規化
			_moveVelocity = _moveVelocity.Normalize(_moveVelocity) * speed;
		}
	}


	bool Player::IsEnemyInCapturableRange() {

		if (!_enemyManager_ref) return false;

		tnl::Vector3 enemyPos;
		AssignEnemyPosition(enemyPos);

		float dis_playerAndEnemy = (enemyPos - _mesh->pos_).length();

		if (dis_playerAndEnemy < _CAPTURABLE_RANGE_ENEMY) {
			return true;
		}

		return false;
	}


	void Player::AssignEnemyPosition(tnl::Vector3& enemy_pos) {

		int enemyZakoLeftCount = _enemyManager_ref->GetRemainingEnemyCount();
		auto zakoList = _enemyManager_ref->_enemyZakoList;
		auto it_zako = _enemyManager_ref->_itZako;

		auto bossList = _enemyManager_ref->_enemyBossList;
		auto it_boss = _enemyManager_ref->_itBoss;

		if (_enemyManager_ref->_enemyZakoList.empty()) {
			enemyZakoLeftCount = 0;
		}

		if (!bossList.empty()) {

			for (auto& boss : bossList) {
				if (boss->_isDead)
					enemyZakoLeftCount = 0;
			}
		}

		if (enemyZakoLeftCount != 0 && !zakoList.empty()) {

			// インデックスが置かれている敵のみイテレーターで取得
			if (_enemyIndex < zakoList.size()) {

				it_zako = std::next(zakoList.begin(), _enemyIndex);
				enemy_pos = (*it_zako)->_mesh->pos_;
			}
			else {
				_enemyIndex = 0;
				it_zako = zakoList.begin();
				enemy_pos = (*it_zako)->_mesh->pos_;
			}
		}
		else if (enemyZakoLeftCount == 0 && zakoList.empty() && !bossList.empty()) {

			it_boss = bossList.begin();

			if ((*it_boss)->_mesh != nullptr) {
				enemy_pos = (*it_boss)->_mesh->pos_;
			}
		}
	}


	const tnl::Vector3& Player::GetBulletMoveDirection() {

		tnl::Vector3 moveDir, enemyPos;

		if (_playerCamera->follow) {

			AssignEnemyPosition(enemyPos);
			moveDir = enemyPos - _mesh->pos_;
			moveDir.normalize();
		}
		else {
			moveDir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
		}

		return moveDir;
	}

	// 操作－-----------------------－-----------------------－-------------------------------------------------------------------
	void Player::ShotPlayerBullet() {

		if (IsShooting()) {

			tnl::Vector3 spawnPos = _mesh->pos_;
			tnl::Vector3 moveDir = GetBulletMoveDirection();

			_straightBullets_player.emplace_back(
				std::make_shared<inl::PlayerBullet>(spawnPos, moveDir, inl::PlayerBullet::COLOR::Red, 10.0f));
		}
	}


	void Player::ShotGunportBullet() {

		if (IsShooting()) {

			tnl::Vector3 moveDir = GetBulletMoveDirection();

			for (auto& blt : _gunportVec) {

				if (blt) {

					_straightBullets_player.emplace_back(
						std::make_shared<inl::PlayerBullet>(blt->_mesh->pos_, moveDir, inl::PlayerBullet::COLOR::White, 7.0f));
				}
			}
		}
	}


	void Player::ControlPlayerMoveByInput(const float deltaTime) {

		float speed = 4.f;

		// 減速  ゲームパッドの場合は△かＹ
		if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) || tnl::Input::IsPadDown(ePad::KEY_3)) {
			speed = 2.5f;
		}

		// 左方向
		if (InputFuncTable::IsButtonDown_LEFT() && !_playerCamera->follow) {

			_moveVelocity -= tnl::Vector3::TransformCoord({ 1.0f, 0, 0 }, _rotY);
			NormalizeCameraSpeed(speed);
		}

		// 右方向
		if (InputFuncTable::IsButtonDown_RIGHT() && !_playerCamera->follow) {

			_moveVelocity += tnl::Vector3::TransformCoord({ 1.0f, 0, 0 }, _rotY);
			NormalizeCameraSpeed(speed);
		}

		// 上方向　　//　このゲームパッドを有効にすると座標が上に飛んで行くバグがある
		if (tnl::Input::IsKeyDown(eKeys::KB_W) /*|| tnl::Input::IsPadDown(ePad::KEY_UP)*/) {

			_moveVelocity += tnl::Vector3::TransformCoord({ 0, 1.0f, 0.1f }, _rotY);
			_mesh->pos_.z += _playerMoveSpeed;

			NormalizeCameraSpeed(speed);
		}

		// 下方向
		if (InputFuncTable::IsButtonDown_DOWN()) {

			_moveVelocity -= tnl::Vector3::TransformCoord({ 0, 1.0f, 0.1f }, _rotY);
			_mesh->pos_.z -= _playerMoveSpeed;

			NormalizeCameraSpeed(speed);
		}

		// 前方向　ゲームパッドの場合はL1
		if (tnl::Input::IsKeyDown(eKeys::KB_SPACE) || tnl::Input::IsPadDown(ePad::KEY_4)) {

			_mesh->pos_ += tnl::Vector3::TransformCoord({ 0,0,2 }, _mesh->rot_) * _forwardVelocity * deltaTime;
		}
	}


	void Player::ChangeTarget_ByMouseWheel() {

		int wheel = tnl::Input::GetMouseWheel();
		auto zakoList = _enemyManager_ref->_enemyZakoList;

		// マウスホイールの入力に応じて敵のインデックスを増減
		if (wheel > 0) {
			_enemyIndex++;
			if (_enemyIndex >= zakoList.size()) {
				_enemyIndex = 0;
			}
		}
		else if (wheel < 0) {
			_enemyIndex--;
			if (_enemyIndex < 0) {
				_enemyIndex = static_cast<int>(zakoList.size() - 1);
			}
		}
	}


	void Player::ControlRotationByPadOrMouse() {

		// ゲームパッド
		if (!_playerCamera->follow) {

			tnl::Vector3 vel = tnl::Input::GetLeftStick();

			// 左右視点
			_rotY *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x));

			// 上下視点
			tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, _rotY);
			_rotX *= tnl::Quaternion::RotationAxis(
				tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * _VIEWPOINT_LERP_RATE_V));
		}

		// マウス
		if (!_playerCamera->follow
			&& tnl::Input::GetLeftStick().x == 0
			&& tnl::Input::GetLeftStick().y == 0
			&& tnl::Input::GetLeftStick().z == 0) {

			tnl::Vector3 vel = tnl::Input::GetMouseVelocity();

			// 左右視点
			_rotY *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x * _VIEWPOINT_LERP_RATE_H));

			// 上下視点
			tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, _rotY);
			_rotX *= tnl::Quaternion::RotationAxis(
				tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * _VIEWPOINT_LERP_RATE_V));
		}
	}


	void Player::AdjustPlayerVelocity() {

		// Time.deltaTimeのようなもの。これがないとプレイヤーが吹っ飛ぶ
		tnl::EasyAdjustObjectVelocity(_CENTROID_RADIUS, _MASS, _FRICTION, _past_moveVelocity, _moveVelocity, _centerOfGravity);

		_mesh->pos_ += _moveVelocity;

		if (_centerOfGravity.length() > FLT_EPSILON) {
			// 重心位置を利用して傾いてほしいアッパーベクトルを作成
			tnl::Vector3 upper = tnl::Vector3::Normalize({ _centerOfGravity.x, _CENTROID_RADIUS, _centerOfGravity.z });
			// 傾きの角度を計算
			float angle = upper.angle({ 0, 1, 0 });
			// 傾きベクトルと真上ベクトルの外積から回転軸を計算し、傾き角度を調整して回転クォータニオンを作成
			_rotXZ = tnl::Quaternion::RotationAxis(tnl::Vector3::Cross(upper, { 0, 1, 0 }), -(angle * 0.2f));
		}

		//// 最終的な姿勢
		//_mesh->rot_ = rot_y_ * rot_xz_;
		// ControlRotationByMouse で上下視点も使用する場合は↓を使う
		_mesh->rot_ = _rotY * _rotX * _rotXZ;
	}

	// カメラ－-----------------------－-----------------------－-----------------------－-----------------------－---------------------
	void Player::ActivateDarkSoulsCamera() {

		tnl::Vector3 playerPos = _mesh->pos_;
		tnl::Vector3 targetEnemyPos;

		AssignEnemyPosition(targetEnemyPos);

		float dis_player_enemy = (targetEnemyPos - playerPos).length();

		if (_playerCamera->follow) {

			ControlCameraWithEnemyFocus(playerPos, targetEnemyPos);
		}
		else {

			ControlCameraWithoutEnemyFocus();
		}
	}



	void Player::ControlCameraWithoutEnemyFocus()
	{
		tnl::Vector3 playerPos = _mesh->pos_;

		// 敵にカメラを固定しない場合
		_playerCamera->target_ = playerPos;
		_playerCamera->target_ -= _CAMERA_OFFSET;

		ControlRotationByPadOrMouse();

		// カメラの動きの遅延処理
		tnl::Vector3 fixPos =
			playerPos + tnl::Vector3::TransformCoord(_DEFAULT_CAMERA_POSITION, _mesh->rot_);
		_playerCamera->pos_ += (fixPos - _playerCamera->pos_) * _CAMERAMOVE_DELAY_RATE;

		// 追従ポインターOFF
		_playerCamera->isShowTargetPointer = false;
	}


	void Player::ControlCameraWithEnemyFocus(tnl::Vector3& playerPos, tnl::Vector3& targetEnemyPos)
	{
		// 追従ポインターON（描画）
		_playerCamera->isShowTargetPointer = true;

		ChangeTarget_ByMouseWheel();
		RenderFollowPointer();

		tnl::Vector3 tmp{};
		tmp.y = playerPos.y + 100;
		// カメラをプレイヤーと敵の中間地点に固定
		_playerCamera->target_ = (tmp + targetEnemyPos) / 2;

		tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0,1,0 }, _playerCamera->axis_y_angle_);
		tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0,0,1 }, q);
		tnl::Vector3 localAxis_x = tnl::Vector3::Cross({ 0,1,0 }, xz);
		q *= tnl::Quaternion::RotationAxis(localAxis_x, _playerCamera->axis_x_angle_);

		_mesh->rot_ = tnl::Quaternion::LookAt(playerPos, targetEnemyPos, localAxis_x);

		float y = 0;

		ControlPlayerMoveWithEnemyFocus(q, y);

		// カメラの動きの遅延処理
		tnl::Vector3 fixPos = playerPos + tnl::Vector3::TransformCoord({ 50, y, -140 }, _mesh->rot_);
		_playerCamera->pos_ += (fixPos - _playerCamera->pos_) * 0.1f;
	}



	void Player::ControlPlayerMoveWithEnemyFocus(tnl::Quaternion& q, float& y)
	{
		//左方向
		if (InputFuncTable::IsButtonDown_LEFT()) {

			tnl::Vector3 newPos =
				_playerCamera->target_ + tnl::Vector3::TransformCoord({ _DISTANCE_OFFSET, 0, _DISTANCE_OFFSET }, q);

			newPos.y = _mesh->pos_.y;

			y = 100;
			_mesh->pos_ = newPos;
			_playerCamera->axis_y_angle_ += tnl::ToRadian(2);
		}

		// 右方向
		if (InputFuncTable::IsButtonDown_RIGHT()) {

			tnl::Vector3 newPos =
				_playerCamera->target_ + tnl::Vector3::TransformCoord({ _DISTANCE_OFFSET, 0, _DISTANCE_OFFSET }, q);

			newPos.y = _mesh->pos_.y;

			y = -100;
			_mesh->pos_ = newPos;
			_playerCamera->axis_y_angle_ -= tnl::ToRadian(2);
		}
	}

	// ボム－-----------------------－-----------------------－-----------------------－-----------------------－-----------------------
	void Player::ValidateBombEffect() {

		float adaptRange = 200.f;

		float euclideanDistance = (_bombParticle->getPosition() - _mesh->pos_).length();

		if (euclideanDistance <= adaptRange) {
			ScenePlay::DeactivateAllEnemyBullets();
		}
	}


	void Player::UseBomb() {

		// マウス中央　ゲームパッドの場合はバツボタン、もしくはAボタンなど
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_MIDDLE) && _currentBomb_stockCount != 0 ||
			tnl::Input::IsPadDownTrigger(ePad::KEY_0) && _currentBomb_stockCount != 0) {

			_isTriggered_playersBombEffect = true;
			_currentBomb_stockCount--;
		}
	}


	void Player::InvalidateBombEffect(const float deltaTime) {

		if (_isTriggered_playersBombEffect) {
			_bombTimer += deltaTime;
			ValidateBombEffect();

			if (_bombTimer > _BOMBEFFECT_TIME_LIMIT) {
				_bombTimer = 0.0f;
				ScenePlay::DeactivateAllEnemyBullets();
				_isTriggered_playersBombEffect = false;
			}
		}
	}


	// 描画－-----------------------－-----------------------－-----------------------－-----------------------－-----------------------
	void Player::RenderFollowPointer() {

		tnl::Vector3 enemyPos;

		if (_playerCamera->isShowTargetPointer) {

			AssignEnemyPosition(enemyPos);
		}

		// スクリーン座標へ変換
		tnl::Vector3 screenPos = tnl::Vector3::ConvertToScreen
		(
			{ enemyPos.x, enemyPos.y, enemyPos.z }
			, DXE_WINDOW_WIDTH
			, DXE_WINDOW_HEIGHT
			, _playerCamera->view_
			, _playerCamera->proj_
		);

		// 追従ポインター描画
		DrawCircleAA(screenPos.x, screenPos.y, 5, 10, GetColor(255, 0, 0), 1, 1);
	}


	void Player::RenderGunport(const Shared<inl::FreeLookCamera> camera) {

		if (_gunportVec.empty()) return;

		for (auto& it : _gunportVec) {
			it->Render(camera);
		}
	}


	void Player::RenderBulletPowerRate() {

		std::ostringstream stream;
		stream << std::fixed << std::setprecision(2) << inl::PlayerBullet::_bulletPowerRate;

		std::string s = stream.str();
		DrawFormatString(1000, 140, -1, "Power:%s / 5.00", s.c_str());
	}


	void Player::RenderBombRemainCount() noexcept {

		std::string s = std::to_string(_currentBomb_stockCount);
		DrawFormatString(30, 80, -1, "Bomb:%s", s.c_str());
	}


	void Player::RenderPlayerHp() {

		if (_hp <= 0) return;

		float x2 = _HP_POS_X + 150;

		float gageWidth = abs(x2 - _HP_POS_X);

		float average = (_MAX_HP > 0) ? gageWidth / _MAX_HP : 0;

		x2 = _HP_POS_X + static_cast<int>(average * _hp);

		DrawBoxAA(_HP_POS_X, _HP_POS_Y, 210, 65, GetColor(150, 150, 150), true);
		DrawBoxAA(_HP_POS_X, _HP_POS_Y, x2, 65, GetColor(0, 255, 0), true);

		SetFontSize(16);
		DrawString(30, 50, "HP:", -1);
	}


	void Player::Render(const Shared<inl::FreeLookCamera> camera) {

		TriggerInvincible(camera);

		RenderBulletPowerRate();
		RenderPlayerHp();
		RenderGunport(camera);

		// ボムの残数
		SetFontSize(DEFAULT_FONT_SIZE);
		RenderBombRemainCount();

		for (auto& blt : _straightBullets_player) {
			blt->Render(camera);
		}
	}



	void Player::TriggerInvincible(const Shared<inl::FreeLookCamera>& camera)
	{
		if (_isInvincible) {

			if (static_cast<int>(_invincibleTimer * 10) % 3 == 0) {

				//　プレイヤー半透明化
				_mesh->setBlendMode(DX_BLENDMODE_ADD);
				_mesh->setAlpha(0.8f);
				_mesh->render(camera);
			}
		}
		else {
			_mesh->setBlendMode(DX_BLENDMODE_NOBLEND);
			_mesh->render(camera);
		}
	}


	// 更新－-----------------------－-----------------------－-----------------------－-----------------------－-----------------------
	void Player::UpdateStraightBullet(float deltaTime)
	{
		auto it_blt = _straightBullets_player.begin();

		while (it_blt != _straightBullets_player.end()) {

			(*it_blt)->Update(deltaTime);

			if (!(*it_blt)->_isActive) {
				it_blt = _straightBullets_player.erase(it_blt);
				continue;
			}
			it_blt++;
		}
	}


	void Player::WatchInvincibleTimer(const float deltaTime) noexcept {

		if (_isInvincible) {
			_invincibleTimer += deltaTime;

			if (_invincibleTimer >= _INVINCIBLE_TIME_LIMIT) {
				_invincibleTimer = 0.0f;
				_isInvincible = false;
			}
		}
	}


	void Player::UpdateGunport_DRY(Shared<inl::Gunport>& gunportVec, const tnl::Vector3 coords) {

		gunportVec->_mesh->pos_ = GetPos() + tnl::Vector3::TransformCoord({ coords }, _mesh->rot_);
		gunportVec->_mesh->rot_ = _mesh->rot_;
	}


	void Player::UpdateGunport() {

		if (_gunportVec.empty()) return;

		const tnl::Vector3 coords[] = {
		{  0, -25, -20},
		{-25,   0, -20},
		{ 25,   0, -20},
		{-15, -15, -20},
		{ 15, -15, -20}
		};


		switch (_gunportVec.size())
		{
		case 1:
		{
			UpdateGunport_DRY(_gunportVec[0], { coords[0] });
			break;
		}
		case 2:
		{
			UpdateGunport_DRY(_gunportVec[0], { coords[1] });
			UpdateGunport_DRY(_gunportVec[1], { coords[2] });
			break;
		}
		case 3:
		{
			UpdateGunport_DRY(_gunportVec[0], { coords[1] });
			UpdateGunport_DRY(_gunportVec[1], { coords[2] });
			UpdateGunport_DRY(_gunportVec[2], { coords[0] });
			break;
		}
		case 4:
		{
			UpdateGunport_DRY(_gunportVec[0], { coords[1] });
			UpdateGunport_DRY(_gunportVec[1], { coords[2] });
			UpdateGunport_DRY(_gunportVec[2], { coords[3] });
			UpdateGunport_DRY(_gunportVec[3], { coords[4] });
			break;
		}
		case 5:
		{
			UpdateGunport_DRY(_gunportVec[0], { coords[1] });
			UpdateGunport_DRY(_gunportVec[1], { coords[2] });
			UpdateGunport_DRY(_gunportVec[2], { coords[3] });
			UpdateGunport_DRY(_gunportVec[3], { coords[4] });
			UpdateGunport_DRY(_gunportVec[4], { coords[0] });
			break;
		}
		}
	}


	void Player::Update(const float deltaTime) {

		ActivateDarkSoulsCamera();

		ControlPlayerMoveByInput(deltaTime);
		AdjustPlayerVelocity();
		WatchInvincibleTimer(deltaTime);

		// カメラを敵に固定するフラグを反転
		// マウス右　ゲームパッドの場合はR1（RB)
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) || tnl::Input::IsPadDownTrigger(ePad::KEY_5)) {

			if (IsEnemyInCapturableRange() || _playerCamera->follow)
				_playerCamera->follow = !_playerCamera->follow;
		}

		_playerCamera->Update(deltaTime);

		ShotPlayerBullet();
		ShotGunportBullet();
		UpdateStraightBullet(deltaTime);

		_playerGunport->ManageGunportCount(_gunportVec);
		UpdateGunport();

		UseBomb();
		InvalidateBombEffect(deltaTime);
	}
}