#include <random>
#include "EnemyZakoCylinder.h"
#include "../../../Bullet/Enemy/EnemyBullet.h"
#include "../../../Bullet/Enemy/StraightBullet.h"
#include "../../../Bullet/Enemy/HomingBullet.h"
#include "../../../../Manager/Enemy/EnemyManager.h"
#include "../../../Bullet/Enemy/BulletFactory.h"


int   EnemyZakoCylinder::_at;
int   EnemyZakoCylinder::_def;


EnemyZakoCylinder::EnemyZakoCylinder(const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera)
	: EnemyZakoBase(data, player, camera), straight_bullet_count(0), homing_bullet_count(0)
{
	_explode_particle = std::make_shared<dxe::Particle>("particle/preset/explosion.bin");

	_collide_size = { 30,30,30 };

	_at = 5;
	_def = 2;

	_enemyManager = std::make_shared<EnemyManager>();

}



void EnemyZakoCylinder::SetMeshInfo() {

	_mesh = dxe::Mesh::CreateCylinderMV(20, 20);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/box.bmp"));
	_mesh->pos_ = GetRandomPosition_Mt19337();
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}



void EnemyZakoCylinder::InitBulletFactoryInstance() {

	_bulletFactory = std::make_shared<BulletFactory>(_mesh);

	// 直行弾
	std::list<Shared<StraightBullet>> bullets_straight =
		_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoCylinder, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_straight) {

		_straightBullet_queue.push_back(bullet);

	}


	// 追尾弾
	std::list<Shared<HomingBullet>> bullets_homing =
		_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoCylinder, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_homing) {
		_homingBullet_queue.push_back(bullet);
	}
}



void EnemyZakoCylinder::DoRoutineMoves(const float& delta_time) {


	// 距離 250～270内で、プレイヤーHPが０でなければプレイヤー追跡
	if (GetDistanceToPlayer() < _IDLE_DISTANCE
		&& GetDistanceToPlayer() > _ATTACK_DISTANCE
		&& Player::_hp != 0
		|| _isNoticedPlayer) {

		LookAtPlayer();

		ChasePlayer(delta_time);
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




void EnemyZakoCylinder::ChasePlayer(const float delta_time) {

	//プレイヤー追跡
	tnl::Vector3 direction = _player_ref->GetPos() - _mesh->pos_;

	direction.Normalize(direction);

	_mesh->pos_ += direction * delta_time * _charaMoveSpeed;
}



void EnemyZakoCylinder::AttackPlayer(const float& delta_time) {

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



void EnemyZakoCylinder::ShotStraightBullet(const float& delta_time) {

	static float reload_time_counter = 0.0f;  // リロード時間を追跡する変数

	straight_bullet_count++;

	// 撃った弾の間隔を空けるための処理
	if (straight_bullet_count % _bulletFireInterval == 0 && !_straightBullet_queue.empty()) {

		Shared<StraightBullet> bullet = _straightBullet_queue.front();
		_straightBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_straight_bullets.push_back(bullet);
		straight_bullet_count = 0;
	}

	ReloadStraightBulletByTimer(reload_time_counter, delta_time);
}



void EnemyZakoCylinder::ReloadStraightBulletByTimer(float& reload_time_counter, const float& delta_time)
{
	if (_straightBullet_queue.empty()) {

		reload_time_counter += delta_time;

		if (reload_time_counter >= _reloadTimeInterval) {
			std::list<Shared<StraightBullet>> bullets =
				_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoCylinder, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_straightBullet_queue.push_back(bullet);
			}
			reload_time_counter = 0.0f;
		}
	}
}


void EnemyZakoCylinder::UpdateStraightBullet(float delta_time)
{
	auto it_blt = _straight_bullets.begin();

	while (it_blt != _straight_bullets.end()) {

		if ((*it_blt)->_isActive) {

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





void EnemyZakoCylinder::ShotHomingBullet(const float& delta_time) {

	static float reload_time_counter = 0.0f;

	homing_bullet_count++;


	// 撃った弾の間隔を空けるための処理
	if (homing_bullet_count % _bulletFireInterval == 0 && !_homingBullet_queue.empty()) {

		Shared<HomingBullet> bullet = _homingBullet_queue.front();
		_homingBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_homing_bullets.push_back(bullet);
		homing_bullet_count = 0;
	}

	ReloadHomingBulletByTimer(reload_time_counter, delta_time);
}



void EnemyZakoCylinder::ReloadHomingBulletByTimer(float& reload_time_counter, const float& delta_time) {

	if (_homingBullet_queue.empty()) {

		reload_time_counter += delta_time;

		if (reload_time_counter >= _reloadTimeInterval) {
			std::list<Shared<HomingBullet>> bullets =
				_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoCylinder, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_homingBullet_queue.push_back(bullet);
			}
			reload_time_counter = 0.0f;
		}
	}
}



void EnemyZakoCylinder::UpdateHomingBullet(const float delta_time) {

	auto it_blt = _homing_bullets.begin();

	while (it_blt != _homing_bullets.end()) {

		_enemyManager->SetStraightBullets(_straight_bullets);

		if ((*it_blt)->_isActive) {

			_enemyManager->EnemyZakoStraightBullet_CollisionPairLists();

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

void EnemyZakoCylinder::Render(Shared<dxe::Camera> camera) {

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

bool EnemyZakoCylinder::Update(float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);

	UpdateStraightBullet(delta_time);
	UpdateHomingBullet(delta_time);

	SearchPlayerMovementState(delta_time);

	return true;
}