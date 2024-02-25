#include "../Player/Player.h"
#include "../../../Manager/Score/ScoreManager.h"
#include "../../../Loader/CsvLoader.h"
#include "../../../Manager/Enemy/EnemyManager.h"
#include "../game/ScenePlay/Bullet/Enemy/EnemyBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/HomingBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/BulletFactory.h"
#include "../game/ScenePlay/Collision/Collision.h"
#include "EnemyZakoBase.h"

Shared<dxe::Particle> EnemyZakoBase::_explode_particle;


bool EnemyZakoBase::_isNoticedPlayer;

// 雑魚エネミーデータ読み取り
EnemyZakoBase::EnemyZakoBase(
	const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera, const Shared<Collision>& collision) {

	_id = data._id;
	_name = data._name;
	_scale = data._scale;
	_hp = data._hp;
	_MAX_HP = data._hp;
	_charaMoveSpeed = data._charaMoveSpeed;

	_maxBulletSpawnCount = data._maxBulletSpawnCount;
	_maxTotalEnemy_SpawnCount = data._maxTotalEnemy_SpawnCount;
	_bullet_MoveSpeed = data._bullet_MoveSpeed;
	_bullet_FireInterval = data._bullet_FireInterval;
	_bullet_reloadTimeInterval = data._bullet_reloadTimeInterval;

	_player_ref = player;
	_mainCamera_ref = camera;
	_collision_ref = collision;

	_shotSE_hdl = LoadSoundMem("sound/se/shot.wav");
}


void EnemyZakoBase::ChasePlayer(const float delta_time) {

	//プレイヤー追跡
	tnl::Vector3 direction = _player_ref->GetPos() - _mesh->pos_;

	direction.Normalize(direction);

	_mesh->pos_ += direction * delta_time * _charaMoveSpeed;
}



void EnemyZakoBase::SearchPlayerMovementState(const float delta_time)
{
	if (_isNoticedPlayer) {
		_timeCountFrom_noticedPlayer += delta_time;

		// プレイヤーに気付いてから、気付いた状態を一定時間確実に保持
		if (_timeCountFrom_noticedPlayer >= _NOTICE_LIMIT_DURATION) {

			_isNoticedPlayer = false;
			_timeCountFrom_noticedPlayer = 0.0f;
		}
	}
	else {

		static float state_timer = 0.0f;

		if (_behave == EnemyZakoBase::BEHAVE::Moving) {
			//　時間制限付きでランダムに移動
			//  ランダムな時間経過後停止(5秒以内)
			_isReached_toInvestigatePos = false;

			if (!_isReached_toInvestigatePos) {
				MoveToRandomInvestigatePos(delta_time);
			}

			if (_isReached_toInvestigatePos) {
				_behave = EnemyZakoBase::BEHAVE::Stop;
			}
			else {

				_isReached_toInvestigatePos = false;
				_behave = EnemyZakoBase::BEHAVE::Moving;
			}
		}
		else if (_behave == EnemyZakoBase::BEHAVE::Stop) {

			//	時には同じ位置にとどまり左右の確認などを行う
			state_timer += delta_time;

			if (state_timer > _CHANGE_NEXT_BEHAVE_DURATION) {

				_behave = static_cast<EnemyZakoBase::BEHAVE>(rand() % 3);

				if (_behave == EnemyZakoBase::BEHAVE::Turn) {

					_isTurning = true;
				}
				else {
					_isTurning = false;
				}

				state_timer = 0.f;
			}
			else {
				_behave = EnemyZakoBase::BEHAVE::Stop;
			}
		}

		else if (_behave == EnemyZakoBase::BEHAVE::Turn) {

			static float angle = 0;

			if (_isTurning) {

				std::mt19937 gen(mt());
				std::uniform_real_distribution<float> rnd_rot(-_ROTATION_MAX_RANGE, _ROTATION_MAX_RANGE);

				if (angle == 0) {
					angle = rnd_rot(mt);
				}
				else {
					if (angle > 0) {
						angle -= 1e-10f; // 0度より大きければ左へ僅かに回転
					}
					else {
						angle += 1e-10f; // 0度より小さければ右へ僅かに回転
					}
				}

				//ランダムな方向へ向きを変える
				_mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(angle));
			}

			if (angle != 0) {

				int rnd = rand() % 2;

				if (rnd == 0) {
					_behave = EnemyZakoBase::BEHAVE::Stop;
					_isTurning = false;
				}
				else {
					_behave = EnemyZakoBase::BEHAVE::Moving;
					_isTurning = false;
				}
			}
			else {
				_behave = EnemyZakoBase::BEHAVE::Turn;
			}
		}
	}
}


void EnemyZakoBase::MoveToRandomInvestigatePos(const float& delta_time)
{
	std::mt19937 gen(mt());

	if (_investigatePos.x == 0 && _investigatePos.y == 0 && _investigatePos.z == 0) {

		std::uniform_real_distribution<float> rnd_x(-_randomInvestigateRange_x, _randomInvestigateRange_x);
		std::uniform_real_distribution<float> rnd_y(-_randomInvestigateRange_y, _randomInvestigateRange_y);
		std::uniform_real_distribution<float> rnd_z(-_randomInvestigateRange_z, _randomInvestigateRange_z);
		_investigatePos.x = rnd_x(mt);
		_investigatePos.y = rnd_y(mt);
		_investigatePos.z = rnd_z(mt);
	}

	tnl::Vector3 direction = _investigatePos - _mesh->pos_;
	direction.Normalize(direction);
	_mesh->pos_ += direction * delta_time * _charaMoveSpeed;

	// 目的地に近づいたら停止する
	if ((_investigatePos - _mesh->pos_).length() < FLT_DIG) { // == 6

		// 原点へ戻る
		_investigatePos = { 0, 0, 0 };
		_isReached_toInvestigatePos = true;
	}
}



bool EnemyZakoBase::DecreaseHP(int damage, Shared<dxe::Camera> camera) {

	if (_hp > 0) {

		damage = max(damage, 1);

		_hp -= damage;
		return true;
	}

	if (_hp <= 0) {

		dxe::DirectXRenderBegin();
		_explode_particle->setPosition(_mesh->pos_);
		_explode_particle->start();
		_explode_particle->render(camera);
		dxe::DirectXRenderEnd();

		ScoreManager::GetInstance().AddKillBonus(1000);
		_isDead = true;
	}
	return false;
}



bool EnemyZakoBase::ShowHpGage_EnemyZako() {

	if (_hp <= 0) return false;

	tnl::Vector3 hpGage_pos = tnl::Vector3::ConvertToScreen
	(
		_mesh->pos_,
		DXE_WINDOW_WIDTH,
		DXE_WINDOW_HEIGHT,
		_mainCamera_ref->view_,
		_mainCamera_ref->proj_
	);

	float x1 = hpGage_pos.x - 30;
	float x2 = hpGage_pos.x + 30;

	float gage_width = abs(x2 - x1);

	float average = (_MAX_HP > 0) ? gage_width / _MAX_HP : 0;

	x2 = x1 + static_cast<int>(average * _hp);

	DrawBoxAA(x1, hpGage_pos.y - 30, x2, hpGage_pos.y - 25, GetColor(255, 0, 0), true);

	return true;
}



void EnemyZakoBase::Render(Shared<dxe::Camera> camera) {

	if (_isDead) return;

	_mesh->render(camera);

	ShowHpGage_EnemyZako();

	for (auto& blt : _straight_bullets) {

		blt->Render(camera);
	}
	for (auto& blt : _homing_bullets) {

		blt->Render(camera);
	}
}



void EnemyZakoBase::ShotStraightBullet(const float& delta_time) {

	_straightBullet_count++;

	// 撃った弾の間隔を空けるための処理
	if (_straightBullet_count % _bullet_FireInterval == 0 && !_straightBullet_queue.empty()) {

		Shared<StraightBullet> bullet = _straightBullet_queue.front();
		_straightBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_straight_bullets.push_back(bullet);
		_straightBullet_count = 0;

		PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);
	}

	ReloadStraightBulletByTimer(delta_time);
}


void EnemyZakoBase::ReloadStraightBulletByTimer(const float& delta_time)
{
	if (_straightBullet_queue.empty()) {

		_reloadStraightBullet_timeCounter += delta_time;

		if (_reloadStraightBullet_timeCounter >= _bullet_reloadTimeInterval) {
			std::list<Shared<StraightBullet>> bullets =
				_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoBox, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_straightBullet_queue.push_back(bullet);
			}
			_reloadStraightBullet_timeCounter = 0.0f;
		}
	}
}



void EnemyZakoBase::UpdateStraightBullet(float delta_time)
{
	auto it_blt = _straight_bullets.begin();

	while (it_blt != _straight_bullets.end()) {

		if ((*it_blt)->_isActive) {

			if (_collision_ref->CheckCollision_EnemyStraightBulletAndPlayer((*it_blt), _player_ref)) {

				if (_player_ref->DecreaseHP(_at - _player_ref->GetDEF())) {
					_player_ref->SetIsInvincible(true);
					_player_ref->PlayDamageHitSE();
				}

				(*it_blt)->_isActive = false;
				(*it_blt)->_timer = 0;
			}

			// 弾の寿命を時間で管理
			(*it_blt)->_timer += delta_time;

			tnl::Vector3 move_dir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
			move_dir.normalize();

			(*it_blt)->_mesh->pos_ += move_dir * _bullet_MoveSpeed * delta_time;


			if ((*it_blt)->_timer > _STRAIGHTBULLET_LIFETIME_LIMIT) {

				(*it_blt)->_isActive = false;
				(*it_blt)->_timer = 0;
			}
		}
		else {
			it_blt = _straight_bullets.erase(it_blt);
			continue;
		}
		it_blt++;
	}
}



void EnemyZakoBase::ShotHomingBullet(const float& delta_time) {

	_homingBullet_count++;

	// 撃った弾の間隔を空けるための処理
	if (_homingBullet_count % _bullet_FireInterval == 0 && !_homingBullet_queue.empty()) {

		Shared<HomingBullet> bullet = _homingBullet_queue.front();
		_homingBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_homing_bullets.push_back(bullet);
		_homingBullet_count = 0;

		PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);
	}

	ReloadHomingBulletByTimer(delta_time);
}



void EnemyZakoBase::ReloadHomingBulletByTimer(const float& delta_time) {

	if (_homingBullet_queue.empty()) {

		_reloadHomingBullet_timeCounter += delta_time;

		if (_reloadHomingBullet_timeCounter >= _bullet_reloadTimeInterval) {
			std::list<Shared<HomingBullet>> bullets =
				_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoBox, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_homingBullet_queue.push_back(bullet);
			}
			_reloadHomingBullet_timeCounter = 0.0f;
		}
	}
}



void EnemyZakoBase::UpdateHomingBullet(const float delta_time) {

	auto it_blt = _homing_bullets.begin();

	while (it_blt != _homing_bullets.end()) {

		if ((*it_blt)->_isActive) {

			if (_collision_ref->CheckCollision_EnemyHomingBulletAndPlayer((*it_blt), _player_ref)) {

				if (_player_ref->DecreaseHP(_at - _player_ref->GetDEF())) {
					_player_ref->SetIsInvincible(true);
					_player_ref->PlayDamageHitSE();
				}

				(*it_blt)->_isActive = false;
				(*it_blt)->_timer = 0;
			}

			float timeToReachPlayer =
				_minTimeToReach + (GetDistanceToPlayer() / 1000) * (_maxTimeToReach - _minTimeToReach);

			timeToReachPlayer = std::clamp(timeToReachPlayer, _minTimeToReach, _maxTimeToReach);

			tnl::Vector3 targetDir = _player_ref->GetPos() - (*it_blt)->_mesh->pos_;
			targetDir.normalize();

			(*it_blt)->_moveDirection =	tnl::Vector3::UniformLerp(
				(*it_blt)->_moveDirection,
				targetDir * _bulletTurnDelayRate,
				timeToReachPlayer, (*it_blt)->_timer
			);

			(*it_blt)->_mesh->pos_ += 
				(*it_blt)->_moveDirection * delta_time * _bullet_MoveSpeed / 1.5f;


			(*it_blt)->_timer += delta_time;

			if ((*it_blt)->_timer > _HOMINGBULLET_LIFETIME_LIMIT) {
				(*it_blt)->_isActive = false;
				(*it_blt)->_timer = 0;
			}
		}

		else if (!(*it_blt)->_isActive) {
			it_blt = _homing_bullets.erase(it_blt);
			continue;
		}
		it_blt++;
	}
}



void EnemyZakoBase::AttackPlayer(const float& delta_time) {

	if (_isShotStraightBullet) {
		ShotStraightBullet(delta_time);

		if (_straightBullet_queue.empty()) {
			_isShotStraightBullet = false;

			int randValue = rand() % 2;

			if (randValue == 0) {
				_isShotStraightBullet = true;
			}
			else {
				_isShotHomingBullet = true;
			}
		}
	}

	else if (_isShotHomingBullet) {
		ShotHomingBullet(delta_time);

		if (_homingBullet_queue.empty()) {
			_isShotHomingBullet = false;
		}
		int randValue = rand() % 2;

		if (randValue == 0) {
			_isShotStraightBullet = true;
		}
		else {
			_isShotHomingBullet = true;
		}
	}
}



void EnemyZakoBase::DoRoutineMoves(const float& delta_time) {

	// 距離 250～270内で、プレイヤーHPが０でなければプレイヤー追跡
	if (GetDistanceToPlayer() < GetIdleDistance()
		&& GetDistanceToPlayer() > GetAttackDistance()
		&& _player_ref->GetHP() != 0 || _isNoticedPlayer) {

		LookAtPlayer();

		if (!_isAttacking)
			ChasePlayer(delta_time);
	}

	// 250以内でプレイヤーHPが０でなければ攻撃
	if (GetDistanceToPlayer() < GetAttackDistance() && _player_ref->GetHP() != 0) {

		LookAtPlayer();

		_isAttacking = true;
		AttackPlayer(delta_time);
	}
	// アイドル状態
	else {
		_isAttacking = false;
		SearchPlayerMovementState(delta_time);
	}
}



bool EnemyZakoBase::Update(const float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);

	UpdateStraightBullet(delta_time);
	UpdateHomingBullet(delta_time);

	return true;
}