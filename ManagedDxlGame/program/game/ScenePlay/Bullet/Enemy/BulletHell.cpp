#include "BulletHell.h"
#include "BulletHellFactory.h"
#include "EnemyBullet.h"
#include "../../ScenePlay.h"
#include "../../Character/Player/Player.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"


// �X�e�[�W1�{�X�i�p�`�����[�j--------------------------------------------------------------------------------------------------

void BulletHell::ShotBulletHell_Normal_Patchouli(const float deltaTime) {

	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

	// ���ꂼ��̃^�C�v�̒e����ʁX�ɏ������邽�߁A�����x�N�^�[����v�f�̃O���[�v����������
	static std::vector<Shared<EnemyBullet>> sphereRoundBlue, cylinderRoundBlue, sphereRoundRed;

	if (sphereRoundBlue.empty() && cylinderRoundBlue.empty() && sphereRoundRed.empty()) {

		for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.size(); i++) {
			auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli[i];

			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {
				sphereRoundBlue.push_back(bullet);
			}
			else if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {
				cylinderRoundBlue.push_back(bullet);
			}
			else {
				sphereRoundRed.push_back(bullet);
			}
		}
	}

	const float roundBullet_radius = 90.0f;
	const float roundBullet_moveSpeed = 100.0f;
	tnl::Vector3 roundBullet_moveDirection;

	// �p�^�[���P�@�{�X�̎��͂��ʒu��]������e
	for (auto& bullet : sphereRoundBlue) {

		// ����]
		if (bullet->_id % 2 == 0)
			bullet->_angle += deltaTime;

		// �E��]
		if (bullet->_id % 2 != 0)
			bullet->_angle -= deltaTime;

		// X
		bullet->_mesh->pos_.x = bossPosition.x + roundBullet_radius * cos(bullet->_angle);
		// Y
		bullet->_mesh->pos_.y = bossPosition.y;
		// Z
		bullet->_mesh->pos_.z = bossPosition.z + roundBullet_radius * sin(bullet->_angle);

		// ��������
		roundBullet_moveDirection.x =
			bossPosition.x + cos(bullet->_angle) * roundBullet_radius;
		roundBullet_moveDirection.z =
			bossPosition.z + sin(bullet->_angle) * roundBullet_radius;

		roundBullet_moveDirection.normalize();

		bullet->_mesh->pos_.x += roundBullet_moveDirection.x * deltaTime * roundBullet_moveSpeed;
		bullet->_mesh->pos_.z += roundBullet_moveDirection.z * deltaTime * roundBullet_moveSpeed;
	}


	// �p�^�[���Q�@���e�ɒǏ]���锒������
	for (auto& bullet : cylinderRoundBlue) {

		// ����]
		if (bullet->_id % 2 == 0)
			bullet->_angle += deltaTime;
		// �E��]
		else
			bullet->_angle -= deltaTime;

		// X
		bullet->_mesh->pos_.x = bossPosition.x + (roundBullet_radius + 150.f) * cos(bullet->_angle);
		// Y
		bullet->_mesh->pos_.y = bossPosition.y;
		// Z
		bullet->_mesh->pos_.z = bossPosition.z + (roundBullet_radius + 150.f) * sin(bullet->_angle);

		bullet->_mesh->pos_.x += roundBullet_moveDirection.x * deltaTime * roundBullet_moveSpeed;
		bullet->_mesh->pos_.z += roundBullet_moveDirection.z * deltaTime * roundBullet_moveSpeed;
	}

	// �p�^�[���R�@�S���ʒe �P��ɂ�16�� * 3�A�����͂ɔ��ˁB������S�Z�b�g
	for (auto& bullet : sphereRoundRed) {

		const float LIFETIME_LIMIT = 3.0f;
		const float offset = 0.5f;
		const float everyDir_bulletRadius = 100.0f;

		float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

		float timing = offset * (bullet->_id / 16);

		//�@������
		if (bullet->_timer == 0) {

			tnl::Vector3 moveDir, spawnPos;

			spawnPos.x = bossPosition.x + everyDir_bulletRadius * cos(angle);
			spawnPos.y = bossPosition.y;
			spawnPos.z = bossPosition.z + everyDir_bulletRadius * sin(angle);

			moveDir.x = cos(angle) * everyDir_bulletRadius;
			moveDir.z = sin(angle) * everyDir_bulletRadius;

			moveDir.normalize();

			bullet->_mesh->pos_ = spawnPos;
			bullet->_moveDirection = moveDir;
		}

		bullet->_timer += deltaTime;

		if (bullet->_isActive && bullet->_timer >= timing) {

			//�@���Z�b�g
			if (bullet->_timer > LIFETIME_LIMIT) {

				bullet->_isActive = false;
				bullet->_timer = 0;

				bullet->_mesh->pos_.x = bossPosition.x + everyDir_bulletRadius * cos(angle);
				bullet->_mesh->pos_.y = bossPosition.y;
				bullet->_mesh->pos_.z = bossPosition.z + everyDir_bulletRadius * sin(angle);
			}
		}

		//�@�X�V
		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 200;
	}
}



void BulletHell::ShotBulletHell_MetalFatigue_Patchouli(const float deltaTime) {

	for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.size(); i++) {

		auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli[i];

		//�@�p�^�[���P
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow) {

			//�@�e���Ƃ̊p�x����
			float angle_origin =
				(2.0f * tnl::PI / EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) *
				(i % EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI);

			const float radius_origin = 150.0f;

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			// ���e �W��
			if (bullet->_id < 8) {

				bullet->_timer += deltaTime;
				bullet->_isActive = true;

				if (bullet->_timer > 0.5) {

					bullet->_isActive = false;
					bullet->_mesh->pos_ = bossPosition;
					bullet->_timer = 0;
				}

				tnl::Vector3 bullet_orbit_origin;

				bullet_orbit_origin.x = bossPosition.x + cos(angle_origin) * radius_origin;
				bullet_orbit_origin.y = bossPosition.y + sin(angle_origin) * radius_origin;

				bullet_orbit_origin.normalize();

				bullet->_moveDirection = bullet_orbit_origin;

				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 300.0f;
			}

			//�@1�g 64��
			if (bullet->_id >= 8 && bullet->_id < 8 + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) {

				WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, radius_origin, 0.f);
			}

			//�@2�g 64��
			if (bullet->_id >= 8 + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI &&
				bullet->_id < 8 + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2)) {

				WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, radius_origin, 0.1f);
			}

			//�@3�g 64��
			if (bullet->_id >= 8 + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) * 2 &&
				bullet->_id < 8 + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 3)) {

				WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, radius_origin, 0.2f);
			}
		}
	}
}


void BulletHell::WaveAssist_MetalFatigue_Patchouli(
	Shared<EnemyBullet>& bullet,
	const float deltaTime,
	const float angleOrigin,
	const float radiusOrigin,
	const float startMoveTime)
{
	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

	tnl::Vector3 bullet_move_orbit_wave = bullet->_moveDirection;

	bullet->_timer += deltaTime;

	// 1�@���e�Ɠ����悤�ɔ��
	if (bullet->_timer > startMoveTime && bullet->_timer < startMoveTime + 0.5) {

		bullet->_isActive = true;

		bullet_move_orbit_wave.x = bossPosition.x + cos(angleOrigin) * radiusOrigin;
		bullet_move_orbit_wave.y = bossPosition.y + sin(angleOrigin) * radiusOrigin;

		bullet->_moveDirection = bullet_move_orbit_wave;
	}
	// 2�@��~�ʒu���炳��ɑS�W�����ɕ��􂳂��Ĕ�΂�
	if (bullet->_timer >= startMoveTime + 0.5 && bullet->_timer < startMoveTime + 1.0) {

		float angle = (2.0f * tnl::PI / 8) * (bullet->_id / 8);

		bullet_move_orbit_wave.x = cos(angle) * radiusOrigin;
		bullet_move_orbit_wave.y = sin(angle) * radiusOrigin;
		bullet_move_orbit_wave.z = sin(angle) * radiusOrigin;

		bullet->_moveDirection = bullet_move_orbit_wave;
	}
	// 3�@���˂���Q�b��ɒe�� false �ɂ��ă��Z�b�g
	if (bullet->_timer > startMoveTime + 2) {

		bullet->_isActive = false;

		bullet->_mesh->pos_ = bossPosition;
		bullet->_timer = 0;
	}

	bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime;
}



void BulletHell::ShotBulletHell_SilentSerena_Patchouli(const float deltaTime) {

	for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.size(); i++) {

		auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		//�@�p�^�[���P
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Sky) {

			float angle =
				(2.0f * tnl::PI / CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI) *
				(bullet->_id % CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI);

			float phi = (tnl::PI / 16) * (bullet->_id % 16);

			const float radius = 20.0f;
			const float LIFETIME_LIMIT = 5.0f;

			if (bullet->_timer == 0) {

				tnl::Vector3 spawnPos = bossPosition;

				/*
					 �e���~����ɓ��Ԋu�Ŕz�u
					 ���̏ꍇ�A�V���p �Ɓ@���ʊp�@�Ƃ����Q�̊p�x���K�v�ɂȂ�
					 �i �V���p�́@ sin(phi)�@�ł̂ݕ\�����@(cos �͕s�j
				*/

				bullet->_mesh->pos_.x = spawnPos.x + sin(phi) * cos(angle) * radius;
				bullet->_mesh->pos_.y = spawnPos.y + sin(phi) * sin(angle) * radius;
				bullet->_mesh->pos_.z = spawnPos.z + cos(phi) * radius;

				bullet->_moveDirection.x = sin(phi) * cos(angle);
				bullet->_moveDirection.y = sin(phi) * sin(angle);
				bullet->_moveDirection.z = cos(phi);

				bullet->_moveDirection.normalize();
			}

			bullet->_timer += deltaTime;

			//�@�X�V
			if (bullet->_timer > 0 && bullet->_timer < LIFETIME_LIMIT) {

				bullet->_isActive = true;

				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 200.f;
			}

			//�@���Z�b�g
			if (bullet->_timer >= LIFETIME_LIMIT) {

				bullet->_isActive = false;

				bullet->_timer = 0;
				bullet->_mesh->pos_ = bossPosition;
			}
		}

		//�@�p�^�[���Q
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue) {

			std::mt19937 mt(rd());

			std::uniform_real_distribution<float> dist_x(-400, 400);
			std::uniform_real_distribution<float> dist_y(-50.0, 150.0);
			std::uniform_real_distribution<float> dist_z(-250, 200);

			tnl::Vector3 moveDir = bullet->_moveDirection;

			const float LIFETIME_LIMIT = 7.0f;

			//�@������
			if (bullet->_timer == 0) {

				tnl::Vector3 spawnPos;

				spawnPos.x = bossPosition.x + dist_x(mt);
				spawnPos.y = bossPosition.y + dist_y(mt);
				spawnPos.z = bossPosition.z + dist_z(mt);

				moveDir = _player_ref->GetPos() - spawnPos;

				moveDir.normalize();

				bullet->_mesh->pos_ = spawnPos;
				bullet->_moveDirection = moveDir;

				bullet->_isActive = true;
			}

			bullet->_timer += deltaTime;

			//�@�X�V
			bullet->_mesh->pos_ += moveDir * deltaTime * 50.f;

			//�@���Z�b�g
			if (bullet->_timer > LIFETIME_LIMIT) {

				bullet->_mesh->pos_ = bossPosition;

				bullet->_timer = 0;
				bullet->_isActive = false;
			}
		}
	}
}


// �X�e�[�W2�{�X�i�`���m�j--------------------------------------------------------------------------------------------------

void BulletHell::ShotBulletHell_Normal_Cirno(const float deltaTime) {

	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

	for (int i = 0; i < EnemyBoss_Cirno::_bullet_normal_cirno.size(); i++) {

		auto bullet = EnemyBoss_Cirno::_bullet_normal_cirno[i];

		//�@�p�^�[��1
		// �V���b�g�K���i2 + N)�s (N��0�`5�܂�)�A�U��̒e���v���C���[�֔��� (81���j
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_Blue) {

			// ���ˊԊu�������邽�߂� wave���`
			int wave = bullet->_id / (STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO / 3);

			const float offset = 0.1f;
			float timing = offset * (bullet->_id / 8);

			bullet->_timer += deltaTime;

			switch (wave)
			{
			case 0:
			{
				WaveAssist_Normal_Cirno(bullet, timing, 0.0f, deltaTime);
			}
			break;
			case 1:
			{
				WaveAssist_Normal_Cirno(bullet, timing, 2.0f, deltaTime);
			}
			break;
			case 2:
			{
				WaveAssist_Normal_Cirno(bullet, timing, 4.0f, deltaTime);
			}
			break;
			}
		}

		// 	�p�^�[���Q  �S����
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {

			const float offset = 0.5f;
			float timing = offset * (bullet->_id / 16);
			float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

			const float radius = 100.0f;
			const float LIFETIME_LIMIT = 2.0f;
			const float BULLET_SPEED = 500.0f;

			if (bullet->_timer == 0) {

				tnl::Vector3 moveDir, spawnPos;

				spawnPos.x = bossPosition.x + radius * cos(angle);
				spawnPos.y = bossPosition.y;
				spawnPos.z = bossPosition.z + radius * sin(angle);

				moveDir.x = cos(angle) * radius;
				moveDir.z = sin(angle) * radius;

				moveDir.normalize();

				bullet->_mesh->pos_ = spawnPos;

				bullet->_moveDirection = moveDir;

				bullet->_isActive = true;
			}

			bullet->_timer += deltaTime;

			if (bullet->_isActive && bullet->_timer >= timing) {

				if (bullet->_timer > LIFETIME_LIMIT) {

					tnl::Vector3 moveDir;

					bullet->_isActive = false;
					bullet->_timer = 0;

					bullet->_mesh->pos_.x = bossPosition.x + radius * cos(angle);
					bullet->_mesh->pos_.y = bossPosition.y;
					bullet->_mesh->pos_.z = bossPosition.z + radius * sin(angle);

					moveDir.x = cos(angle) * radius;
					moveDir.z = sin(angle) * radius;

					moveDir.normalize();

					bullet->_moveDirection = moveDir;
				}
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}
	}
}


void BulletHell::WaveAssist_Normal_Cirno(Shared<EnemyBullet>& bullet, const float timing, const float delayOffset, const float delta_time)
{
	static const float width = 70.0f;
	static const float defaultLimit = 4.0f;

	//�@�e���ƂɎ����X�V
	if (bullet->_timer >= timing + delayOffset) {

		bullet->_isActive = true;

		tnl::Vector3 spawn_pos = _bossMesh_ref->pos_;             // �����ʒu
		tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos; // ����
		float angle = (tnl::PI / 8) * (bullet->_id % 8);          // �p�x

		moveDir.x += cos(angle) * width;

		moveDir.normalize();

		bullet->_moveDirection = moveDir;

		bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;
	}

	//�@���Z�b�g
	if (bullet->_timer > defaultLimit + delayOffset) {

		bullet->_isActive = false;
		bullet->_timer = 0;
		bullet->_mesh->pos_ = _bossMesh_ref->pos_;
	}
}


// IcicleFall----------------------------------------------------------------------------------------------
float assistTimer_icicleFall_straightBlue_cirno = 0.f;
float assistTimer_icicleFall_straightYellow_cirno = 0.f;


void BulletHell::ShotBulletHell_IcicleFall_Cirno(const float deltaTime) {

	for (int i = 0; i < EnemyBoss_Cirno::_bullet_icicleFall_cirno.size(); i++) {
		auto bullet = EnemyBoss_Cirno::_bullet_icicleFall_cirno[i];

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		float angleToPlayer =
			atan2(_player_ref->GetPos().z - bossPosition.z, _player_ref->GetPos().x - bossPosition.x);

		// 	�p�^�[��1
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

			const float LIFETIME_LIMIT = 2.0f;

			float angleOffset =      // ���ƉE��10��������
				(bullet->_id >= 10 && bullet->_id < 20) ||
				(bullet->_id >= 30 && bullet->_id < 40) ||
				(bullet->_id >= 50 && bullet->_id < 60) ||
				(bullet->_id >= 70 && bullet->_id < 89) ||
				(bullet->_id >= 100 && bullet->_id < 109) ||
				(bullet->_id >= 110 && bullet->_id < 119) ?
				tnl::ToRadian(angleToPlayer - 45) : tnl::ToRadian(angleToPlayer + 45);

			assistTimer_icicleFall_straightBlue_cirno += deltaTime;

			if (assistTimer_icicleFall_straightBlue_cirno > 0) {
				// 0 �` 19 �̒e
				if (bullet->_id < 20) {

					// �v���C���[���������E�̃I�t�Z�b�g��������
					float newAngle = angleToPlayer + angleOffset;
					float radius = (2.f * tnl::PI / 1.2f) * (bullet->_id % 10) * 20.f;

					if (bullet->_timer == 0)
					{
						InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 1.0f, deltaTime);
				}
			}

			if (assistTimer_icicleFall_straightBlue_cirno > 15) {
				// 20 �` 39 �̒e
				if (bullet->_id >= 20 && bullet->_id < 40) {

					float radius = (2.f * tnl::PI / 1.75f) * (bullet->_id % 10) * 17.f;
					float newAngle = angleToPlayer + angleOffset;

					if (bullet->_timer == 0)
					{
						InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 1.5f, deltaTime);
				}
			}

			if (assistTimer_icicleFall_straightBlue_cirno > 30) {
				// 40 �` 59 �̒e
				if (bullet->_id >= 40 && bullet->_id < 60) {

					float radius = (2.f * tnl::PI / 2.f) * (bullet->_id % 10) * 15.f;
					float newAngle = angleToPlayer + angleOffset;

					if (bullet->_timer == 0)
					{
						InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 2.0f, deltaTime);
				}
			}

			if (assistTimer_icicleFall_straightBlue_cirno > 45) {
				// 60 �` 79 �̒e
				if (bullet->_id >= 60 && bullet->_id < 80) {

					float radius = (2.f * tnl::PI / 2.f) * (bullet->_id % 10) * 12.5f;
					float newAngle = angleToPlayer + angleOffset;

					if (bullet->_timer == 0)
					{
						InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 2.5f, deltaTime);
				}
			}

			if (assistTimer_icicleFall_straightBlue_cirno > 60) {
				// 80 �` 99 �̒e
				if (bullet->_id >= 90 && bullet->_id < 109) {

					float radius = (2.f * tnl::PI / 2.f) * (bullet->_id % 10) * 10.f;
					float newAngle = angleToPlayer + angleOffset;

					if (bullet->_timer == 0)
					{
						InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 3.0f, deltaTime);
				}
			}

			if (assistTimer_icicleFall_straightBlue_cirno > 75) {
				// 100 �` 119 �̒e
				if (bullet->_id >= 100 && bullet->_id < 119) {

					float radius = (2.f * tnl::PI / 2.f) * (bullet->_id % 10) * 8.5f;
					float newAngle = angleToPlayer + angleOffset;

					if (bullet->_timer == 0)
					{
						InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 3.5f, deltaTime);
				}
			}
		}

		// 	�p�^�[��2
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow) {

			const float BULLET_SPACING = 50.0f;
			const float BULLET_SPEED = 400.0f;
			const float FIRE_INTERVAL = 5.0f;
			const int BULLETS_PER_ROW = 5;   // 1�s������̒e��

			assistTimer_icicleFall_straightYellow_cirno += deltaTime;

			if (assistTimer_icicleFall_straightYellow_cirno > 0) {

				if (bullet->_id < 5) {

					if (bullet->_timer == 0) {

						InitAssist_IcicleFall_StraightYellow_Cirno(bullet, BULLETS_PER_ROW, BULLET_SPACING);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
				}
			}

			if (assistTimer_icicleFall_straightYellow_cirno > 10) {

				if (bullet->_id >= 5 && bullet->_id < 10) {

					float fireTime = (bullet->_id / BULLETS_PER_ROW) * FIRE_INTERVAL;

					if (bullet->_timer == 0) {

						InitAssist_IcicleFall_StraightYellow_Cirno(bullet, BULLETS_PER_ROW, BULLET_SPACING);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
				}
			}

			if (assistTimer_icicleFall_straightYellow_cirno > 20) {

				if (bullet->_id >= 10 && bullet->_id < 15) {

					float fireTime = (bullet->_id / BULLETS_PER_ROW) * FIRE_INTERVAL;

					if (bullet->_timer == 0) {

						InitAssist_IcicleFall_StraightYellow_Cirno(bullet, BULLETS_PER_ROW, BULLET_SPACING);
					}

					bullet->_timer += deltaTime;

					UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
				}
			}
		}
	}
}


void BulletHell::InitAssist_IcicleFall_StraightBlue_Cirno(
	Shared<EnemyBullet>& bullet,
	const float radius,
	const float newAngle)
{
	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
	tnl::Vector3 bossForward = { cos(_bossMesh_ref->rot_.y), 0, sin(_bossMesh_ref->rot_.y) };

	tnl::Vector3 moveDir, spawnPos;

	spawnPos = bossPosition + bossForward * radius * cos(newAngle);
	spawnPos.x += radius * cos(newAngle);
	spawnPos.z += radius * sin(newAngle);

	moveDir += tnl::Vector3{ cos(newAngle), 0 , sin(newAngle) };
	moveDir.normalize();

	bullet->_mesh->pos_ = spawnPos;

	bullet->_moveDirection = moveDir * 1.f;

	bullet->_isActive = true;
}


void BulletHell::UpdateAssist_IcicleFall_StraightBlue_Cirno(
	Shared<EnemyBullet>& bullet,
	const float radius,
	const float newAngle,
	const float startChangeDirTime,
	const float deltaTime)
{
	const float BULLET_SPEED = 200.0f;

	if (bullet->_timer > startChangeDirTime && bullet->_timer < startChangeDirTime + 0.1f)
	{
		tnl::Vector3 changeDir = _player_ref->GetPos() - bullet->_mesh->pos_;
		changeDir += tnl::Vector3{ radius * cos(newAngle), 0 , radius * sin(newAngle) };

		changeDir.normalize();

		bullet->_moveDirection = changeDir;
		bullet->_moveDirection.normalize();
	}

	if (bullet->_timer > startChangeDirTime + 3.f)
	{
		bullet->_isActive = false;
		bullet->_timer = 0;
		bullet->_mesh->pos_ = _bossMesh_ref->pos_;
	}

	bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
}


void BulletHell::UpdateAssist_IcicleFall_StraightYellow_Cirno(
	Shared<EnemyBullet>& bullet,
	const float deltaTime,
	const float bulletSpeed)
{
	if (bullet->_isActive) {

		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
	}

	if (bullet->_timer > 4.f) {

		bullet->_isActive = false;
		bullet->_timer = 0;
		bullet->_mesh->pos_ = _bossMesh_ref->pos_;
	}
}

void BulletHell::InitAssist_IcicleFall_StraightYellow_Cirno(
	Shared<EnemyBullet>& bullet,
	const int bullets_perRow,
	const float bulletSpace)
{
	tnl::Vector3 direction = _player_ref->GetPos() - _bossMesh_ref->pos_;
	direction.normalize();
	bullet->_moveDirection = direction;

	// �����ȊO�̒e�����E�ɂ����悤�ɒ���
	int column = bullet->_id % bullets_perRow;
	float offset = (column - bullets_perRow / 2) * bulletSpace;
	tnl::Vector3 sideOffset = tnl::Vector3::TransformCoord({ offset,0,0 }, _bossMesh_ref->rot_);

	bullet->_mesh->pos_ = _bossMesh_ref->pos_ + sideOffset;
	bullet->_isActive = true;
}


// ----------------------------------------------------------------------------------------------
void BulletHell::ShotBulletHell_PerfectFreeze_Cirno(const float deltaTime) {

	for (int i = 0; i < EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.size(); i++) {

		auto bullet = EnemyBoss_Cirno::_bullet_perfectFreeze_cirno[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_White) {

			std::mt19937 mt(rd());

			int shotNum = 0;
			const float radius = 150.0f;
			tnl::Vector3 everyDirBullet_moveDirection;
			float phi = (tnl::PI / 16) * (bullet->_id % 16);

			if (bullet->_timer == 0) {

				tnl::Vector3 spawn_pos = bossPosition;

				// �e���~����ɓ��Ԋu�Ŕz�u����
				float angle = (2.0f * tnl::PI / 80) * (shotNum % 80);

				spawn_pos.x += sin(phi) * cos(angle);
				spawn_pos.y += sin(phi) * sin(angle);
				spawn_pos.z += sin(angle);
				spawn_pos += GenerateRandomVector(mt, -250, 50, -250, 50, -150, 150);

				everyDirBullet_moveDirection.x = cos(angle) * radius;
				everyDirBullet_moveDirection.y = sin(angle) * radius;
				everyDirBullet_moveDirection.z = sin(angle) * radius;
				everyDirBullet_moveDirection += GenerateRandomVector(mt, -250, 50, -150, 150, -250, 50);
				everyDirBullet_moveDirection.normalize();

				bullet->_moveDirection = everyDirBullet_moveDirection;
				bullet->_mesh->pos_ = spawn_pos;
				bullet->_isActive = true;
			}

			bullet->_timer += deltaTime;

			if (bullet->_timer < 2) {

				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 0.1f;
			}
			else if (bullet->_timer > 2 && bullet->_timer < 5) {

				bullet->_moveDirection = tnl::Vector3(0, 0, 0); // �e�̑��x��0�ɂ���
			}
			else if ((int)bullet->_timer == 5) {

				float angle = (2.0f * tnl::PI / 80) * (shotNum % 80);
				everyDirBullet_moveDirection.x = sin(phi) * cos(angle) * radius;
				everyDirBullet_moveDirection.y = sin(phi) * sin(angle) * radius;
				everyDirBullet_moveDirection.z = sin(angle) * radius;
				everyDirBullet_moveDirection += GenerateRandomVector(mt, -250, 50, -150, 150, -250, 50);
				everyDirBullet_moveDirection.normalize();

				bullet->_moveDirection = everyDirBullet_moveDirection;

				shotNum = (shotNum + 1) % 80;
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 200;

			if (bullet->_timer > 9) {
				bullet->_isActive = false;

				bullet->_timer = 0;
			}
		}

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

			tnl::Vector3 spawn_pos = bossPosition;

			if (bullet->_timer == 0) {

				float angle = (tnl::PI / 8) * (bullet->_id % 8);

				tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos;

				moveDir.x += cos(angle) * 70;

				moveDir.normalize();

				bullet->_moveDirection = moveDir;

				bullet->_mesh->pos_ = spawn_pos;
			}

			bullet->_timer += deltaTime;

			const float offset = 0.1f;
			float timing = offset * (bullet->_id / 8);

			if (bullet->_timer >= timing) {

				bullet->_isActive = true;

				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 500;
			}
			else {

				bullet->_mesh->pos_ = spawn_pos;
			}

			if (bullet->_timer > 4) {
				bullet->_isActive = false;
				bullet->_timer = 0;
				bullet->_mesh->pos_ = spawn_pos;
			}
		}
	}
}


// �X�e�[�W3�{�X�i�z�K�q�j--------------------------------------------------------------------------------------------------

void BulletHell::ShotBulletHell_Normal_Suwako(const float deltaTime) {
	// �S���� �P��ɂ��U�S�������A�W�����֖����ɔ�΂�������

	for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.size(); i++) {

		auto bullet = EnemyBoss_MoriyaSuwako::_bullet_normal_suwako[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// �E���
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {

			if (bullet->_timer == 0) {

				const float every_direction_bullet_radius = 100.0f;
				float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

				tnl::Vector3 spawn_pos = bossPosition;
				tnl::Vector3 moveDirection;


				spawn_pos.x += cos(angle) * bullet->_id;
				spawn_pos.z += sin(angle) * bullet->_id;

				moveDirection.x = -sin(angle) * every_direction_bullet_radius;
				moveDirection.z = cos(angle) * every_direction_bullet_radius;

				moveDirection.normalize();

				bullet->_mesh->pos_ = spawn_pos;
				bullet->_moveDirection = moveDirection;
			}

			float timeLimit = 0;
			bullet->_timer += deltaTime;

			int row = bullet->_id / 56;

			if (row == 0)
			{
				if (bullet->_timer > 0) {

					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 1)
			{
				if (bullet->_timer > 1) {
					timeLimit = 1;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 2)
			{
				if (bullet->_timer > 2) {

					timeLimit = 2;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 3)
			{
				if (bullet->_timer > 3) {

					timeLimit = 3;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 4)
			{
				if (bullet->_timer > 4) {

					timeLimit = 4;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 500;
		}

		// ��
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {

			if (bullet->_timer == 0) {

				const float every_direction_bullet_radius = 100.0f;
				float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

				tnl::Vector3 spawn_pos = bossPosition;
				tnl::Vector3 moveDirection;

				spawn_pos.x -= cos(angle) * bullet->_id;
				spawn_pos.z -= sin(angle) * bullet->_id;

				moveDirection.x = sin(angle) * every_direction_bullet_radius;
				moveDirection.z = -cos(angle) * every_direction_bullet_radius;

				moveDirection.normalize();

				bullet->_mesh->pos_ = spawn_pos;

				bullet->_moveDirection = moveDirection;
			}

			bullet->_timer += deltaTime;

			int row = bullet->_id / 56;
			float timeLimit = 0;

			if (row == 0)
			{
				if (bullet->_timer > 0) {

					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 1)
			{
				if (bullet->_timer > 1) {

					timeLimit = 1;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 2)
			{
				if (bullet->_timer > 2) {

					timeLimit = 2;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 3)
			{
				if (bullet->_timer > 3) {

					timeLimit = 3;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}
			if (row == 4)
			{
				if (bullet->_timer > 4) {

					timeLimit = 4;
					UpdateAssist_Normal_Suwako(bullet, timeLimit);
				}
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 500;
		}
	}
}


void BulletHell::UpdateAssist_Normal_Suwako(Shared<EnemyBullet>& bullet, const float timeLimit)
{
	bullet->_isActive = true;

	if (bullet->_timer > timeLimit + 1.f) {

		bullet->_isActive = false;
		bullet->_timer = 0;
		bullet->_mesh->pos_ = _bossMesh_ref->pos_;
	}
}


void BulletHell::ShotBulletHell_IronRingOfMoriya_Suwako(const float deltaTime) {

	for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.size(); i++) {

		auto bullet = EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako[i];

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
		static const float bulletSpeed = 150.f;

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {

			const float circle1Radius = 80.0f;
			const float circle2Radius = 100.0f;

			float angle = (2.0f * tnl::PI / 45) * (bullet->_id % 45);

			int row = bullet->_id / 90;

			if (row == 0)
			{
				if (bullet->_id < 45) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 45 && bullet->_id < 90) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}
			}
			if (row == 1)
			{
				if (bullet->_id >= 90 && bullet->_id > 135) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 135 && bullet->_id < 180) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 2)
			{
				if (bullet->_id >= 180 && bullet->_id > 225) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 225 && bullet->_id < 270) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}
			}
			if (row == 3)
			{
				if (bullet->_id >= 270 && bullet->_id > 315) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 315 && bullet->_id < 360) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 4)
			{
				if (bullet->_id >= 360 && bullet->_id > 405) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 405 && bullet->_id < 450) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 5)
			{
				if (bullet->_id >= 450 && bullet->_id > 495) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 495 && bullet->_id < 540) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 6)
			{
				if (bullet->_id >= 540 && bullet->_id > 585) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 585 && bullet->_id < 630) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 7)
			{
				if (bullet->_id >= 630 && bullet->_id > 675) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 675 && bullet->_id < 720) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 8)
			{
				if (bullet->_id >= 720 && bullet->_id > 765) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 765 && bullet->_id < 810) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

			}
			if (row == 9)
			{
				if (bullet->_id >= 810 && bullet->_id > 855) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}

				if (bullet->_id >= 855 && bullet->_id < 900) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2Radius,
						angle,
						deltaTime,
						bulletSpeed,
						row);
				}
			}
		}
	}
}


void BulletHell::WaveAssist_IronRingOfMoriya_Suwako(
	Shared<EnemyBullet>& bullet,
	const float circleRadius,
	const float angle,
	const float deltaTime,
	const float bulletSpeed,
	const int startMoveTime)
{
	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
	tnl::Vector3 spawn_pos = bossPosition;
	tnl::Vector3 targetDir;

	if (bullet->_timer == 0) {

		spawn_pos.x += circleRadius * cos(angle);
		spawn_pos.z += circleRadius * sin(angle);
		bullet->_mesh->pos_ = spawn_pos;
	}

	targetDir = _player_ref->GetPos() - spawn_pos;
	targetDir.normalize();

	bullet->_timer += deltaTime;

	if (bullet->_timer > startMoveTime) {

		bullet->_isActive = true;

		bullet->_moveDirection = tnl::Vector3::UniformLerp(
			bullet->_moveDirection,
			targetDir * 0.5f,
			1.5f,
			bullet->_timer
		);

		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
	}
	else {
		bullet->_isActive = false;
	}

	if (bullet->_timer > startMoveTime + 10) {

		bullet->_isActive = false;
		bullet->_timer = 0;
	}
}


void BulletHell::ShotBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako(const float deltaTime) {

	for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size(); i++) {

		auto bullet = EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako[i];

		// ��֕����Ă��牺�֍~�낷
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

			tnl::Vector3 upward_velocity = { 0,50,0 };

			if (bullet->_timer == 0) {

				InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(upward_velocity, bullet);
			}

			bullet->_timer += deltaTime;

			UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(bullet, deltaTime);
		}

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_EmeraldGreen) {

			tnl::Vector3 upward_velocity = { 0,30,0 };

			if (bullet->_timer == 0) {

				InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(upward_velocity, bullet);
			}

			bullet->_timer += deltaTime;

			UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(bullet, deltaTime);
		}
	}
}


void BulletHell::InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(tnl::Vector3& upwardVelocity, Shared<EnemyBullet>& bullet)
{

	tnl::Vector3 spawn_pos = _bossMesh_ref->pos_;
	std::mt19937 mt(rd());

	std::uniform_real_distribution<float> diff_x(-500.f, 500.f);
	std::uniform_real_distribution<float> diff_y(50, 150.f);
	std::uniform_real_distribution<float> diff_z(-200.f, 0.f);

	tnl::Vector3 offset = { diff_x(mt) ,diff_y(mt),diff_z(mt) };

	tnl::Vector3 moveDir;

	moveDir += spawn_pos + offset;
	moveDir.y += upwardVelocity.y; // �O�����֗͂�������

	moveDir.normalize();

	bullet->_moveDirection = moveDir;
	bullet->_mesh->pos_ = spawn_pos + tnl::Vector3(0, 50, 0);
}


void BulletHell::UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(Shared<EnemyBullet>& bullet, const float deltaTime)
{
	const float bulletSpeed = 200.0f;
	const float gravityAcceleration = -9.8f;
	const float offset = 0.1f;
	const float lifeTime = 6.0f;

	float timing = offset * (bullet->_id / 10);

	if (bullet->_timer >= timing) {

		bullet->_isActive = true;

		tnl::Vector3 gravity = { 0, gravityAcceleration * deltaTime, 0 };

		bullet->_moveDirection += gravity * deltaTime;
		bullet->_moveDirection.normalize();

		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
	}

	if (bullet->_timer > timing + lifeTime) {

		bullet->_isActive = false;

		bullet->_timer = 0;

		bullet->_mesh->pos_ = _bossMesh_ref->pos_;
	}
}