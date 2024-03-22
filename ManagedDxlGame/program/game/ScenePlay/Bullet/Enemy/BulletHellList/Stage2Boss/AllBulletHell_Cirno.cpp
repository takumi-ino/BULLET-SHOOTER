#include "../../BulletHell.h"
#include "../../BulletHellsBulletNumInfo.h"
#include "../../EnemyBullet.h"
#include "../../../../Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "AllBulletHell_Cirno.h"
#include "../../../../RandomValue/RandomValueGenerator.h"

/*
�@�@�@�@�E�֐��̐錾�́@BulletHell.h�@�ɂ���܂��B
  �@�@  �E�����ł̓X�e�[�W�Q�{�X�A�u�`���m�v�̒e���֐���`�����Ă��܂��B
*/


namespace inl {

	//�@�ʏ�U��-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_Normal_Cirno(const float deltaTime) {

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		for (int i = 0; i < EnemyBoss_Cirno::_bullet_normal_cirno.size(); i++) {

			auto bullet = EnemyBoss_Cirno::_bullet_normal_cirno[i];

			//�@�p�^�[��1�@�V���b�g�K���i2 + N)�s (N��0�`5�܂�)�A�U��̒e���v���C���[�֔��� (81���j
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_Blue) {

				const int BULLET_WAVE_COUNT = 3;
				const int BULLET_NUM_EACH_COLUMN = 8;
				const float OFFSET = 0.1f;

				// ���ˊԊu�������邽�߂̔g������
				int wave = bullet->_id / (STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO / BULLET_WAVE_COUNT);

				// �^�C�}�[�N��
				bullet->_timer += deltaTime;

				//�@�e�g�̔��ˊԊu���󂯂�I�t�Z�b�g
				float startTiming = OFFSET * (bullet->_id / BULLET_NUM_EACH_COLUMN);

				switch (wave)
				{
				case 0:
					WaveAssist_Normal_Cirno(bullet, startTiming, 0.0f, deltaTime);
					break;
				case 1:
					WaveAssist_Normal_Cirno(bullet, startTiming, 2.0f, deltaTime);
					break;
				case 2:
					WaveAssist_Normal_Cirno(bullet, startTiming, 4.0f, deltaTime);
					break;
				}
			}

			// �p�^�[���Q�@�S���ʒe �P��ɂ�16��
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {

				const float RADIUS = 100.0f;         // ���a
				const float LIFETIME_LIMIT = 2.0f;   // ����
				const float BULLET_SPEED = 500.0f;   // ����

				const int BULLETNUM_EACH_WAVE = 16;  // �P�x�Ɍ��e�̐�
				const float OFFSET = 0.5f;           // ���ˊԊu�����W��

				//�@���ˊԊu
				float startTiming = OFFSET * (bullet->_id / BULLETNUM_EACH_WAVE);

				//�@�p�x
				float angle = (2.0f * tnl::PI / BULLETNUM_EACH_WAVE) * (bullet->_id % BULLETNUM_EACH_WAVE);

				//�@������
				if (bullet->_timer == 0.f) {

					tnl::Vector3 moveDir, spawnPos;

					//�@�����ʒu
					spawnPos.x = bossPosition.x + RADIUS * cos(angle);
					spawnPos.y = bossPosition.y;
					spawnPos.z = bossPosition.z + RADIUS * sin(angle);

					//�@����
					moveDir.x = cos(angle) * RADIUS;
					moveDir.z = sin(angle) * RADIUS;

					moveDir.normalize();

					bullet->_mesh->pos_ = spawnPos;

					bullet->_moveDirection = moveDir;

					bullet->_isActive = true;
				}

				// �^�C�}�[�N��
				bullet->_timer += deltaTime;

				//�@���Z�b�g
				if (bullet->_isActive &&
					bullet->_timer >= startTiming &&
					bullet->_timer > LIFETIME_LIMIT)
				{

					bullet->_isActive = false;
					bullet->_timer = 0;

					//�@�ʒu
					bullet->_mesh->pos_.x = bossPosition.x + RADIUS * cos(angle);
					bullet->_mesh->pos_.y = bossPosition.y;
					bullet->_mesh->pos_.z = bossPosition.z + RADIUS * sin(angle);

					tnl::Vector3 moveDir;

					//�@����
					moveDir.x = cos(angle) * RADIUS;
					moveDir.z = sin(angle) * RADIUS;
					moveDir.normalize();

					bullet->_moveDirection = moveDir;
				}

				// �X�V
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
			}
		}
	}

	//�@�ʏ�U���A�V�X�g�֐�--------------------------------------------------------------------------------------------------------------
	void BulletHell::WaveAssist_Normal_Cirno(
		Shared<EnemyBullet>& bullet,
		const float timing,
		const float delayOffset,
		const float deltaTime)
	{
		const float WIDTH = 70.0f;         // �e�P�s����̗�̍ő啝
		const float DEFAULT_LIMIT = 4.0f;  // �e�̃��Z�b�g��^�C�~���O
		const float BULLET_SPEED = 500.f;  // �e�̑���

		//�@�e���ƂɎ����X�V
		if (bullet->_timer >= timing + delayOffset) {

			bullet->_isActive = true;

			tnl::Vector3 spawn_pos = _bossMesh_ref->pos_;             // �����ʒu
			tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos; // ����
			float angle = (tnl::PI / 8) * (bullet->_id % 8);          // �p�x

			moveDir.x += cos(angle) * WIDTH;

			moveDir.normalize();

			bullet->_moveDirection = moveDir;

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}

		//�@���Z�b�g
		if (bullet->_timer > DEFAULT_LIMIT + delayOffset) {

			bullet->_isActive = false;
			bullet->_timer = 0.f;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}


	//�@����U���P-----------------------------------------------------------------------------------------------------------------

	float assistTimer_icicleFall_straightBlue_cirno = 0.f;
	float assistTimer_icicleFall_straightYellow_cirno = 0.f;

	void BulletHell::ShotBulletHell_IcicleFall_Cirno(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_Cirno::_bullet_icicleFall_cirno.size(); i++) {
			auto bullet = EnemyBoss_Cirno::_bullet_icicleFall_cirno[i];

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			float angleToPlayer =    // �v���C���[�Ƃ̊p�x
				atan2(_player_ref->GetPos().z - bossPosition.z, 
					_player_ref->GetPos().x - bossPosition.x
				);


			// 	�p�^�[��1�@���E�ɔ��ˌ�A�����֕����]������e
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

				const float LIFETIME_LIMIT = 2.0f;
				const float ANGLE_OFFSET = 45.f;

				const float BULLET_SPEED = 20.f;
				const float DELAY_OFFSET = 15.f;
				const int BULLET_NUM_EACHLINE = 10;

				float angleOffset =    // �e�����ƉE��10���O���[�v�ɕ����čX�V
					(bullet->_id >= 10 && bullet->_id < 20) ||    // 0�`20
					(bullet->_id >= 30 && bullet->_id < 40) ||	  // 20�`40
					(bullet->_id >= 50 && bullet->_id < 60) ||	  // 40�`60
					(bullet->_id >= 70 && bullet->_id < 89) ||	  // 60�`80
					(bullet->_id >= 100 && bullet->_id < 109) ||  // 80�`100
					(bullet->_id >= 110 && bullet->_id < 119) ?	  // 100�`120
					tnl::ToRadian(angleToPlayer - ANGLE_OFFSET) :
					tnl::ToRadian(angleToPlayer + ANGLE_OFFSET);

				//�@�^�C�}�[�N��
				assistTimer_icicleFall_straightBlue_cirno += deltaTime;


				// �v���C���[���������E�̃I�t�Z�b�g���������p�x���擾
				float newAngle = angleToPlayer + angleOffset;


				//�@��P�g
				if (assistTimer_icicleFall_straightBlue_cirno > 0.f) {

					// 0 �` 19 �̒e-------------------------------------------------------------------------
					if (bullet->_id < 20) {

						// �p�x
						float radius =
							(2.f * tnl::PI) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							BULLET_SPEED;

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						bullet->_timer += deltaTime;

						//�@�X�V
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 1.0f, deltaTime);
					}
				}

				const float RADIUS_OFFSET = 0.5f;

				//�@��Q�g
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET) {

					// 20 �` 39 �̒e---------------------------------------------------------------------------------
					if (bullet->_id >= 20 && bullet->_id < 40) {

						// �p�x
						float radius =
							(2.f * tnl::PI / RADIUS_OFFSET) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 3.f);

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 1.5f, deltaTime);
					}
				}

				//�@��R�g
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 2.f) {

					// 40 �` 59 �̒e----------------------------------------------------------------------------------
					if (bullet->_id >= 40 && bullet->_id < 60) {

						// �p�x
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 2.f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 5.f);

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 2.0f, deltaTime);
					}
				}

				//�@��S�g
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 3.f) {

					// 60 �` 79 �̒e-----------------------------------------------------------------------------------
					if (bullet->_id >= 60 && bullet->_id < 80) {

						// �p�x
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 2.5f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 7.5f);

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 2.5f, deltaTime);
					}
				}

				//�@��T�g
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 4.f) {

					// 80 �` 99 �̒e----------------------------------------------------------------------------------
					if (bullet->_id >= 90 && bullet->_id < 109) {

						// �p�x
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 3.0f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 10.f);

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 3.0f, deltaTime);
					}
				}

				//�@��U�g
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 5.f) {

					// 100 �` 119 �̒e---------------------------------------------------------------------------------
					if (bullet->_id >= 100 && bullet->_id < 119) {

						// �p�x
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 3.5f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 12.5f);

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 3.5f, deltaTime);
					}
				}
			}

			// 	�p�^�[��2�@�P�s�T��̒e���v���C���[�֏Ǝˁi�R�Z�b�g�j
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow) {

				const float BULLET_SPEED = 400.0f;   //�@�e�̑���
				const float FIRE_INTERVAL = 5.0f;    //�@���ˊԊu
				const float DELAY_OFFSET = 10.f;     //�@�x�������p�W��


				//�@�g���Ǘ�����^�C�}�[
				assistTimer_icicleFall_straightYellow_cirno += deltaTime;

				//�@��P�g---------------------------------------------------------
				if (assistTimer_icicleFall_straightYellow_cirno > 0.f) {

					// id 0�`5�̒e---------------------------------------------------------------------
					if (bullet->_id < 5) {

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightYellow_Cirno(bullet);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
					}
				}

				//�@��Q�g---------------------------------------------------------
				if (assistTimer_icicleFall_straightYellow_cirno > DELAY_OFFSET) {

					// id 5�`10�̒e---------------------------------------------------------------------
					if (bullet->_id >= 5 && bullet->_id < 10) {

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightYellow_Cirno(bullet);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
					}
				}

				//�@��R�g---------------------------------------------------------
				if (assistTimer_icicleFall_straightYellow_cirno > DELAY_OFFSET * 2.f) {

					// id 10�`15�̒e---------------------------------------------------------------------
					if (bullet->_id >= 10 && bullet->_id < 15) {

						// ������
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightYellow_Cirno(bullet);
						}

						//�@�X�V
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
					}
				}
			}
		}
	}


	//�@����U���P�A�V�X�g�i�������A���e�j------------------------------------------------------------------------
	void BulletHell::InitAssist_IcicleFall_StraightBlue_Cirno(
		Shared<EnemyBullet>& bullet,
		const float radius,
		const float newAngle)
	{
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// �{�X�̐��ʍ��W�擾�@xz����
		tnl::Vector3 bossForward = { cos(_bossMesh_ref->rot_.y), 0, sin(_bossMesh_ref->rot_.y) };

		tnl::Vector3 moveDir, spawnPos;

		spawnPos = bossPosition + bossForward * radius * cos(newAngle);
		spawnPos.x += radius * cos(newAngle);
		spawnPos.z += radius * sin(newAngle);

		moveDir += tnl::Vector3{ cos(newAngle), 0 , sin(newAngle) };
		moveDir.normalize();

		bullet->_mesh->pos_ = spawnPos;    // �ʒu

		bullet->_moveDirection = moveDir;  // ����

		bullet->_isActive = true;          // �A�N�e�B�u���
	}


	//�@����U���P�A�V�X�g�i�X�V�A���e�j------------------------------------------------------------------------
	void BulletHell::UpdateAssist_IcicleFall_StraightBlue_Cirno(
		Shared<EnemyBullet>& bullet,
		const float radius,
		const float newAngle,
		const float startChangeDirTime,
		const float deltaTime)
	{
		const float BULLET_SPEED = 200.0f;   //�@�e�̑���
		const float BULLET_LIFETIME = 3.0f;  //�@����

		if (bullet->_timer > startChangeDirTime &&
			bullet->_timer < startChangeDirTime + 0.1f)
		{
			//�@�v���C���[�����֋O���C��
			tnl::Vector3 changeDir = _player_ref->GetPos() - bullet->_mesh->pos_;
			changeDir += tnl::Vector3{ radius * cos(newAngle), 0 , radius * sin(newAngle) };

			changeDir.normalize();

			bullet->_moveDirection = changeDir;
			bullet->_moveDirection.normalize();
		}

		//�@���Z�b�g
		if (bullet->_timer > startChangeDirTime + BULLET_LIFETIME)
		{
			bullet->_isActive = false;
			bullet->_timer = 0;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}

		//�@�X�V
		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
	}


	//�@����U���P�A�V�X�g�i�������A���F���e�j------------------------------------------------------------------------
	void BulletHell::InitAssist_IcicleFall_StraightYellow_Cirno(Shared<EnemyBullet>& bullet)
	{
		const int BULLETS_PER_ROW = 5;
		const float BULLET_SPACING = 50.0f;  //�@�e�ǂ����̂̊Ԋu

		// �v���C���[�����擾
		tnl::Vector3 direction = _player_ref->GetPos() - _bossMesh_ref->pos_;
		direction.normalize();
		bullet->_moveDirection = direction;

		// �����ȊO�̒e�����E�ɂ����悤�ɒ���
		int column = bullet->_id % BULLETS_PER_ROW;
		float offset = (column - (BULLETS_PER_ROW / 2)) * BULLET_SPACING;

		// �{�X�̌����Ă�������ɍ��킹��
		tnl::Vector3 sideOffset = tnl::Vector3::TransformCoord({ offset,0,0 }, _bossMesh_ref->rot_);

		bullet->_mesh->pos_ = _bossMesh_ref->pos_ + sideOffset;
		bullet->_isActive = true;
	}


	//�@����U���P�A�V�X�g�i�X�V�A���F���e�j------------------------------------------------------------------------
	void BulletHell::UpdateAssist_IcicleFall_StraightYellow_Cirno(
		Shared<EnemyBullet>& bullet,
		const float deltaTime,
		const float bulletSpeed)
	{
		const float BULLET_LIFETIME = 4.0f;

		//�@�X�V
		if (bullet->_isActive) {

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
		}

		//�@���Z�b�g
		if (bullet->_timer > BULLET_LIFETIME) {

			bullet->_isActive = false;
			bullet->_timer = 0.f;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}


	//�@����U���Q--------------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_PerfectFreeze_Cirno(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.size(); i++) {

			auto bullet = EnemyBoss_Cirno::_bullet_perfectFreeze_cirno[i];
			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			//�@�p�^�[���P�@�@�X�t�B�A�A�~����A���@�@�{�X�̎��͂Ƀ����_���ɒe�𐶐�
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_White) {

				const float RADIUS = 150.0f;                      //�@���a
				const float BULLET_SPEED = 200.0f;                //�@����
				const float BULLET_LIFETIME = 8.0f;               //�@����

				float theta = (2.0f * tnl::PI) * (bullet->_id);   // ���ʊp
				float phi = tnl::PI * bullet->_id;                // �V���p

				//�@������
				if (bullet->_timer == 0.f) {

					tnl::Vector3 spawnPos = bossPosition;

					//�@�����ʒu
					spawnPos.x += sin(phi) * cos(theta);
					spawnPos.y += sin(phi) * sin(theta);
					spawnPos.z += sin(theta);
					spawnPos += RandomValueGenerator::Vector(-250, 50, -250, 50, -150, 150);

					tnl::Vector3 moveDirection = _player_ref->GetPos() - bullet->_mesh->pos_;

					//�@����
					moveDirection.x = cos(theta) * RADIUS;
					moveDirection.y = sin(theta) * RADIUS;
					moveDirection.z = sin(theta) * RADIUS;
					moveDirection += RandomValueGenerator::Vector(-250, 50, -150, 150, -250, 50);
					moveDirection.normalize();

					bullet->_moveDirection = moveDirection;
					bullet->_mesh->pos_ = spawnPos;
					bullet->_isActive = true;
				}

				//�@��~����
				const float STOPPING_DURATION = 2.f;

				//�@�^�C�}�[�N��
				bullet->_timer += deltaTime;

				if (bullet->_timer < STOPPING_DURATION) {

					//�@�X�V
					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 0.1f;
				}
				else if (bullet->_timer > STOPPING_DURATION && bullet->_timer < STOPPING_DURATION * 2.f) {

					// �e���ꎞ��~
					bullet->_moveDirection = tnl::Vector3(0, 0, 0);
				}
				//�@�ړ��ĊJ
				else if (static_cast<int>(bullet->_timer) == 5) {

					float angle = (2.0f * tnl::PI) * (bullet->_id);

					tnl::Vector3 moveDirection = _player_ref->GetPos() - bullet->_mesh->pos_;

					moveDirection.x += sin(phi) * cos(angle) * RADIUS;
					moveDirection.y += sin(phi) * sin(angle) * RADIUS;
					moveDirection.z += sin(angle) * RADIUS;
					moveDirection += RandomValueGenerator::Vector(-250, 50, -150, 150, -250, 50);
					moveDirection.normalize();

					bullet->_moveDirection = moveDirection;
				}

				// �X�V
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;

				// ���Z�b�g
				if (bullet->_timer > BULLET_LIFETIME) {
					bullet->_isActive = false;

					bullet->_timer = 0.f;
				}
			}

			//�@�p�^�[���Q�@�@�X�t�B�A�A���s�e�A�@�@�v���C���[�ւP�s�W��̒e��A��
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

				tnl::Vector3 spawnPos = bossPosition;
				const int BULLET_NUM_EACH_LINE = 8;

				// ������
				if (bullet->_timer == 0.f) {

					//�@�p�x
					float theta = (tnl::PI / BULLET_NUM_EACH_LINE) * (bullet->_id % BULLET_NUM_EACH_LINE);

					//�@����
					tnl::Vector3 moveDir = _player_ref->GetPos() - spawnPos;
					moveDir.x += cos(theta) * 70;
					moveDir.normalize();

					bullet->_moveDirection = moveDir;

					bullet->_mesh->pos_ = spawnPos;
				}

				//�@�^�C�}�[�N��
				bullet->_timer += deltaTime;

				const float LIFE_TIME = 4.f;
				const float START_OFFSET = 0.1f;
				const float BULLET_SPEED = 500.f;

				float startTiming = START_OFFSET * (bullet->_id / BULLET_NUM_EACH_LINE);

				// �X�V�J�n
				if (bullet->_timer >= startTiming) {

					bullet->_isActive = true;
					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
				}

				//�@���Z�b�g
				if (bullet->_timer > LIFE_TIME) {

					bullet->_isActive = false;
					bullet->_timer = 0.f;
					bullet->_mesh->pos_ = spawnPos;
				}
			}
		}
	}
}