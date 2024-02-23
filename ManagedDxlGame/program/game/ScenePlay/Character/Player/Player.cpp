#include <sstream>
#include <iomanip>
#include "../../Bullet/Player/PlayerBullet.h"
#include "../../Sky/SkyBox.h"
#include "../../ScenePlay.h"
#include "../../Camera/FreeLookCamera.h"
#include "../../Character/Enemy/EnemyZakoBase.h"
#include "../../Character/Enemy/EnemyBossBase.h"
#include "../../Pause/PauseMenu.h"
#include "../game/ScenePlay/Bullet/Player/Gunport.h"
#include "Player.h"


int Player::_hp;
int Player::_MAX_HP;
int Player::_at;
int Player::_def;
int Player::_currentBomb_stockCount;
int Player::_getDamageSE_hdl;

float Player::_moveSpeed;
float Player::_invincibleTimer;
float Player::_bombTimer;

bool Player::_isInvincible;

Shared<dxe::Particle> Player::_bombParticle;

Player::Player(const Shared<FreeLookCamera> camera_ref) {

	_mesh = dxe::Mesh::CreateSphereMV(20, 10, 10, false);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/prismatic-star.png"));
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
	_mesh->pos_ = { 0, 100, -300 };


	_collideSize = { 20, 20, 20 };

	_hp = 8;
	_MAX_HP = _hp;
	_at = 2;
	_def = 4;
	_moveSpeed = 0.4f;

	_getDamageSE_hdl = LoadSoundMem("sound/se/getHit.mp3");

	_mainCamera_ref = camera_ref;

	_bombParticle = std::make_shared<dxe::Particle>("particle/preset/bombEffect.bin");
	_playerGunport = std::make_shared<Gunport>();
}


// �@�\�|-----------------------�|-----------------------�|-----------------------�|-----------------------�|---------------------
void Player::SetEnemyZakoListRef(const std::vector<Shared<EnemyZakoBase>>& enemies_list_ref) {
	_enemyZakoList_ref = enemies_list_ref;
}


void Player::SetEnemyBossListRef(const std::vector<Shared<EnemyBossBase>>& enemyBoss_list_ref) {
	_enemyBossList_ref = enemyBoss_list_ref;
}


void Player::EraseEnemyZakoListRef(Shared<EnemyZakoBase>& enemy_list_ref) {

	for (auto it = _enemyZakoList_ref.begin(); it != _enemyZakoList_ref.end(); it++) {

		if (*it == enemy_list_ref) {
			_enemyZakoList_ref.erase(it);
			break;
		}
	}
}


void Player::EraseEnemyBossListRef(Shared<EnemyBossBase>& enemyBoss_ref) {

	for (auto it = _enemyBossList_ref.begin(); it != _enemyBossList_ref.end(); it++) {

		if (*it == enemyBoss_ref) {
			_enemyBossList_ref.erase(it);
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
		// �Q�[���I�[�o�[
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

		// �x�N�g�����K��
		move_vel_ = move_vel_.Normalize(move_vel_) * speed;
	}
}


const tnl::Vector3 Player::GetTargetsScreenCoordinates(const float& x, const float& y, const float& z) {

	tnl::Vector3 temp = { x,y,z };

	return temp;
}


bool Player::IsEnemyInCapturableRange() {

	tnl::Vector3 player_pos = _mesh->pos_;

	if (!_enemyZakoList_ref.empty()) {

		for (auto it_enemyList : _enemyZakoList_ref) {

			if (!it_enemyList) break;

			tnl::Vector3 target_enemy_pos = it_enemyList->_mesh->pos_;
			float dis_player_enemy = (target_enemy_pos - player_pos).length();
			if (dis_player_enemy < 500) {
				return true;
			}
		}
	}
	else if (_enemyZakoList_ref.empty() && !_enemyBossList_ref.empty()) {

		auto it_bossList = _enemyBossList_ref.begin();

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


const tnl::Vector3& Player::GetEnemyPosition() {
	tnl::Vector3 moveDir;

	if (_mainCamera_ref->follow) {

		if (!_enemyZakoList_ref.empty()) {

			if (_enemyIndex < _enemyZakoList_ref.size()) {
				_it_zako_ref = std::next(_enemyZakoList_ref.begin(), _enemyIndex);
				_enemyPos_ref = (*_it_zako_ref)->_mesh->pos_;
			}
			else {
				_enemyIndex = 0;
				_it_zako_ref = _enemyZakoList_ref.begin();
				_enemyPos_ref = (*_it_zako_ref)->_mesh->pos_;
			}
		}
		else if (_enemyZakoList_ref.empty() && !_enemyBossList_ref.empty()) {
			_it_boss_ref = _enemyBossList_ref.begin();
			_enemyPos_ref = (*_it_boss_ref)->_mesh->pos_;
		}

		moveDir = _enemyPos_ref - _mesh->pos_;
		moveDir.normalize();
	}
	else {
		moveDir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
	}

	return moveDir;
}

// ����|-----------------------�|-----------------------�|-----------------------�|-----------------------�|---------------------
bool Player::IsShooting() {

	return tnl::Input::IsMouseDown(eMouse::LEFT) || tnl::Input::IsPadDown(ePad::KEY_1);
}


void Player::ShotPlayerBullet(const float deltaTime) {

	if (IsShooting()) {

		tnl::Vector3 spawn_pos = _mesh->pos_;
		tnl::Vector3 moveDir = GetEnemyPosition();

		_straightBullets_player.emplace_back(std::make_shared<PlayerBullet>(spawn_pos, moveDir, PlayerBullet::COLOR::Red, 10.0f));
	}
}



void Player::ShotGunportBullet() {

	if (IsShooting()) {

		tnl::Vector3 moveDir = GetEnemyPosition();

		for (auto& blt : _gunportVec) {

			if (blt) {

				_straightBullets_player.emplace_back(std::make_shared<PlayerBullet>(blt->_mesh->pos_, moveDir, PlayerBullet::COLOR::White, 7.0f));
			}
		}
	}
}


void Player::ControlPlayerMoveByInput(const float delta_time) {

	float speed = 4.f;

	// ����  �Q�[���p�b�h�̏ꍇ�́����x
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) || tnl::Input::IsPadDown(ePad::KEY_3))
		speed = 2.5f;


	// ������
	if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT) && !_mainCamera_ref->follow) {

		move_vel_ -= tnl::Vector3::TransformCoord({ 1.0f, 0, 0 }, rot_y_);

		NormalizeCameraSpeed(speed);
	}

	// �E����
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT) && !_mainCamera_ref->follow) {

		move_vel_ += tnl::Vector3::TransformCoord({ 1.0f, 0, 0 }, rot_y_);

		NormalizeCameraSpeed(speed);
	}

	// ������@�@//�@���̃Q�[���p�b�h��L���ɂ���ƍ��W����ɔ��ōs���o�O������
	if (tnl::Input::IsKeyDown(eKeys::KB_W) /*|| tnl::Input::IsPadDown(ePad::KEY_UP)*/) {

		move_vel_ += tnl::Vector3::TransformCoord({ 0, 1.0f, 0.1f }, rot_y_);
		_mesh->pos_.z += _moveSpeed;

		NormalizeCameraSpeed(speed);
	}

	// ������
	if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsPadDown(ePad::KEY_DOWN)) {

		move_vel_ -= tnl::Vector3::TransformCoord({ 0, 1.0f, 0.1f }, rot_y_);
		_mesh->pos_.z -= _moveSpeed;

		NormalizeCameraSpeed(speed);
	}

	// �O�����@�Q�[���p�b�h�̏ꍇ��L1
	if (tnl::Input::IsKeyDown(eKeys::KB_SPACE) || tnl::Input::IsPadDown(ePad::KEY_4)) {

		_mesh->pos_ += tnl::Vector3::TransformCoord(
			{ 0,0,2 }, _mesh->rot_) * _forwardVelocity * delta_time * 150;
	}
}


void Player::ChangeTarget_ByMouseWheel() {

	int wheel = tnl::Input::GetMouseWheel();

	// �}�E�X�z�C�[���̓��͂ɉ����ēG�̃C���f�b�N�X�𑝌�
	if (wheel > 0) {
		_enemyIndex++;
		if (_enemyIndex >= _enemyZakoList_ref.size()) {
			_enemyIndex = 0;
		}
	}
	else if (wheel < 0) {
		_enemyIndex--;
		if (_enemyIndex < 0) {
			_enemyIndex = static_cast<int>(_enemyZakoList_ref.size() - 1);
		}
	}
}


void Player::ControlRotationByPadOrMouse() {

	// �Q�[���p�b�h
	if (!_mainCamera_ref->follow) {

		tnl::Vector3 vel = tnl::Input::GetLeftStick();

		// ���E���_
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x * 1));

		// �㉺���_
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_y_);
		rot_x_ *= tnl::Quaternion::RotationAxis(tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * 0.01f));
	}

	// �}�E�X
	if (!_mainCamera_ref->follow
		&& tnl::Input::GetLeftStick().x == 0
		&& tnl::Input::GetLeftStick().y == 0
		&& tnl::Input::GetLeftStick().z == 0) {

		tnl::Vector3 vel = tnl::Input::GetMouseVelocity();

		// ���E���_
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(vel.x * 0.05f));

		// �㉺���_
		tnl::Vector3 forward = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_y_);
		rot_x_ *= tnl::Quaternion::RotationAxis(tnl::Vector3::Cross({ 0, 1, 0 }, forward), tnl::ToRadian(vel.y * 0.01f));
	}
}

void Player::AdjustPlayerVelocity() {

	// Time.deltaTime�̂悤�Ȃ��́B���ꂪ�Ȃ��ƃv���C���[���������
	tnl::EasyAdjustObjectVelocity(centroid_radius_, mass_, friction_, past_move_vel_, move_vel_, center_of_gravity_);

	_mesh->pos_ += move_vel_;

	if (center_of_gravity_.length() > FLT_EPSILON) {
		// �d�S�ʒu�𗘗p���ČX���Ăق����A�b�p�[�x�N�g�����쐬
		tnl::Vector3 upper = tnl::Vector3::Normalize({ center_of_gravity_.x, centroid_radius_, center_of_gravity_.z });
		// �X���̊p�x���v�Z
		float angle = upper.angle({ 0, 1, 0 });
		// �X���x�N�g���Ɛ^��x�N�g���̊O�ς����]�����v�Z���A�X���p�x�𒲐����ĉ�]�N�H�[�^�j�I�����쐬
		rot_xz_ = tnl::Quaternion::RotationAxis(tnl::Vector3::Cross(upper, { 0, 1, 0 }), -(angle * 0.2f));
	}

	//// �ŏI�I�Ȏp��
	//_mesh->rot_ = rot_y_ * rot_xz_;
	// ControlRotationByMouse �ŏ㉺���_���g�p����ꍇ�́����g��
	_mesh->rot_ = rot_y_ * rot_x_ * rot_xz_;
}

// �J�����|-----------------------�|-----------------------�|-----------------------�|-----------------------�|---------------------
void Player::AssignTargetEnemy_ForDarkSoulsCamera(tnl::Vector3& target_enemy_pos) {

	if (!_enemyZakoList_ref.empty()) {

		if (_enemyIndex < _enemyZakoList_ref.size()) {

			_it_zako_ref = std::next(_enemyZakoList_ref.begin(), _enemyIndex);
			target_enemy_pos = (*_it_zako_ref)->_mesh->pos_;
		}
		else {
			_enemyIndex = 0;
			_it_zako_ref = _enemyZakoList_ref.begin();
			target_enemy_pos = (*_it_zako_ref)->_mesh->pos_;
		}
	}
	else if (_enemyZakoList_ref.empty() && !_enemyBossList_ref.empty()) {

		auto it_bossList_ref = _enemyBossList_ref.begin();

		if (*it_bossList_ref)
			target_enemy_pos = (*it_bossList_ref)->_mesh->pos_;
	}
}

tnl::Vector3 cameraPos;
bool isGetLastCameraPos = false;

void Player::ActivateDarkSoulsCamera() {

	tnl::Vector3 playerPos = _mesh->pos_;
	tnl::Vector3 targetEnemyPos;

	AssignTargetEnemy_ForDarkSoulsCamera(targetEnemyPos);

	float dis_player_enemy = (targetEnemyPos - playerPos).length();


	if (_mainCamera_ref->follow) {

		if (!isGetLastCameraPos) {
			cameraPos = _mainCamera_ref->pos_;
			isGetLastCameraPos = true;
		}

		ControlCameraWithEnemyFocus(playerPos, targetEnemyPos);
	}
	else {

		ControlCameraWithoutEnemyFocus();
	}
}


void Player::ControlCameraWithoutEnemyFocus()
{
	tnl::Vector3 playerPos = _mesh->pos_;

	tnl::Vector3 cameraOffset = { 0, -50, 20 };

	// �G�ɃJ�������Œ肵�Ȃ��ꍇ
	_mainCamera_ref->target_ = playerPos;
	_mainCamera_ref->target_ -= cameraOffset;

	ControlRotationByPadOrMouse();

	// �J�����̓����̒x������
	tnl::Vector3 fixPos = playerPos + tnl::Vector3::TransformCoord({ 0, 100, -150 }, _mesh->rot_);
	_mainCamera_ref->pos_ += (fixPos - _mainCamera_ref->pos_) * 0.05f;

	// �Ǐ]�|�C���^�[OFF
	_mainCamera_ref->isShowTargetPointer = false;
}


void Player::ControlCameraWithEnemyFocus(tnl::Vector3& player_pos, tnl::Vector3& target_enemy_pos)
{
	// �Ǐ]�|�C���^�[ON�i�`��j
	_mainCamera_ref->isShowTargetPointer = true;

	ChangeTarget_ByMouseWheel();
	RenderFollowPointer();

	tnl::Vector3 tmp{};
	tmp.y = player_pos.y + 100;
	// �J�������v���C���[�ƓG�̒��Ԓn�_�ɌŒ�
	_mainCamera_ref->target_ = (tmp + target_enemy_pos) / 2;

	//_player_ref->_mainCamera_ref->ControlFreeLookCamera(_mainCamera_ref, _player_ref->_mesh, _mainCamera_ref->target_);
	// �J�����̈ʒu = �v���C���[���W + (normalized(�v���C���[���W - �G���W) * �J�����ƃv���C���[�̍���)
	// �������A�P���Ƀv���C���[�̍��W���g���ƃv���C���[�ƃG�l�~�[���d�Ȃ��Ă��܂����߁A�������g��

	tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0,1,0 }, _mainCamera_ref->axis_y_angle_);
	tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0,0,1 }, q);
	tnl::Vector3 localAxis_x = tnl::Vector3::Cross({ 0,1,0 }, xz);
	q *= tnl::Quaternion::RotationAxis(localAxis_x, _mainCamera_ref->axis_x_angle_);

	_mesh->rot_ = tnl::Quaternion::LookAt(player_pos, target_enemy_pos, localAxis_x);

	float offset = -300;

	float y = 0;

	//������
	if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsPadDown(ePad::KEY_LEFT)) {

		tnl::Vector3 newPos = _mainCamera_ref->target_ + tnl::Vector3::TransformCoord({ offset, 0, offset }, q);
		newPos.y = _mesh->pos_.y;

		y = 100;
		_mesh->pos_ = newPos;
		_mainCamera_ref->axis_y_angle_ += tnl::ToRadian(2);
	}

	// �E����
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsPadDown(ePad::KEY_RIGHT)) {

		tnl::Vector3 newPos = _mainCamera_ref->target_ + tnl::Vector3::TransformCoord({ offset, 0, offset }, q);
		newPos.y = _mesh->pos_.y;

		y = -100;
		_mesh->pos_ = newPos;
		_mainCamera_ref->axis_y_angle_ -= tnl::ToRadian(2);
	}

	// �J�����̓����̒x������
	tnl::Vector3 fix_pos = player_pos + tnl::Vector3::TransformCoord({ 50, y, -140 }, _mesh->rot_);
	_mainCamera_ref->pos_ += (fix_pos - _mainCamera_ref->pos_) * 0.1f;
}

// �{���|-----------------------�|-----------------------�|-----------------------�|-----------------------�|-----------------------
void Player::ValidateBombEffect() {

	float adaptRange = 200.f;

	float euclideanDistance = (_bombParticle->getPosition() - _mesh->pos_).length();

	if (euclideanDistance <= adaptRange) {
		ScenePlay::DeactivateAllEnemyBullets();
	}
}


void Player::UseBomb() {

	// �}�E�X�����@�Q�[���p�b�h�̏ꍇ�̓o�c�{�^���A��������A�{�^���Ȃ�
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_MIDDLE) && _currentBomb_stockCount != 0 ||
		tnl::Input::IsPadDownTrigger(ePad::KEY_0) && _currentBomb_stockCount != 0) {

		ScenePlay::_isRender_playersBombEffect = true;
		_currentBomb_stockCount--;
	}
}


void Player::InvalidateBombEffect(const float delta_time) {

	if (ScenePlay::_isRender_playersBombEffect) {
		_bombTimer += delta_time;
		ValidateBombEffect();

		if (_bombTimer > 3.0f) {
			_bombTimer = 0.0f;
			ScenePlay::DeactivateAllEnemyBullets();
			ScenePlay::_isRender_playersBombEffect = false;
		}
	}
}

// �`��|-----------------------�|-----------------------�|-----------------------�|-----------------------�|-----------------------
void Player::RenderFollowPointer()
{
	if (_mainCamera_ref->isShowTargetPointer) {

		if (!_enemyZakoList_ref.empty()) {

			// �C���f�b�N�X���u����Ă���G�̂݃C�e���[�^�[�Ŏ擾
			if (_enemyIndex < _enemyZakoList_ref.size()) {

				_it_zako_ref = std::next(_enemyZakoList_ref.begin(), _enemyIndex);
				_enemyPos_ref = (*_it_zako_ref)->_mesh->pos_;
			}
			else {
				_enemyIndex = 0;
				_it_zako_ref = _enemyZakoList_ref.begin();
				_enemyPos_ref = (*_it_zako_ref)->_mesh->pos_;
			}
		}
		else if (_enemyZakoList_ref.empty() && !_enemyBossList_ref.empty()) {
			_it_boss_ref = _enemyBossList_ref.begin();
			_enemyPos_ref = (*_it_boss_ref)->_mesh->pos_;
		}
	}

	// �X�N���[�����W�֕ϊ�
	tnl::Vector3 screenPos = tnl::Vector3::ConvertToScreen
	(
		{ _enemyPos_ref.x, _enemyPos_ref.y, _enemyPos_ref.z }
		, DXE_WINDOW_WIDTH
		, DXE_WINDOW_HEIGHT
		, _mainCamera_ref->view_
		, _mainCamera_ref->proj_
	);

	// �Ǐ]�|�C���^�[�`��
	GetTargetsScreenCoordinates(_enemyPos_ref.x, _enemyPos_ref.y, _enemyPos_ref.z);
	DrawCircleAA(screenPos.x, screenPos.y, 5, 10, GetColor(255, 0, 0), 1, 1);
}


void Player::RenderGunport(const Shared<FreeLookCamera> camera) {

	if (_gunportVec.empty()) return;

	for (auto it : _gunportVec) {
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

	float x1 = 60;
	float x2 = x1 + 150;

	float gageWidth = abs(x2 - x1);

	float average = (_MAX_HP > 0) ? gageWidth / _MAX_HP : 0;

	x2 = x1 + static_cast<int>(average * _hp);

	DrawBoxAA(x1, 50, x2, 65, GetColor(0, 255, 0), true);
}


void Player::Render(const Shared<FreeLookCamera> camera) {

	if (_isInvincible) {

		if (static_cast<int>(_invincibleTimer * 10) % 3 == 0) {

			//�@�v���C���[��������
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
	RenderBulletPowerRate();
	RenderGunport(camera);

	SetFontSize(16);
	DrawString(30, 50, "HP:", -1);

	// �{���̎c��
	SetFontSize(20);
	RenderBombRemainCount();

	for (auto blt : _straightBullets_player) blt->Render(camera);
}


// �X�V�|-----------------------�|-----------------------�|-----------------------�|-----------------------�|-----------------------
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


void Player::UpdateGunport_DRY(Shared<Gunport>& gunportVec,  const tnl::Vector3 coords) {

	gunportVec->_mesh->pos_ = GetPos() + tnl::Vector3::TransformCoord({ coords }, _mesh->rot_);
	gunportVec->_mesh->rot_ = _mesh->rot_;
}


void Player::UpdateGunport() {

	if (_gunportVec.empty()) return;

	const tnl::Vector3 coords[] = {
	{0, -25, -20},
	{-25, 0, -20},
	{25, 0, -20},
	{-15, -15, -20},
	{15, -15, -20}
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
		UpdateGunport_DRY(_gunportVec[0], { coords[1] } );
		UpdateGunport_DRY(_gunportVec[1], { coords[2] } );
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

	// �J������G�ɌŒ肷��t���O�𔽓]
	// �}�E�X�E�@�Q�[���p�b�h�̏ꍇ��R1�iRB)
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