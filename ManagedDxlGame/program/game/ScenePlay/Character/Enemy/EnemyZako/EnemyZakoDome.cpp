#include <random>
#include "EnemyZakoDome.h"
#include "../../../Bullet/Enemy/EnemyBullet.h"
#include "../../../Bullet/Enemy/StraightBullet.h"
#include "../../../Bullet/Enemy/HomingBullet.h"
#include "../../../../Manager/Enemy/EnemyManager.h"
#include "../../../Bullet/Enemy/BulletFactory.h"


int   EnemyZakoDome::_at;
int   EnemyZakoDome::_def;


EnemyZakoDome::EnemyZakoDome(const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera)
	: EnemyZakoBase(data, player, camera)
{
	_explode_particle = std::make_shared<dxe::Particle>("particle/preset/explosion.bin");

	_collide_size = { 30,30,30 };

	_at = 5;
	_def = 2;

	_straight_bullet_count = 0;
	_homing_bullet_count = 0;


	_enemyManager = std::make_shared<EnemyManager>();

}



void EnemyZakoDome::SetMeshInfo() {

	_mesh = dxe::Mesh::CreateDomeMV(20);

	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/box.bmp"));

	_mesh->pos_ = GetRandomPosition_Mt19337();
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}



void EnemyZakoDome::InitBulletFactoryInstance() {

	_bulletFactory = std::make_shared<BulletFactory>(_mesh);

	// 直行弾
	std::list<Shared<StraightBullet>> bullets_straight =
		_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoDome, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_straight) {

		_straightBullet_queue.push_back(bullet);

	}


	// 追尾弾
	std::list<Shared<HomingBullet>> bullets_homing =
		_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoDome, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_homing) {
		_homingBullet_queue.push_back(bullet);
	}
}



void EnemyZakoDome::DoRoutineMoves(const float& delta_time) {


	// 距離 250～270内で、プレイヤーHPが０でなければプレイヤー追跡
	if (GetDistanceToPlayer() < _IDLE_DISTANCE
		&& GetDistanceToPlayer() > _ATTACK_DISTANCE
		&& Player::_hp != 0
		|| _isNoticedPlayer) {

		LookAtPlayer();

		ChasePlayer(delta_time);

		if (GetDistanceToPlayer() > 700) {

			_isNoticedPlayer = false;
			MoveToRandomInvestigatePos(delta_time);
		}
	}

	// 250以内でプレイヤーHPが０でなければ攻撃
	if (GetDistanceToPlayer() < _ATTACK_DISTANCE && Player::_hp != 0) {
		LookAtPlayer();

		AttackPlayer(delta_time);
	}
	// アイドル状態
	else {

		SearchPlayerMovementState(delta_time);
	}

	prev_pos = _mesh->pos_;
}




void EnemyZakoDome::AttackPlayer(const float& delta_time) {

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



void EnemyZakoDome::ShotStraightBullet(const float& delta_time) {

	static float reload_time_counter = 0.0f;  // リロード時間を追跡する変数

	_straight_bullet_count++;

	// 撃った弾の間隔を空けるための処理
	if (_straight_bullet_count % _bulletFireInterval == 0 && !_straightBullet_queue.empty()) {


		Shared<StraightBullet> bullet = _straightBullet_queue.front();
		_straightBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_straight_bullets.push_back(bullet);
		_straight_bullet_count = 0;

		PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_LOOP);

	}
	else {
		StopSoundMem(_shotSE_hdl);
	}

	ReloadStraightBulletByTimer(reload_time_counter, delta_time);
}



void EnemyZakoDome::ReloadStraightBulletByTimer(float& reload_time_counter, const float& delta_time)
{
	if (_straightBullet_queue.empty()) {

		reload_time_counter += delta_time;

		if (reload_time_counter >= _reloadTimeInterval) {
			std::list<Shared<StraightBullet>> bullets =
				_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoDome, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_straightBullet_queue.push_back(bullet);
			}
			reload_time_counter = 0.0f;
		}
	}
}


void EnemyZakoDome::UpdateStraightBullet(float delta_time)
{
	auto it_blt = _straight_bullets.begin();

	while (it_blt != _straight_bullets.end()) {

		_enemyManager->SetStraightBullets(_straight_bullets);

		if ((*it_blt)->_isActive) {

			_enemyManager->EnemyZakoStraightBullet_CollisionPairLists(_straight_bullets);

			(*it_blt)->_timer += delta_time;


			tnl::Vector3 move_dir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
			move_dir.normalize();

			(*it_blt)->_mesh->pos_ += move_dir * _bulletMoveSpeed * delta_time;


			if ((*it_blt)->_timer > 3) {

				(*it_blt)->_isActive = false;
				(*it_blt)->_timer = 0;
			}
		}

		else if (!(*it_blt)->_isActive) {
			it_blt = _straight_bullets.erase(it_blt);
			continue;
		}
		it_blt++;
	}
}



void EnemyZakoDome::ShotHomingBullet(const float& delta_time) {

	static float reload_time_counter = 0.0f;

	_homing_bullet_count++;


	// 撃った弾の間隔を空けるための処理
	if (_homing_bullet_count % _bulletFireInterval == 0 && !_homingBullet_queue.empty()) {

		Shared<HomingBullet> bullet = _homingBullet_queue.front();
		_homingBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_homing_bullets.push_back(bullet);
		_homing_bullet_count = 0;
	}

	ReloadHomingBulletByTimer(reload_time_counter, delta_time);
}



void EnemyZakoDome::ReloadHomingBulletByTimer(float& reload_time_counter, const float& delta_time) {

	if (_homingBullet_queue.empty()) {

		reload_time_counter += delta_time;

		if (reload_time_counter >= _reloadTimeInterval) {
			std::list<Shared<HomingBullet>> bullets =
				_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoDome, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_homingBullet_queue.push_back(bullet);
			}
			reload_time_counter = 0.0f;
		}
	}
}



void EnemyZakoDome::UpdateHomingBullet(const float delta_time) {

	auto it_blt = _homing_bullets.begin();

	while (it_blt != _homing_bullets.end()) {

		if ((*it_blt)->_isActive) {

			(*it_blt)->_timer += delta_time;

			tnl::Vector3 diff = _player_ref->GetPos() - _mesh->pos_;
			diff.normalize();

			(*it_blt)->_mesh->pos_ += diff * delta_time * _bulletMoveSpeed / 1.5f;

			if ((*it_blt)->_timer > 3) {
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


bool EnemyZakoDome::Update(const float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);

	UpdateStraightBullet(delta_time);
	UpdateHomingBullet(delta_time);

	return true;
}



void EnemyZakoDome::Render(Shared<dxe::Camera> camera) {

	if (_isDead) return;

	_mesh->render(camera);

	ShowHpGage_EnemyZako();

	for (auto blt : _straight_bullets) {

		blt->Render(camera);
	}
	for (auto blt : _homing_bullets) {

		blt->Render(camera);
	}
}