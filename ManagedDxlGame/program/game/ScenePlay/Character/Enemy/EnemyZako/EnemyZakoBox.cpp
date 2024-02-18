#include <random>
#include "EnemyZakoBox.h"
#include "../../../Bullet/Enemy/EnemyBullet.h"
#include "../../../Bullet/Enemy/StraightBullet.h"
#include "../../../Bullet/Enemy/HomingBullet.h"
#include "../../../Bullet/Enemy/BulletFactory.h"
#include "../../../../Manager/Enemy/EnemyManager.h"
#include "../../../ScenePlay.h"


int   EnemyZakoBox::_at;
int   EnemyZakoBox::_def;


EnemyZakoBox::EnemyZakoBox(const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera)
	: EnemyZakoBase(data, player, camera)
{
	_explode_particle = std::make_shared<dxe::Particle>("particle/preset/explosion.bin");

	_collide_size = { 30,30,30 };

	_at = 5;
	_def = 2;

	_randomInvestigateRange_x = 500;
	_randomInvestigateRange_y = 200;
	_randomInvestigateRange_z = 500;

	_straight_bullet_count = 0;
	_homing_bullet_count = 0;

	_enemyManager = std::make_shared<EnemyManager>();
}



void EnemyZakoBox::SetMeshInfo() {

	tnl::Vector3 meshSize = { 20,20,20 };
	_mesh = dxe::Mesh::CreateBoxMV(meshSize,
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_left.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_right.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_up.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_down.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_back.bmp"),
		dxe::Texture::CreateFromFile("graphics/boxTexture/box_forward.bmp"));

	_mesh->pos_ = GetRandomPosition_Mt19337();
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}



void EnemyZakoBox::InitBulletFactoryInstance() {

	_bulletFactory = std::make_shared<BulletFactory>(_mesh);

	// íºçsíe
	std::list<Shared<StraightBullet>> bullets_straight =
		_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoBox, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_straight) {

		_straightBullet_queue.push_back(bullet);
	}

	// í«îˆíe
	std::list<Shared<HomingBullet>> bullets_homing =
		_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoBox, _maxBulletSpawnCount);
	for (const auto& bullet : bullets_homing) {
		_homingBullet_queue.push_back(bullet);
	}
}



void EnemyZakoBox::DoRoutineMoves(const float& delta_time) {

	// ãóó£ 250Å`270ì‡Ç≈ÅAÉvÉåÉCÉÑÅ[HPÇ™ÇOÇ≈Ç»ÇØÇÍÇŒÉvÉåÉCÉÑÅ[í«ê’
	if (GetDistanceToPlayer() < _IDLE_DISTANCE
		&& GetDistanceToPlayer() > _ATTACK_DISTANCE
		&& Player::_hp != 0
		|| _isNoticedPlayer) {

		LookAtPlayer();

		ChasePlayer(delta_time);
	}

	// 250à»ì‡Ç≈ÉvÉåÉCÉÑÅ[HPÇ™ÇOÇ≈Ç»ÇØÇÍÇŒçUåÇ
	if (GetDistanceToPlayer() < _ATTACK_DISTANCE && Player::_hp != 0) {

		LookAtPlayer();

		AttackPlayer(delta_time);
	}
	// ÉAÉCÉhÉãèÛë‘
	else {
		SearchPlayerMovementState(delta_time);
	}
}



void EnemyZakoBox::AttackPlayer(const float& delta_time) {

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



void EnemyZakoBox::ShotStraightBullet(const float& delta_time) {

	static float reload_time_counter = 0.0f;  // ÉäÉçÅ[Éhéûä‘Çí«ê’Ç∑ÇÈïœêî

	_straight_bullet_count++;

	// åÇÇ¡ÇΩíeÇÃä‘äuÇãÛÇØÇÈÇΩÇﬂÇÃèàóù
	if (_straight_bullet_count % _bulletFireInterval == 0 && !_straightBullet_queue.empty()) {

		Shared<StraightBullet> bullet = _straightBullet_queue.front();
		_straightBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_straight_bullets.push_back(bullet);
		_straight_bullet_count = 0;

		PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);
	}

	ReloadStraightBulletByTimer(reload_time_counter, delta_time);
}



void EnemyZakoBox::ReloadStraightBulletByTimer(float& reload_time_counter, const float& delta_time)
{
	if (_straightBullet_queue.empty()) {

		reload_time_counter += delta_time;

		if (reload_time_counter >= _reloadTimeInterval) {
			std::list<Shared<StraightBullet>> bullets =
				_bulletFactory->CreateStraightBullet(StraightBullet::USER::ZakoBox, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_straightBullet_queue.push_back(bullet);
			}
			reload_time_counter = 0.0f;
		}
	}
}



void EnemyZakoBox::UpdateStraightBullet(float delta_time)
{
	auto it_blt = _straight_bullets.begin();

	while (it_blt != _straight_bullets.end()) {

		_enemyManager->SetStraightBullets(_straight_bullets);

		if ((*it_blt)->_isActive) {

			_enemyManager->EnemyZakoStraightBullet_CollisionPairLists(_straight_bullets);

			// íeÇÃéıñΩÇéûä‘Ç≈ä«óù
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



void EnemyZakoBox::ShotHomingBullet(const float& delta_time) {

	static float reload_time_counter = 0.0f;

	_homing_bullet_count++;


	// åÇÇ¡ÇΩíeÇÃä‘äuÇãÛÇØÇÈÇΩÇﬂÇÃèàóù
	if (_homing_bullet_count % _bulletFireInterval == 0 && !_homingBullet_queue.empty()) {

		Shared<HomingBullet> bullet = _homingBullet_queue.front();
		_homingBullet_queue.pop_front();

		bullet->_mesh->pos_ = _mesh->pos_;
		bullet->_mesh->rot_ = _mesh->rot_;

		_homing_bullets.push_back(bullet);
		_homing_bullet_count = 0;

		PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);
	}

	ReloadHomingBulletByTimer(reload_time_counter, delta_time);
}



void EnemyZakoBox::ReloadHomingBulletByTimer(float& reload_time_counter, const float& delta_time) {

	if (_homingBullet_queue.empty()) {

		reload_time_counter += delta_time;

		if (reload_time_counter >= _reloadTimeInterval) {
			std::list<Shared<HomingBullet>> bullets =
				_bulletFactory->CreateHomingBullet(HomingBullet::USER::ZakoBox, _maxBulletSpawnCount);

			for (const auto& bullet : bullets) {
				_homingBullet_queue.push_back(bullet);
			}
			reload_time_counter = 0.0f;
		}
	}
}



void EnemyZakoBox::UpdateHomingBullet(const float delta_time) {

	auto it_blt = _homing_bullets.begin();

	while (it_blt != _homing_bullets.end()) {

		if ((*it_blt)->_isActive) {

			(*it_blt)->_timer += delta_time;

			(*it_blt)->_moveDirection = _player_ref->GetPos() - (*it_blt)->_mesh->pos_;
			(*it_blt)->_moveDirection.normalize();

			(*it_blt)->_mesh->pos_ += (*it_blt)->_moveDirection * delta_time * _bulletMoveSpeed / 1.5f;


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

void EnemyZakoBox::Render(Shared<dxe::Camera> camera) {

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

bool EnemyZakoBox::Update(const float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);

	UpdateStraightBullet(delta_time);
	UpdateHomingBullet(delta_time);

	return true;
}