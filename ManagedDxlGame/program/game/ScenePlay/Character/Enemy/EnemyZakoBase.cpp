#include "EnemyZakoBase.h"
#include "../Player/Player.h"
#include "../../../Manager/Score/ScoreManager.h"
#include "../../../Manager/Enemy/EnemyManager.h"
#include "../game/ScenePlay/Bullet/Enemy/EnemyBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/StraightBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/HomingBullet.h"
#include "../game/ScenePlay/Bullet/Enemy/BulletFactory.h"
#include "../game/ScenePlay/Collision/Collision.h"
#include "../game/Utility/CustomException.h"
#include "../game/ScenePlay/RandomValue/RandomValueGenerator.h"


namespace inl {

	Shared<dxe::Particle> EnemyZakoBase::_explode_particle;

	bool EnemyZakoBase::_isNoticedPlayer;


	namespace {

		const float _ROTATION_MAX_RANGE{ 180.0f };
		const float _STRAIGHTBULLET_LIFETIME_LIMIT{ 3.0f };
		const float _HOMINGBULLET_LIFETIME_LIMIT{ 5.0f };
	}


	// 雑魚エネミーデータ読み取り
	EnemyZakoBase::EnemyZakoBase(
		const EnemyZakoInfo& data,
		const Shared<Player>& player,
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision)
	{

		Shared<CustomException> cus = std::make_shared<CustomException>();

		auto path = cus->TryLoadSound("sound/se/shot.wav", "inl::EnemyZakoBase::EnemyZakoBase()");

		_shotSE_hdl = path;                                           // SE
		_id = data._id;												  // ID
		_hp = data._hp;												  // HP
		_MAX_HP = data._hp;											  // 最大HP
		_name = data._name;											  // 名前
		_scale = data._scale;										  // サイズ
		_enemyMoveSpeed = data._enemyMoveSpeed;						  // 移動スピード
		_maxBulletSpawnCount = data._maxBulletSpawnCount;			  // 弾の最大生成数
		_maxTotalEnemy_spawnCount = data._maxTotalEnemy_spawnCount;	  // 敵の総数
		_bulletMoveSpeed = data._bulletMoveSpeed;					  // 弾のスピード
		_bulletFireInterval = data._bulletFireInterval;				  // 弾の撃つ間隔
		_bulletReloadTimeInterval = data._bulletReloadTimeInterval;	  // 弾のリロード間隔

		_player_ref = player;										  // プレイヤー
		_enemyCamera = camera;										  // カメラ
		_collision_ref = collision;									  // 当たり判定
	}


	bool EnemyZakoBase::DecreaseHP(int damage, const Shared<dxe::Camera> camera) {

		if (_hp > 0) {

			damage = max(damage, 1);

			_hp -= damage;
			return true;
		}

		if (_hp <= 0) {

			//　爆発エフェクト起動
			dxe::DirectXRenderBegin();
			_explode_particle->setPosition(_mesh->pos_);
			_explode_particle->start();
			_explode_particle->render(camera);
			dxe::DirectXRenderEnd();

			//　キルボーナス獲得
			ScoreManager::GetInstance().AddKillBonus(1000);
			_isDead = true;
		}
		return false;
	}

	//　弾---------------------------------------------------------------------------------------------------------------------------
	// 直行弾---------------------------------------------------------------------------------
	void EnemyZakoBase::ShotStraightBullet(const float deltaTime) {

		_straightBullet_count++;

		//_straightBullet_count が　_bullet_fireIntervalの倍数になった時
		if (_straightBullet_count % _bulletFireInterval == 0 && !_straightBullet_queue.empty()) {

			Shared<StraightBullet> bullet = _straightBullet_queue.front();
			_straightBullet_queue.pop_front();

			bullet->_mesh->pos_ = _mesh->pos_;   //　位置
			bullet->_mesh->rot_ = _mesh->rot_;   //　回転

			_straight_bullets.push_back(bullet);
			_straightBullet_count = 0;

			PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);  // 発射音
		}

		ReloadStraightBulletByTimer(deltaTime);           //　リロード
	}

	// 追尾弾---------------------------------------------------------------------------------
	void EnemyZakoBase::ShotHomingBullet(const float deltaTime) {

		_homingBullet_count++;

		//_homingBullet_count が　_bullet_fireIntervalの倍数になった時
		if (_homingBullet_count % _bulletFireInterval == 0 && !_homingBullet_queue.empty()) {

			Shared<HomingBullet> bullet = _homingBullet_queue.front();
			_homingBullet_queue.pop_front();

			bullet->_mesh->pos_ = _mesh->pos_;  //　位置
			bullet->_mesh->rot_ = _mesh->rot_;  //　回転

			_homing_bullets.push_back(bullet);
			_homingBullet_count = 0;

			PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);
		}

		ReloadHomingBulletByTimer(deltaTime);
	}

	void EnemyZakoBase::ReloadStraightBulletByTimer(const float deltaTime)
	{
		if (!_straightBullet_queue.empty()) 
			return;         // 弾がまだあればリターン

		_reloadStraightBullet_timeCounter += deltaTime;

		//　リロードタイマーがまだ溜まっていなければリターン
		if (_reloadStraightBullet_timeCounter < _bulletReloadTimeInterval)
			return;

		std::list<Shared<StraightBullet>> bullets =                 // 弾生成
			_bulletFactory->CreateStraightBullet(
				StraightBullet::USER::ZakoBox,
				_maxBulletSpawnCount
			);

		for (const auto& bullet : bullets) {                        // 弾装填

			_straightBullet_queue.push_back(bullet);
		}

		_reloadStraightBullet_timeCounter = 0.0f;                   // リロードタイマーリセット
	}


	void EnemyZakoBase::ReloadHomingBulletByTimer(const float deltaTime) {              // 追尾弾

		if (!_homingBullet_queue.empty())
			return; // 弾がまだあればリターン

		_reloadHomingBullet_timeCounter += deltaTime;

		//　リロードタイマーがまだ溜まっていなければリターン
		if (_reloadHomingBullet_timeCounter < _bulletReloadTimeInterval)
			return;

		std::list<Shared<HomingBullet>> bullets =                // 弾生成
			_bulletFactory->CreateHomingBullet(
				HomingBullet::USER::ZakoBox,
				_maxBulletSpawnCount
			);

		for (const auto& bullet : bullets) {                     // 弾装填

			_homingBullet_queue.push_back(bullet);
		}

		_reloadHomingBullet_timeCounter = 0.0f;
	}


	void EnemyZakoBase::UpdateStraightBullet(const float deltaTime)              // 直行弾
	{
		auto it_blt = _straight_bullets.begin();

		while (it_blt != _straight_bullets.end()) {

			if ((*it_blt)->_isActive) {

				// 当たり判定　bool
				if (_collision_ref->CheckCollision_EnemyStraightBulletAndPlayer((*it_blt), _player_ref)) {

					//　攻撃力からプレイヤー防御力を引いた分 HPを削る
					if (_player_ref->DecreaseHP(_at - _player_ref->GetDEF())) {

						_player_ref->SetIsInvincible(true);   // 無敵時間
						_player_ref->PlayDamageHitSE();       // ダメージ音
					}

					(*it_blt)->_isActive = false;             // 非アクティブ化
					(*it_blt)->_timer = 0.f;                  // 弾のライフタイマーオフ
				}

				// 弾の寿命を時間で管理
				(*it_blt)->_timer += deltaTime;

				tnl::Vector3 moveDir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
				moveDir.normalize();

				//　更新
				(*it_blt)->_mesh->pos_ += moveDir * _bulletMoveSpeed * deltaTime;


				if ((*it_blt)->_timer > _STRAIGHTBULLET_LIFETIME_LIMIT) {

					(*it_blt)->_isActive = false;            // 非アクティブ化
					(*it_blt)->_timer = 0.f;				 // 弾のライフタイマーオフ
				}
			}
			else {
				//　削除
				it_blt = _straight_bullets.erase(it_blt);
				continue;
			}

			it_blt++;
		}
	}


	void EnemyZakoBase::UpdateHomingBullet(const float deltaTime) {              // 追尾弾

		auto it_blt = _homing_bullets.begin();

		while (it_blt != _homing_bullets.end()) {

			if ((*it_blt)->_isActive) {

				// 当たり判定　bool
				if (_collision_ref->CheckCollision_EnemyHomingBulletAndPlayer((*it_blt), _player_ref)) {

					//　攻撃力からプレイヤー防御力を引いた分 HPを削る
					if (_player_ref->DecreaseHP(_at - _player_ref->GetDEF())) {

						_player_ref->SetIsInvincible(true);
						_player_ref->PlayDamageHitSE();
					}

					(*it_blt)->_isActive = false;
					(*it_blt)->_timer = 0;
				}

				// タイマー起動
				(*it_blt)->_timer += deltaTime;

				// プレイヤーに到達するまでの時間
				float timeToReachPlayer =
					_minTimeToReach + (GetDistanceToPlayer() / 1000) * (_maxTimeToReach - _minTimeToReach);

				// 上の時間を指定の範囲内に制限
				timeToReachPlayer = std::clamp(timeToReachPlayer, _minTimeToReach, _maxTimeToReach);

				tnl::Vector3 targetDir = _player_ref->GetPos() - (*it_blt)->_mesh->pos_;
				targetDir.normalize();

				// 線形補間で弾を旋回させる
				(*it_blt)->_moveDirection = tnl::Vector3::UniformLerp(
					(*it_blt)->_moveDirection,        // 方向
					targetDir * _bulletTurnDelayRate, // 目標地点
					timeToReachPlayer,                // 目標到達時間
					(*it_blt)->_timer                 // 経過時間
				);

				// 更新
				(*it_blt)->_mesh->pos_ +=
					(*it_blt)->_moveDirection * deltaTime * _bulletMoveSpeed / 1.5f;

				//　リセット
				if ((*it_blt)->_timer > _HOMINGBULLET_LIFETIME_LIMIT) {

					(*it_blt)->_isActive = false;
					(*it_blt)->_timer = 0;
				}
			}
			else {

				//　削除
				it_blt = _homing_bullets.erase(it_blt);
				continue;
			}

			it_blt++;
		}
	}

	//　挙動---------------------------------------------------------------------------------------------------------------------------
	void EnemyZakoBase::ChasePlayer(const float deltaTime) {

		//プレイヤー追跡
		tnl::Vector3 direction = _player_ref->GetPos() - _mesh->pos_;

		direction.Normalize(direction);

		_mesh->pos_ += direction * deltaTime * _enemyMoveSpeed;
	}


	void EnemyZakoBase::SearchPlayerMovementState(const float deltaTime)
	{
		//　プレイヤーに気付いている
		if (_isNoticedPlayer) {

			_timeCountFrom_noticedPlayer += deltaTime;

			// プレイヤーに気付いてから、気付いた状態を一定時間確実に保持
			if (_timeCountFrom_noticedPlayer >= _NOTICE_LIMIT_DURATION) {

				_isNoticedPlayer = false;
				_timeCountFrom_noticedPlayer = 0.0f;
			}
		}
		//　プレイヤーに気付いていない
		else {

			static float stateTimer = 0.0f;

			//　移動
			if (_behave == EnemyZakoBase::BEHAVE::Moving) {

				//　時間制限付きでランダムに移動
				//  ランダムな時間経過後停止(5秒以内)
				_isReachedToInvestigatePos = false;

				//　移動
				if (!_isReachedToInvestigatePos) {
					MoveToRandomInvestigatePos(deltaTime);
				}
				//　停止
				if (_isReachedToInvestigatePos) {
					_behave = EnemyZakoBase::BEHAVE::Stop;
				}
				//　移動
				else {
					_isReachedToInvestigatePos = false;
					_behave = EnemyZakoBase::BEHAVE::Moving;
				}
			}
			//　待機
			else if (_behave == EnemyZakoBase::BEHAVE::Stop) {

				//	時には同じ位置にとどまり左右の確認などを行う
				stateTimer += deltaTime;

				//　次の挙動に移るまでの一定時間内であれば
				if (stateTimer > _CHANGE_NEXT_BEHAVE_DURATION) {

					//　３つの挙動から１つをランダムで選択
					_behave = static_cast<EnemyZakoBase::BEHAVE>(rand() % 3);

					if (_behave == EnemyZakoBase::BEHAVE::Turn) {

						_isTurning = true;
					}
					else {
						_isTurning = false;
					}

					stateTimer = 0.f;
				}
				else {
					_behave = EnemyZakoBase::BEHAVE::Stop;
				}
			}
			//　ターン
			else if (_behave == EnemyZakoBase::BEHAVE::Turn) {

				static float angle = 0;

				if (_isTurning) {

					auto randomVector = RandomValueGenerator::Float(-_ROTATION_MAX_RANGE, _ROTATION_MAX_RANGE);

					if (angle == 0) {
						angle = randomVector;
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

					//　ランダムで停止か移動を選択
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


	void EnemyZakoBase::MoveToRandomInvestigatePos(const float deltaTime)
	{
		if (_investigatePos.x == 0 && _investigatePos.y == 0 && _investigatePos.z == 0) {

			auto randomVector = RandomValueGenerator::Vector(
				-_randomInvestigateRange_x, _randomInvestigateRange_x, 
				-_randomInvestigateRange_y, _randomInvestigateRange_y,
				-_randomInvestigateRange_z, _randomInvestigateRange_z
			);

			_investigatePos = randomVector;
		}

		//　移動方向取得
		tnl::Vector3 direction = _investigatePos - _mesh->pos_;
		direction.Normalize(direction);

		//　移動
		_mesh->pos_ += direction * deltaTime * _enemyMoveSpeed;

		// 目的地に近づいたら停止する
		if ((_investigatePos - _mesh->pos_).length() < FLT_DIG) { // FLT_DIG == 6

			// 原点へ戻る
			_investigatePos = { 0, 0, 0 };
			_isReachedToInvestigatePos = true;
		}
	}


	void EnemyZakoBase::AttackPlayer(const float deltaTime) {

		if (_isShotStraightBullet) {              // 直行弾

			ShotStraightBullet(deltaTime);

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
		else if (_isShotHomingBullet) {           // 追尾弾

			ShotHomingBullet(deltaTime);

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


	void EnemyZakoBase::DoRoutineMoves(const float deltaTime) {

		// 距離 250～270内で、プレイヤーHPが０でなければプレイヤー追跡
		if (GetDistanceToPlayer() < GetIdleDistance()
			&& GetDistanceToPlayer() > GetAttackDistance()
			&& _player_ref->GetHP() != 0 || _isNoticedPlayer) {

			LookAtPlayer();

			if (!_isAttacking)
				ChasePlayer(deltaTime);
		}

		// 250以内でプレイヤーHPが０でなければ攻撃
		if (GetDistanceToPlayer() < GetAttackDistance() && _player_ref->GetHP() != 0) {

			LookAtPlayer();

			_isAttacking = true;
			AttackPlayer(deltaTime);
		}
		// アイドル状態
		else {
			_isAttacking = false;
			SearchPlayerMovementState(deltaTime);
		}
	}


	bool EnemyZakoBase::ShowHpGage_EnemyZako() {

		if (_hp <= 0) return false;

		tnl::Vector3 hpGage_pos = tnl::Vector3::ConvertToScreen
		(
			_mesh->pos_,
			DXE_WINDOW_WIDTH,
			DXE_WINDOW_HEIGHT,
			_enemyCamera->view_,
			_enemyCamera->proj_
		);

		float x1 = hpGage_pos.x - 30;
		float x2 = hpGage_pos.x + 30;

		float gage_width = abs(x2 - x1);

		float average = (_MAX_HP > 0) ? gage_width / _MAX_HP : 0;

		x2 = x1 + static_cast<int>(average * _hp);

		DrawBoxAA(x1, hpGage_pos.y - 30, x2, hpGage_pos.y - 25, GetColor(255, 0, 0), true);

		return true;
	}


	void EnemyZakoBase::Render(const Shared<dxe::Camera> camera) {

		if (_isDead) return;

		_mesh->render(camera);

		ShowHpGage_EnemyZako();

		for (const auto& blt : _straight_bullets) {

			blt->Render(camera);
		}
		for (const auto& blt : _homing_bullets) {

			blt->Render(camera);
		}
	}


	bool EnemyZakoBase::Update(const float deltaTime) {

		if (_isDead) return false;

		DoRoutineMoves(deltaTime);

		UpdateStraightBullet(deltaTime);
		UpdateHomingBullet(deltaTime);

		return true;
	}
}