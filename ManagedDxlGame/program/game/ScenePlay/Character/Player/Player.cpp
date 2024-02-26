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
#include "Player.h"


Shared<dxe::Particle> Player::_bombParticle;

Player::Player(const Shared<FreeLookCamera> camera_ref) {

	_mesh = dxe::Mesh::CreateSphereMV(20, 10, 10, false);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/prismatic-star.png"));
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
	_mesh->pos_ = _START_POSITION;

	InitPlayerStatus(ScenePlay::GetGameDifficulty());

	_getDamageSE_hdl = LoadSoundMem("sound/se/getHit.mp3");

	_mainCamera_ref = camera_ref;

	_bombParticle = std::make_shared<dxe::Particle>("particle/preset/bombEffect.bin");
	_playerGunport = std::make_shared<Gunport>();
}


void Player::InitPlayerStatus(const std::string difficulty) {

	_csvLoader = std::make_shared<CsvLoader>(difficulty);
	auto status = _csvLoader->LoadPlayerStatus("csv/PlayerStatus.csv");

	_hp = status._hp;
	_MAX_HP = _hp;
	_at = status._at;
	_def = status._def;
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
		PauseMenu::_isShowPauseOption = true;
		return false;
	}
	return false;
}


void Player::PlayDamageHitSE() {

	PlaySoundMem(_getDamageSE_hdl, DX_PLAYTYPE_BACK, TRUE);
}


void Player::NormalizeCameraSpeed(const float speed) {

	tnl::Vector3 zero = { 0,0,0 };

	if ((move_vel_ - zero).length() > 0.0f) {

		// ベクトル正規化
		move_vel_ = move_vel_.Normalize(move_vel_) * speed;
	}
}


bool Player::IsEnemyInCapturableRange() {

	if (!_enemyManager_ref) return false;

	tnl::Vector3 enemyPos;
	AssignEnemyPosition(enemyPos);

	float dis_playerAndEnemy = (enemyPos - _mesh->pos_).length();

	if (dis_playerAndEnemy < _capturableEnemyRange) {
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

	if (_mainCamera_ref->follow) {

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
void Player::ShotPlayerBullet(const float deltaTime) {

	if (IsShooting()) {

		tnl::Vector3 spawnPos = _mesh->pos_;
		tnl::Vector3 moveDir = GetBulletMoveDirection();

		_straightBullets_player.emplace_back(
			std::make_shared<PlayerBullet>(spawnPos, moveDir, PlayerBullet::COLOR::Red, 10.0f));
	}
}


void Player::ShotGunportBullet() {

	if (IsShooting()) {

		tnl::Vector3 moveDir = GetBulletMoveDirection();

		for (auto& blt : _gunportVec) {

			if (blt) {

				_straightBullets_player.emplace_back(
					std::make_shared<PlayerBullet>(blt->_mesh->pos_, moveDir, PlayerBullet::COLOR::White, 7.0f));
			}
		}
	}
}


void Player::ControlPlayerMoveByInput(const float delta_time) {

	float speed = 4.f;

	// 減速  ゲームパッドの場合は△かＹ
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) || tnl::Input::IsPadDown(ePad::KEY_3)) {
		speed = 2.5f;
	}

	// 左方向
	if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT) && !_mainCamera_ref->follow) {

		move_vel_ -= tnl::Vector3::TransformCoord({ 1.0f, 0, 0 }, rot_y_);
		NormalizeCameraSpeed(speed);
	}

	// 右方向
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT) && !_mainCamera_ref->follow) {

		move_vel_ += tnl::Vector3::TransformCoord({ 1.0f, 0, 0 }, rot_y_);
		NormalizeCameraSpeed(speed);
	}

	// 上方向　　//　このゲームパッドを有効にすると座標が上に飛んで行くバグがある
	if (tnl::Input::IsKeyDown(eKeys::KB_W) /*|| tnl::Input::IsPadDown(ePad::KEY_UP)*/) {

		move_vel_ += tnl::Vector3::TransformCoord({ 0, 1.0f, 0.1f }, rot_y_);
		_mesh->pos_.z += _moveSpeed;

		NormalizeCameraSpeed(speed);
	}

	// 下方向
	if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsPadDown(ePad::KEY_DOWN)) {

		move_vel_ -= tnl::Vector3::TransformCoord({ 0, 1.0f, 0.1f }, rot_y_);
		_mesh->pos_.z -= _moveSpeed;

		NormalizeCameraSpeed(speed);
	}

	// 前方向　ゲームパッドの場合はL1
	if (tnl::Input::IsKeyDown(eKeys::KB_SPACE) || tnl::Input::IsPadDown(ePad::KEY_4)) {

		_mesh->pos_ += tnl::Vector3::TransformCoord({ 0,0,2 }, _mesh->rot_) * _forwardVelocity * delta_time;
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
	if (!_mainCamera_ref->follow) {

		tnl::Vector3 vel = tnl::Input::GetLeftStick();

		// 左右視点
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x));

		// 上下視点
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_y_);
		rot_x_ *= tnl::Quaternion::RotationAxis(
			tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * _viewpoint_lerpRate_v));
	}

	// マウス
	if (!_mainCamera_ref->follow
		&& tnl::Input::GetLeftStick().x == 0
		&& tnl::Input::GetLeftStick().y == 0
		&& tnl::Input::GetLeftStick().z == 0) {

		tnl::Vector3 vel = tnl::Input::GetMouseVelocity();

		// 左右視点
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x * _viewpoint_lerpRate_h));

		// 上下視点
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_y_);
		rot_x_ *= tnl::Quaternion::RotationAxis(
			tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * _viewpoint_lerpRate_v));
	}
}


void Player::AdjustPlayerVelocity() {

	// Time.deltaTimeのようなもの。これがないとプレイヤーが吹っ飛ぶ
	tnl::EasyAdjustObjectVelocity(centroid_radius_, mass_, friction_, past_move_vel_, move_vel_, center_of_gravity_);

	_mesh->pos_ += move_vel_;

	if (center_of_gravity_.length() > FLT_EPSILON) {
		// 重心位置を利用して傾いてほしいアッパーベクトルを作成
		tnl::Vector3 upper = tnl::Vector3::Normalize({ center_of_gravity_.x, centroid_radius_, center_of_gravity_.z });
		// 傾きの角度を計算
		float angle = upper.angle({ 0, 1, 0 });
		// 傾きベクトルと真上ベクトルの外積から回転軸を計算し、傾き角度を調整して回転クォータニオンを作成
		rot_xz_ = tnl::Quaternion::RotationAxis(tnl::Vector3::Cross(upper, { 0, 1, 0 }), -(angle * 0.2f));
	}

	//// 最終的な姿勢
	//_mesh->rot_ = rot_y_ * rot_xz_;
	// ControlRotationByMouse で上下視点も使用する場合は↓を使う
	_mesh->rot_ = rot_y_ * rot_x_ * rot_xz_;
}

// カメラ－-----------------------－-----------------------－-----------------------－-----------------------－---------------------
void Player::ActivateDarkSoulsCamera() {

	tnl::Vector3 playerPos = _mesh->pos_;
	tnl::Vector3 targetEnemyPos;

	AssignEnemyPosition(targetEnemyPos);

	float dis_player_enemy = (targetEnemyPos - playerPos).length();

	if (_mainCamera_ref->follow) {

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
	_mainCamera_ref->target_ = playerPos;
	_mainCamera_ref->target_ -= _CAMERA_OFFSET;

	ControlRotationByPadOrMouse();

	// カメラの動きの遅延処理
	tnl::Vector3 fixPos =
		playerPos + tnl::Vector3::TransformCoord(_DEFAULT_CAMERA_POSITION, _mesh->rot_);
	_mainCamera_ref->pos_ += (fixPos - _mainCamera_ref->pos_) * _cameraMove_delayRate;

	// 追従ポインターOFF
	_mainCamera_ref->isShowTargetPointer = false;
}


void Player::ControlCameraWithEnemyFocus(tnl::Vector3& player_pos, tnl::Vector3& target_enemy_pos)
{
	// 追従ポインターON（描画）
	_mainCamera_ref->isShowTargetPointer = true;

	ChangeTarget_ByMouseWheel();
	RenderFollowPointer();

	tnl::Vector3 tmp{};
	tmp.y = player_pos.y + 100;
	// カメラをプレイヤーと敵の中間地点に固定
	_mainCamera_ref->target_ = (tmp + target_enemy_pos) / 2;

	tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0,1,0 }, _mainCamera_ref->axis_y_angle_);
	tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0,0,1 }, q);
	tnl::Vector3 localAxis_x = tnl::Vector3::Cross({ 0,1,0 }, xz);
	q *= tnl::Quaternion::RotationAxis(localAxis_x, _mainCamera_ref->axis_x_angle_);

	_mesh->rot_ = tnl::Quaternion::LookAt(player_pos, target_enemy_pos, localAxis_x);

	float y = 0;

	//左方向
	if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT)) {

		tnl::Vector3 newPos =
			_mainCamera_ref->target_ + tnl::Vector3::TransformCoord({ _distance_offset, 0, _distance_offset }, q);

		newPos.y = _mesh->pos_.y;

		y = 100;
		_mesh->pos_ = newPos;
		_mainCamera_ref->axis_y_angle_ += tnl::ToRadian(2);
	}

	// 右方向
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT)) {

		tnl::Vector3 newPos =
			_mainCamera_ref->target_ + tnl::Vector3::TransformCoord({ _distance_offset, 0, _distance_offset }, q);

		newPos.y = _mesh->pos_.y;

		y = -100;
		_mesh->pos_ = newPos;
		_mainCamera_ref->axis_y_angle_ -= tnl::ToRadian(2);
	}

	// カメラの動きの遅延処理
	tnl::Vector3 fixPos = player_pos + tnl::Vector3::TransformCoord({ 50, y, -140 }, _mesh->rot_);
	_mainCamera_ref->pos_ += (fixPos - _mainCamera_ref->pos_) * 0.1f;
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


void Player::InvalidateBombEffect(const float delta_time) {

	if (_isTriggered_playersBombEffect) {
		_bombTimer += delta_time;
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

	if (_mainCamera_ref->isShowTargetPointer) {

		AssignEnemyPosition(enemyPos);
	}

	// スクリーン座標へ変換
	tnl::Vector3 screenPos = tnl::Vector3::ConvertToScreen
	(
		{ enemyPos.x, enemyPos.y, enemyPos.z }
		, DXE_WINDOW_WIDTH
		, DXE_WINDOW_HEIGHT
		, _mainCamera_ref->view_
		, _mainCamera_ref->proj_
	);

	// 追従ポインター描画
	DrawCircleAA(screenPos.x, screenPos.y, 5, 10, GetColor(255, 0, 0), 1, 1);
}


void Player::RenderGunport(const Shared<FreeLookCamera> camera) {

	if (_gunportVec.empty()) return;

	for (auto& it : _gunportVec) {
		it->Render(camera);
	}
}


void Player::RenderBulletPowerRate() {

	std::ostringstream stream;
	stream << std::fixed << std::setprecision(2) << PlayerBullet::_bulletPowerRate;

	std::string s = stream.str();
	DrawFormatString(1000, 140, -1, "Power:%s / 5.00", s.c_str());
}


void Player::RenderBombRemainCount() {

	std::string s = std::to_string(_currentBomb_stockCount);
	DrawFormatString(30, 80, -1, "Bomb:%s", s.c_str());
}


void Player::RenderPlayerHp() {

	if (_hp <= 0) return;

	float x2 = _hp_posX + 150;

	float gageWidth = abs(x2 - _hp_posX);

	float average = (_MAX_HP > 0) ? gageWidth / _MAX_HP : 0;

	x2 = _hp_posX + static_cast<int>(average * _hp);

	DrawBoxAA(_hp_posX, _hp_posY, 210, 65, GetColor(150, 150, 150), true);
	DrawBoxAA(_hp_posX, _hp_posY, x2, 65, GetColor(0, 255, 0), true);

	SetFontSize(16);
	DrawString(30, 50, "HP:", -1);
}


void Player::Render(const Shared<FreeLookCamera> camera) {

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



void Player::TriggerInvincible(const Shared<FreeLookCamera>& camera)
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
void Player::UpdateStraightBullet(float delta_time)
{
	auto it_blt = _straightBullets_player.begin();

	while (it_blt != _straightBullets_player.end()) {

		(*it_blt)->Update(delta_time);

		if (!(*it_blt)->_isActive) {
			it_blt = _straightBullets_player.erase(it_blt);
			continue;
		}
		it_blt++;
	}
}


void Player::WatchInvincibleTimer(const float delta_time) {

	if (_isInvincible) {
		_invincibleTimer += delta_time;

		if (_invincibleTimer >= _INVINCIBLE_TIME_LIMIT) {
			_invincibleTimer = 0.0f;
			_isInvincible = false;
		}
	}
}


void Player::UpdateGunport_DRY(Shared<Gunport>& gunportVec, const tnl::Vector3 coords) {

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


void Player::Update(const float delta_time) {

	ActivateDarkSoulsCamera();

	ControlPlayerMoveByInput(delta_time);
	AdjustPlayerVelocity();
	WatchInvincibleTimer(delta_time);

	// カメラを敵に固定するフラグを反転
	// マウス右　ゲームパッドの場合はR1（RB)
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) || tnl::Input::IsPadDownTrigger(ePad::KEY_5)) {
		if (IsEnemyInCapturableRange() || _mainCamera_ref->follow)
			_mainCamera_ref->follow = !_mainCamera_ref->follow;
	}

	_mainCamera_ref->Update(delta_time);

	ShotPlayerBullet(delta_time);
	ShotGunportBullet();
	UpdateStraightBullet(delta_time);

	_playerGunport->ManageGunportCount(_gunportVec);
	UpdateGunport();

	UseBomb();
	InvalidateBombEffect(delta_time);
}