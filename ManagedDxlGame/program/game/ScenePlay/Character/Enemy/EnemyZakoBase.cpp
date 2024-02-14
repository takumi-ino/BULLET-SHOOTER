#include <random>
#include "../Player/Player.h"
#include "../../../Manager/Score/ScoreManager.h"
#include "../../Bullet/Enemy/EnemyBullet.h"
#include "../../../Loader/CsvLoader.h"
#include "../../../Manager/Enemy/EnemyManager.h"
#include "EnemyZakoBase.h"

Shared<dxe::Particle> EnemyZakoBase::_explode_particle;


bool EnemyZakoBase::_isNoticedPlayer;

// 雑魚エネミーデータ読み取り
EnemyZakoBase::EnemyZakoBase(const EnemyZakoInfo& data, const Shared<Player>& player, const Shared<dxe::Camera>& camera) {

	_id = data._id;
	_name = data._name;
	_scale = data._scale;
	_hp = data._hp;
	_MAX_HP = data._hp;
	_charaMoveSpeed = data._charaMoveSpeed;

	_maxBulletSpawnCount = data._maxBulletSpawnCount;
	_maxTotalEnemySpawnCount = data._maxTotalEnemySpawnCount;
	_bulletMoveSpeed = data._bulletMoveSpeed;
	_bulletFireInterval = data._bulletFireInterval;
	_reloadTimeInterval = data._reloadTimeInterval;

	_player_ref = player;
	_mainCamera_ref = camera;
}


void EnemyZakoBase::SearchPlayerMovementState(const float delta_time)
{
	if (_isNoticedPlayer) {
		_timeFrom_noticedPlayer += delta_time;

		// プレイヤーに気付いてから5秒間は確実に気付いた状態を保持
		if (_timeFrom_noticedPlayer >= 5.0f) {
			_isNoticedPlayer = false;
			_timeFrom_noticedPlayer = 0.0f;
		}
	}
	else {

		static float state_timer = 0.0f;

		if (_behave == EnemyZakoBase::MOVE_BEHAVE::Moving) {
			//　時間制限付きでランダムに移動
			//  ランダムな時間経過後停止(5秒以内)
			_isReachedToInvestigatePos = false;

			if (!_isReachedToInvestigatePos) {
				MoveToRandomInvestigatePos(delta_time);
			}

			if (_isReachedToInvestigatePos) {

				_behave = EnemyZakoBase::MOVE_BEHAVE::Stop;
			}
			else {
				_isReachedToInvestigatePos = false;
				_behave = EnemyZakoBase::MOVE_BEHAVE::Moving;
			}
		}
		else if (_behave == EnemyZakoBase::MOVE_BEHAVE::Stop) {

			//	時には同じ位置にとどまり左右の確認などを行う
			state_timer += delta_time;

			if (state_timer > 5) {

				_behave = static_cast<EnemyZakoBase::MOVE_BEHAVE>(rand() % 3);

				if (_behave == EnemyZakoBase::MOVE_BEHAVE::Turn)
					_isTurning = true;
				else _isTurning = false;

				state_timer = 0.f;
			}
			else {
				_behave = EnemyZakoBase::MOVE_BEHAVE::Stop;
			}
		}

		else if (_behave == EnemyZakoBase::MOVE_BEHAVE::Turn) {

			static float angle = 0;

			if (_isTurning) {

				std::random_device mt;
				std::mt19937 gen(mt());
				std::uniform_real_distribution<float> rnd_rot(-180.0f, 180.0f);

				if (angle == 0) {
					angle = rnd_rot(mt);
				}
				else {
					if (angle > 0) {
						angle -= 1e-10;
					}
					else {
						angle += 1e-10;
					}
				}

				//ランダムな方向へ向きを変える
				_mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(angle));
			}

			if (angle != 0) {

				int rnd = rand() % 2;

				if (rnd == 0) {
					_behave = EnemyZakoBase::MOVE_BEHAVE::Stop;
					_isTurning = false;
				}
				else {
					_behave = EnemyZakoBase::MOVE_BEHAVE::Moving;
					_isTurning = false;
				}
			}
			else {
				_behave = EnemyZakoBase::MOVE_BEHAVE::Turn;
			}
		}
	}
}


void EnemyZakoBase::MoveToRandomInvestigatePos(const float& delta_time)
{
	std::random_device mt;
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
	if ((_investigatePos - _mesh->pos_).length() < 10) {

		// 原点へ戻る
		_investigatePos = { 0, 0, 0 };
		_isReachedToInvestigatePos = true;
	}
}



bool EnemyZakoBase::DecreaseHP(int damage, Shared<dxe::Camera> camera) {

	if (_hp > 0) {

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

	tnl::Vector3 hpGage_pos =
		tnl::Vector3::ConvertToScreen(_mesh->pos_, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, _mainCamera_ref->view_, _mainCamera_ref->proj_);

	float x1 = hpGage_pos.x - 30;
	float x2 = hpGage_pos.x + 30;

	float gage_width = abs(x2 - x1);

	float average = (_MAX_HP > 0) ? gage_width / _MAX_HP : 0;

	x2 = x1 + static_cast<int>(average * _hp);

	DrawBoxAA(x1, hpGage_pos.y - 30, x2, hpGage_pos.y - 25, GetColor(255, 0, 0), true);

	return true;
}