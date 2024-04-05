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


	// �G���G�l�~�[�f�[�^�ǂݎ��
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
		_MAX_HP = data._hp;											  // �ő�HP
		_name = data._name;											  // ���O
		_scale = data._scale;										  // �T�C�Y
		_enemyMoveSpeed = data._enemyMoveSpeed;						  // �ړ��X�s�[�h
		_maxBulletSpawnCount = data._maxBulletSpawnCount;			  // �e�̍ő吶����
		_maxTotalEnemy_spawnCount = data._maxTotalEnemy_spawnCount;	  // �G�̑���
		_bulletMoveSpeed = data._bulletMoveSpeed;					  // �e�̃X�s�[�h
		_bulletFireInterval = data._bulletFireInterval;				  // �e�̌��Ԋu
		_bulletReloadTimeInterval = data._bulletReloadTimeInterval;	  // �e�̃����[�h�Ԋu

		_player_ref = player;										  // �v���C���[
		_enemyCamera = camera;										  // �J����
		_collision_ref = collision;									  // �����蔻��
	}


	bool EnemyZakoBase::DecreaseHP(int damage, const Shared<dxe::Camera> camera) {

		if (_hp > 0) {

			damage = max(damage, 1);

			_hp -= damage;
			return true;
		}

		if (_hp <= 0) {

			//�@�����G�t�F�N�g�N��
			dxe::DirectXRenderBegin();
			_explode_particle->setPosition(_mesh->pos_);
			_explode_particle->start();
			_explode_particle->render(camera);
			dxe::DirectXRenderEnd();

			//�@�L���{�[�i�X�l��
			ScoreManager::GetInstance().AddKillBonus(1000);
			_isDead = true;
		}
		return false;
	}

	//�@�e---------------------------------------------------------------------------------------------------------------------------
	// ���s�e---------------------------------------------------------------------------------
	void EnemyZakoBase::ShotStraightBullet(const float deltaTime) {

		_straightBullet_count++;

		//_straightBullet_count ���@_bullet_fireInterval�̔{���ɂȂ�����
		if (_straightBullet_count % _bulletFireInterval == 0 && !_straightBullet_queue.empty()) {

			Shared<StraightBullet> bullet = _straightBullet_queue.front();
			_straightBullet_queue.pop_front();

			bullet->_mesh->pos_ = _mesh->pos_;   //�@�ʒu
			bullet->_mesh->rot_ = _mesh->rot_;   //�@��]

			_straight_bullets.push_back(bullet);
			_straightBullet_count = 0;

			PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);  // ���ˉ�
		}

		ReloadStraightBulletByTimer(deltaTime);           //�@�����[�h
	}

	// �ǔ��e---------------------------------------------------------------------------------
	void EnemyZakoBase::ShotHomingBullet(const float deltaTime) {

		_homingBullet_count++;

		//_homingBullet_count ���@_bullet_fireInterval�̔{���ɂȂ�����
		if (_homingBullet_count % _bulletFireInterval == 0 && !_homingBullet_queue.empty()) {

			Shared<HomingBullet> bullet = _homingBullet_queue.front();
			_homingBullet_queue.pop_front();

			bullet->_mesh->pos_ = _mesh->pos_;  //�@�ʒu
			bullet->_mesh->rot_ = _mesh->rot_;  //�@��]

			_homing_bullets.push_back(bullet);
			_homingBullet_count = 0;

			PlaySoundMem(_shotSE_hdl, DX_PLAYTYPE_BACK);
		}

		ReloadHomingBulletByTimer(deltaTime);
	}

	void EnemyZakoBase::ReloadStraightBulletByTimer(const float deltaTime)
	{
		if (!_straightBullet_queue.empty()) 
			return;         // �e���܂�����΃��^�[��

		_reloadStraightBullet_timeCounter += deltaTime;

		//�@�����[�h�^�C�}�[���܂����܂��Ă��Ȃ���΃��^�[��
		if (_reloadStraightBullet_timeCounter < _bulletReloadTimeInterval)
			return;

		std::list<Shared<StraightBullet>> bullets =                 // �e����
			_bulletFactory->CreateStraightBullet(
				StraightBullet::USER::ZakoBox,
				_maxBulletSpawnCount
			);

		for (const auto& bullet : bullets) {                        // �e���U

			_straightBullet_queue.push_back(bullet);
		}

		_reloadStraightBullet_timeCounter = 0.0f;                   // �����[�h�^�C�}�[���Z�b�g
	}


	void EnemyZakoBase::ReloadHomingBulletByTimer(const float deltaTime) {              // �ǔ��e

		if (!_homingBullet_queue.empty())
			return; // �e���܂�����΃��^�[��

		_reloadHomingBullet_timeCounter += deltaTime;

		//�@�����[�h�^�C�}�[���܂����܂��Ă��Ȃ���΃��^�[��
		if (_reloadHomingBullet_timeCounter < _bulletReloadTimeInterval)
			return;

		std::list<Shared<HomingBullet>> bullets =                // �e����
			_bulletFactory->CreateHomingBullet(
				HomingBullet::USER::ZakoBox,
				_maxBulletSpawnCount
			);

		for (const auto& bullet : bullets) {                     // �e���U

			_homingBullet_queue.push_back(bullet);
		}

		_reloadHomingBullet_timeCounter = 0.0f;
	}


	void EnemyZakoBase::UpdateStraightBullet(const float deltaTime)              // ���s�e
	{
		auto it_blt = _straight_bullets.begin();

		while (it_blt != _straight_bullets.end()) {

			if ((*it_blt)->_isActive) {

				// �����蔻��@bool
				if (_collision_ref->CheckCollision_EnemyStraightBulletAndPlayer((*it_blt), _player_ref)) {

					//�@�U���͂���v���C���[�h��͂��������� HP�����
					if (_player_ref->DecreaseHP(_at - _player_ref->GetDEF())) {

						_player_ref->SetIsInvincible(true);   // ���G����
						_player_ref->PlayDamageHitSE();       // �_���[�W��
					}

					(*it_blt)->_isActive = false;             // ��A�N�e�B�u��
					(*it_blt)->_timer = 0.f;                  // �e�̃��C�t�^�C�}�[�I�t
				}

				// �e�̎��������ԂŊǗ�
				(*it_blt)->_timer += deltaTime;

				tnl::Vector3 moveDir = tnl::Vector3::TransformCoord({ 0,0,1 }, _mesh->rot_);
				moveDir.normalize();

				//�@�X�V
				(*it_blt)->_mesh->pos_ += moveDir * _bulletMoveSpeed * deltaTime;


				if ((*it_blt)->_timer > _STRAIGHTBULLET_LIFETIME_LIMIT) {

					(*it_blt)->_isActive = false;            // ��A�N�e�B�u��
					(*it_blt)->_timer = 0.f;				 // �e�̃��C�t�^�C�}�[�I�t
				}
			}
			else {
				//�@�폜
				it_blt = _straight_bullets.erase(it_blt);
				continue;
			}

			it_blt++;
		}
	}


	void EnemyZakoBase::UpdateHomingBullet(const float deltaTime) {              // �ǔ��e

		auto it_blt = _homing_bullets.begin();

		while (it_blt != _homing_bullets.end()) {

			if ((*it_blt)->_isActive) {

				// �����蔻��@bool
				if (_collision_ref->CheckCollision_EnemyHomingBulletAndPlayer((*it_blt), _player_ref)) {

					//�@�U���͂���v���C���[�h��͂��������� HP�����
					if (_player_ref->DecreaseHP(_at - _player_ref->GetDEF())) {

						_player_ref->SetIsInvincible(true);
						_player_ref->PlayDamageHitSE();
					}

					(*it_blt)->_isActive = false;
					(*it_blt)->_timer = 0;
				}

				// �^�C�}�[�N��
				(*it_blt)->_timer += deltaTime;

				// �v���C���[�ɓ��B����܂ł̎���
				float timeToReachPlayer =
					_minTimeToReach + (GetDistanceToPlayer() / 1000) * (_maxTimeToReach - _minTimeToReach);

				// ��̎��Ԃ��w��͈͓̔��ɐ���
				timeToReachPlayer = std::clamp(timeToReachPlayer, _minTimeToReach, _maxTimeToReach);

				tnl::Vector3 targetDir = _player_ref->GetPos() - (*it_blt)->_mesh->pos_;
				targetDir.normalize();

				// ���`��ԂŒe����񂳂���
				(*it_blt)->_moveDirection = tnl::Vector3::UniformLerp(
					(*it_blt)->_moveDirection,        // ����
					targetDir * _bulletTurnDelayRate, // �ڕW�n�_
					timeToReachPlayer,                // �ڕW���B����
					(*it_blt)->_timer                 // �o�ߎ���
				);

				// �X�V
				(*it_blt)->_mesh->pos_ +=
					(*it_blt)->_moveDirection * deltaTime * _bulletMoveSpeed / 1.5f;

				//�@���Z�b�g
				if ((*it_blt)->_timer > _HOMINGBULLET_LIFETIME_LIMIT) {

					(*it_blt)->_isActive = false;
					(*it_blt)->_timer = 0;
				}
			}
			else {

				//�@�폜
				it_blt = _homing_bullets.erase(it_blt);
				continue;
			}

			it_blt++;
		}
	}

	//�@����---------------------------------------------------------------------------------------------------------------------------
	void EnemyZakoBase::ChasePlayer(const float deltaTime) {

		//�v���C���[�ǐ�
		tnl::Vector3 direction = _player_ref->GetPos() - _mesh->pos_;

		direction.Normalize(direction);

		_mesh->pos_ += direction * deltaTime * _enemyMoveSpeed;
	}


	void EnemyZakoBase::SearchPlayerMovementState(const float deltaTime)
	{
		//�@�v���C���[�ɋC�t���Ă���
		if (_isNoticedPlayer) {

			_timeCountFrom_noticedPlayer += deltaTime;

			// �v���C���[�ɋC�t���Ă���A�C�t������Ԃ���莞�Ԋm���ɕێ�
			if (_timeCountFrom_noticedPlayer >= _NOTICE_LIMIT_DURATION) {

				_isNoticedPlayer = false;
				_timeCountFrom_noticedPlayer = 0.0f;
			}
		}
		//�@�v���C���[�ɋC�t���Ă��Ȃ�
		else {

			static float stateTimer = 0.0f;

			//�@�ړ�
			if (_behave == EnemyZakoBase::BEHAVE::Moving) {

				//�@���Ԑ����t���Ń����_���Ɉړ�
				//  �����_���Ȏ��Ԍo�ߌ��~(5�b�ȓ�)
				_isReachedToInvestigatePos = false;

				//�@�ړ�
				if (!_isReachedToInvestigatePos) {
					MoveToRandomInvestigatePos(deltaTime);
				}
				//�@��~
				if (_isReachedToInvestigatePos) {
					_behave = EnemyZakoBase::BEHAVE::Stop;
				}
				//�@�ړ�
				else {
					_isReachedToInvestigatePos = false;
					_behave = EnemyZakoBase::BEHAVE::Moving;
				}
			}
			//�@�ҋ@
			else if (_behave == EnemyZakoBase::BEHAVE::Stop) {

				//	���ɂ͓����ʒu�ɂƂǂ܂荶�E�̊m�F�Ȃǂ��s��
				stateTimer += deltaTime;

				//�@���̋����Ɉڂ�܂ł̈�莞�ԓ��ł����
				if (stateTimer > _CHANGE_NEXT_BEHAVE_DURATION) {

					//�@�R�̋�������P�������_���őI��
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
			//�@�^�[��
			else if (_behave == EnemyZakoBase::BEHAVE::Turn) {

				static float angle = 0;

				if (_isTurning) {

					auto randomVector = RandomValueGenerator::Float(-_ROTATION_MAX_RANGE, _ROTATION_MAX_RANGE);

					if (angle == 0) {
						angle = randomVector;
					}
					else {
						if (angle > 0) {
							angle -= 1e-10f; // 0�x���傫����΍��֋͂��ɉ�]
						}
						else {
							angle += 1e-10f; // 0�x��菬������ΉE�֋͂��ɉ�]
						}
					}

					//�����_���ȕ����֌�����ς���
					_mesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(angle));
				}

				if (angle != 0) {

					//�@�����_���Œ�~���ړ���I��
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

		//�@�ړ������擾
		tnl::Vector3 direction = _investigatePos - _mesh->pos_;
		direction.Normalize(direction);

		//�@�ړ�
		_mesh->pos_ += direction * deltaTime * _enemyMoveSpeed;

		// �ړI�n�ɋ߂Â������~����
		if ((_investigatePos - _mesh->pos_).length() < FLT_DIG) { // FLT_DIG == 6

			// ���_�֖߂�
			_investigatePos = { 0, 0, 0 };
			_isReachedToInvestigatePos = true;
		}
	}


	void EnemyZakoBase::AttackPlayer(const float deltaTime) {

		if (_isShotStraightBullet) {              // ���s�e

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
		else if (_isShotHomingBullet) {           // �ǔ��e

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

		// ���� 250�`270���ŁA�v���C���[HP���O�łȂ���΃v���C���[�ǐ�
		if (GetDistanceToPlayer() < GetIdleDistance()
			&& GetDistanceToPlayer() > GetAttackDistance()
			&& _player_ref->GetHP() != 0 || _isNoticedPlayer) {

			LookAtPlayer();

			if (!_isAttacking)
				ChasePlayer(deltaTime);
		}

		// 250�ȓ��Ńv���C���[HP���O�łȂ���΍U��
		if (GetDistanceToPlayer() < GetAttackDistance() && _player_ref->GetHP() != 0) {

			LookAtPlayer();

			_isAttacking = true;
			AttackPlayer(deltaTime);
		}
		// �A�C�h�����
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