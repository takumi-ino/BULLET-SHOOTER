#include "../../Bullet/Player/PlayerBullet.h"
#include "../../Sky/SkyBox.h"
#include "../../ScenePlay.h"
#include "../../Camera/FreeLookCamera.h"
#include "../../Character/Enemy/EnemyZakoBase.h"
#include "../../Character/Enemy/EnemyBossBase.h"
#include "../../Pause/PauseMenu.h"
#include "Player.h"


int Player::_hp;
int Player::_MAX_HP;
int Player::_at;
int Player::_def;
int Player::_current_bomb_stock_count;

float Player::_moveSpeed;
float Player::_invincible_timer;
float Player::_bomb_timer;

bool Player::_isInvincible;

Shared<dxe::Particle> Player::_bomb_particle;


Player::Player(const Shared<FreeLookCamera> camera_ref) {

	_mesh = dxe::Mesh::CreateSphereMV(20, 10, 10, false);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/prismatic-star.png"));
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
	_mesh->pos_ = { 0, 100, -300 };

	_bomb_particle = std::make_shared<dxe::Particle>("particle/preset/bombEffect.bin");

	_collide_size = { 20, 20, 20 };

	_hp = 8;
	_MAX_HP = _hp;
	_at = 2;
	_def = 4;
	_moveSpeed = 0.4f;

	_getDamageSE_hdl = LoadSoundMem("sound/se/getHit.mp3");

	_mainCamera_ref = camera_ref;
}

// 機能－-----------------------－-----------------------－-----------------------－-----------------------－---------------------
void Player::SetEnemyZakoListRef(const std::vector<Shared<EnemyZakoBase>>& enemies_list_ref) {
	_enemyZako_list_ref = enemies_list_ref;
}



void Player::SetEnemyBossListRef(const std::vector<Shared<EnemyBossBase>>& enemyBoss_list_ref) {
	_enemyBoss_list_ref = enemyBoss_list_ref;
}



void Player::EraseEnemyZakoListRef(Shared<EnemyZakoBase>& enemy_list_ref) {

	for (auto it = _enemyZako_list_ref.begin(); it != _enemyZako_list_ref.end(); it++) {

		if (*it == enemy_list_ref) {
			_enemyZako_list_ref.erase(it);
			break;
		}
	}
}


void Player::EraseEnemyBossListRef(Shared<EnemyBossBase>& enemyBoss_ref) {

	for (auto it = _enemyBoss_list_ref.begin(); it != _enemyBoss_list_ref.end(); it++) {

		if (*it == enemyBoss_ref) {
			_enemyBoss_list_ref.erase(it);
			break;
		}
	}
}


bool Player::DecreaseHP(const int damage) {

	if (_hp > 0) {

		if (!_isInvincible) {
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
}

int Player::_getDamageSE_hdl;

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


const tnl::Vector3 Player::GetTargetsScreenCoordinates(const float& x, const float& y, const float& z) {

	tnl::Vector3 temp = { x,y,z };

	return temp;
}


bool Player::IsEnemyInCapturableRange() {

	tnl::Vector3 player_pos = _mesh->pos_;

	if (!_enemyZako_list_ref.empty()) {

		for (auto it_enemyList = _enemyZako_list_ref.begin(); it_enemyList != _enemyZako_list_ref.end(); it_enemyList++) {

			if (!(*it_enemyList)) break;

			tnl::Vector3 target_enemy_pos = (*it_enemyList)->_mesh->pos_;
			float dis_player_enemy = (target_enemy_pos - player_pos).length();
			if (dis_player_enemy < 500) {
				return true;
			}
		}
	}
	else if (_enemyZako_list_ref.empty() && !_enemyBoss_list_ref.empty()) {

		auto it_bossList = _enemyBoss_list_ref.begin();

		if ((*it_bossList)) {

			tnl::Vector3 target_enemy_pos = (*it_bossList)->_mesh->pos_;
			float dis_player_enemy = (target_enemy_pos - player_pos).length();
			if (dis_player_enemy < 500) {
				return true;
			}
		}
	}

	return false;
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

	// 最終的な姿勢
	_mesh->rot_ = rot_y_ * rot_xz_;
	// ControlRotationByMouse で上下視点も使用する場合は↓を使う
	_mesh->rot_ = rot_y_ * rot_x_ * rot_xz_;
}


// 操作－-----------------------－-----------------------－-----------------------－-----------------------－---------------------
void Player::ShotPlayerBullet() {

	tnl::Vector3 moveDir;

	if (_mainCamera_ref->follow) {

		std::vector<Shared<EnemyZakoBase>>::iterator it_zako;
		std::vector<Shared<EnemyBossBase>>::iterator it_boss;
		tnl::Vector3 enemyPos;

		if (!_enemyZako_list_ref.empty()) {

			// インデックスが置かれている敵のみイテレーターで取得
			it_zako = std::next(_enemyZako_list_ref.begin(), _enemy_index);
			enemyPos = (*it_zako)->_mesh->pos_;
		}
		else if (_enemyZako_list_ref.empty() && !_enemyBoss_list_ref.empty()) {
			it_boss = _enemyBoss_list_ref.begin();
			enemyPos = (*it_boss)->_mesh->pos_;
		}

		moveDir = enemyPos - _mesh->pos_;
		moveDir.normalize();
	}
	else {
		moveDir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
	}


	if (tnl::Input::IsMouseDown(eMouse::LEFT) || tnl::Input::IsPadDown(ePad::KEY_1)) {

		tnl::Vector3 spawn_pos = _mesh->pos_;

		_straight_bullets_player.emplace_back(std::make_shared<PlayerBullet>(spawn_pos, moveDir, _player_ref));
	}
}


void Player::ControlPlayerMoveByInput(const float delta_time) {

	float speed = 4.f;

	// 減速  ゲームパッドの場合は△かＹ
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) || tnl::Input::IsPadDown(ePad::KEY_3))
		speed = 2.5f;


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

		_mesh->pos_ += tnl::Vector3::TransformCoord(
			{ 0,0,2 }, _mesh->rot_) * _forward_velocity * delta_time * 150;
	}
}


void Player::ChangeTarget_ByMouseWheel() {

	int wheel = tnl::Input::GetMouseWheel();

	// マウスホイールの入力に応じて敵のインデックスを増減
	if (wheel > 0) {
		_enemy_index++;
		if (_enemy_index >= _enemyZako_list_ref.size()) {
			_enemy_index = 0;
		}
	}
	else if (wheel < 0) {
		_enemy_index--;
		if (_enemy_index < 0) {
			_enemy_index = _enemyZako_list_ref.size() - 1;
		}
	}
}


void Player::ControlRotationByPadOrMouse() {

	// ゲームパッド
	if (!_mainCamera_ref->follow) {

		tnl::Vector3 vel = tnl::Input::GetLeftStick();

		// 左右視点
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x * 1));

		// 上下視点
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_y_);
		rot_x_ *= tnl::Quaternion::RotationAxis(tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * 0.01f));
	}

	// マウス
	if (!_mainCamera_ref->follow
		&& tnl::Input::GetLeftStick().x == 0
		&& tnl::Input::GetLeftStick().y == 0
		&& tnl::Input::GetLeftStick().z == 0) {

		tnl::Vector3 vel = tnl::Input::GetMouseVelocity();

		// 左右視点
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x * 0.05f));

		// 上下視点
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_y_);
		rot_x_ *= tnl::Quaternion::RotationAxis(tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * 0.01f));
	}
}


// カメラ－-----------------------－-----------------------－-----------------------－-----------------------－---------------------
void Player::AssignTargetEnemy_ForDarkSoulsCamera(tnl::Vector3& target_enemy_pos) {

	if (!_enemyZako_list_ref.empty()) {

		for (auto it_zakoList_ref = _enemyZako_list_ref.begin(); it_zakoList_ref != _enemyZako_list_ref.end(); it_zakoList_ref++) {

			if ((*it_zakoList_ref)) {
				target_enemy_pos = (*it_zakoList_ref)->_mesh->pos_;
			}
		}
	}
	if (_enemyZako_list_ref.empty() && !_enemyBoss_list_ref.empty()) {

		auto it_bossList_ref = _enemyBoss_list_ref.begin();

		if (*it_bossList_ref)
			target_enemy_pos = (*it_bossList_ref)->_mesh->pos_;
	}
}


void Player::ActivateDarkSoulsCamera() {

	tnl::Vector3 player_pos = _mesh->pos_;
	tnl::Vector3 target_enemy_pos;

	AssignTargetEnemy_ForDarkSoulsCamera(target_enemy_pos);

	float dis_player_enemy = (target_enemy_pos - player_pos).length();

	if (_mainCamera_ref->follow) {

		ControlCameraWithEnemyFocus(player_pos, target_enemy_pos);
	}
	else {

		ControlCameraWithoutEnemyFocus();
	}
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

	//_player_ref->_mainCamera_ref->ControlFreeLookCamera(_mainCamera_ref, _player_ref->_mesh, _mainCamera_ref->target_);
	// カメラの位置 = プレイヤー座標 + (normalized(プレイヤー座標 - 敵座標) * カメラとプレイヤーの差分)
	// ただし、単純にプレイヤーの座標を使うとプレイヤーとエネミーが重なってしまうため、差分を使う

	tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0,1,0 }, _mainCamera_ref->axis_y_angle_);
	tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0,0,1 }, q);
	tnl::Vector3 local_axis_x = tnl::Vector3::Cross({ 0,1,0 }, xz);
	q *= tnl::Quaternion::RotationAxis(local_axis_x, _mainCamera_ref->axis_x_angle_);

	_mesh->rot_ = tnl::Quaternion::LookAt(player_pos, target_enemy_pos, local_axis_x);

	float offset = -300;

	float y = 0;

	//左方向
	if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT)) {

		tnl::Vector3 new_pos = _mainCamera_ref->target_ + tnl::Vector3::TransformCoord({ offset, 0, offset }, q);
		new_pos.y = _mesh->pos_.y;

		y = 100;
		_mesh->pos_ = new_pos;
		_mainCamera_ref->axis_y_angle_ += tnl::ToRadian(2);
	}

	// 右方向
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT)) {

		tnl::Vector3 new_pos = _mainCamera_ref->target_ + tnl::Vector3::TransformCoord({ offset, 0, offset }, q);
		new_pos.y = _mesh->pos_.y;

		y = -100;
		_mesh->pos_ = new_pos;
		_mainCamera_ref->axis_y_angle_ -= tnl::ToRadian(2);
	}

	// カメラの動きの遅延処理
	tnl::Vector3 fix_pos = player_pos + tnl::Vector3::TransformCoord({ 50, y, -140 }, _mesh->rot_);
	_mainCamera_ref->pos_ += (fix_pos - _mainCamera_ref->pos_) * 0.1f;
}



void Player::ControlCameraWithoutEnemyFocus()
{
	tnl::Vector3 player_pos = _mesh->pos_;

	tnl::Vector3 _camera_offset = { 0, -50, 20 };

	// 敵にカメラを固定しない場合
	_mainCamera_ref->target_ = player_pos;
	_mainCamera_ref->target_ -= _camera_offset;

	ControlRotationByPadOrMouse();

	// カメラの動きの遅延処理
	tnl::Vector3 fix_pos = player_pos + tnl::Vector3::TransformCoord({ 0, 100, -150 }, _mesh->rot_);
	_mainCamera_ref->pos_ += (fix_pos - _mainCamera_ref->pos_) * 0.1f;

	// 追従ポインターOFF
	_mainCamera_ref->isShowTargetPointer = false;
}


// ボム－-----------------------－-----------------------－-----------------------－-----------------------－-----------------------
void Player::ValidateBombEffect() {

	float adaptRange = 200.f;

	float euclideanDistance = (_bomb_particle->getPosition() - _mesh->pos_).length();

	if (euclideanDistance <= adaptRange) {
		ScenePlay::DeactivateAllEnemyBullets();
	}
}


void Player::UseBomb() {

	// マウス中央　ゲームパッドの場合はバツボタン、もしくはAボタンなど
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_MIDDLE) && _current_bomb_stock_count != 0 ||
		tnl::Input::IsPadDownTrigger(ePad::KEY_0) && _current_bomb_stock_count != 0) {

		ScenePlay::_isRenderPlayersBombEffect = true;
		_current_bomb_stock_count--;
	}
}


void Player::InvalidateBombEffect(const float delta_time) {

	if (ScenePlay::_isRenderPlayersBombEffect) {
		_bomb_timer += delta_time;
		ValidateBombEffect();

		if (_bomb_timer > 3.0f) {
			_bomb_timer = 0.0f;
			ScenePlay::DeactivateAllEnemyBullets();
			ScenePlay::_isRenderPlayersBombEffect = false;
		}
	}
}


// 描画－-----------------------－-----------------------－-----------------------－-----------------------－-----------------------
void Player::RenderFollowPointer()
{
	std::vector<Shared<EnemyZakoBase>>::iterator it_zako;
	std::vector<Shared<EnemyBossBase>>::iterator it_boss;
	tnl::Vector3 enemy_pos;

	if (_mainCamera_ref->isShowTargetPointer) {

		if (!_enemyZako_list_ref.empty()) {

			// インデックスが置かれている敵のみイテレーターで取得
			it_zako = std::next(_enemyZako_list_ref.begin(), _enemy_index);

			if ((*it_zako)->_isDead) {
				it_zako = _enemyZako_list_ref.erase(it_zako);
			}
			else {
				enemy_pos = (*it_zako)->_mesh->pos_;
				it_zako++;
			}
		}
		else if (_enemyZako_list_ref.empty() && !_enemyBoss_list_ref.empty()) {
			it_boss = _enemyBoss_list_ref.begin();
			enemy_pos = (*it_boss)->_mesh->pos_;
		}


		// スクリーン座標へ変換
		tnl::Vector3 screen_pos = tnl::Vector3::ConvertToScreen
		(
			{ enemy_pos.x, enemy_pos.y, enemy_pos.z }
			, DXE_WINDOW_WIDTH
			, DXE_WINDOW_HEIGHT
			, _mainCamera_ref->view_
			, _mainCamera_ref->proj_
		);

		// 追従ポインター描画
		GetTargetsScreenCoordinates(enemy_pos.x, enemy_pos.y, enemy_pos.z);
		DrawCircleAA(screen_pos.x, screen_pos.y, 5, 10, GetColor(255, 0, 0), 1, 1);
	}
}

void Player::RenderBombRemainCount() {

	std::string s = std::to_string(_current_bomb_stock_count);
	DrawFormatString(30, 80, -1, "Bomb:%s", s.c_str());
}

void Player::RenderPlayerHp() {

	if (_hp <= 0) return;

	float x1 = 60;
	float x2 = x1 + 150;

	float gage_width = abs(x2 - x1);

	float average = (_MAX_HP > 0) ? gage_width / _MAX_HP : 0;

	x2 = x1 + static_cast<int>(average * _hp);

	DrawBoxAA(x1, 50, x2, 65, GetColor(0, 255, 0), true);
}


void Player::Render(const Shared<FreeLookCamera> camera) {

	if (_isInvincible) {

		if (static_cast<int>(_invincible_timer * 10) % 3 == 0) {

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

	DrawBoxAA(60, 50, 210, 65, GetColor(150, 150, 150), true);

	RenderPlayerHp();
	SetFontSize(16);
	DrawString(30, 50, "HP:", -1);

	// ボムの残数
	SetFontSize(20);
	RenderBombRemainCount();

	for (auto blt : _straight_bullets_player) blt->Render(camera);
}


// 更新－-----------------------－-----------------------－-----------------------－-----------------------－-----------------------
void Player::UpdateStraightBullet(float delta_time)
{
	auto it_blt = _straight_bullets_player.begin();

	while (it_blt != _straight_bullets_player.end()) {

		(*it_blt)->Update(delta_time);

		if (!(*it_blt)->_isActive) {
			it_blt = _straight_bullets_player.erase(it_blt);
			continue;
		}
		it_blt++;
	}
}

void Player::WatchInvincibleTimer(const float delta_time) {

	if (_isInvincible) {
		_invincible_timer += delta_time;

		if (_invincible_timer >= _INVINCIBLE_TIME_LIMIT) {
			_invincible_timer = 0.0f;
			_isInvincible = false;
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

	ShotPlayerBullet();
	UpdateStraightBullet(delta_time);


	UseBomb();
	InvalidateBombEffect(delta_time);
}