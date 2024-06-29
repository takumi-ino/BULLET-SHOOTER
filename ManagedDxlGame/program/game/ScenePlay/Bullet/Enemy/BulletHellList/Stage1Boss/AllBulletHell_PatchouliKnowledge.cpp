#include "../../BulletHell.h"
#include "../../BulletHellsBulletNumInfo.h"
#include "../../EnemyBullet.h"
#include "../../../../Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../../../../RandomValue/RandomValueGenerator.h"


/*
�@�@�@�@�E�֐��̐錾�́@BulletHell.h�@�ɂ���܂��B
  �@�@  �E�����ł̓X�e�[�W1�{�X�A�u�p�`�����[�E�m�[���b�W�v�̒e���֐���`�����Ă��܂��B
*/


namespace inl {

	//�@�ʏ�U��-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_Normal_Patchouli(const float deltaTime) {

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// ���ꂼ��̃^�C�v�̒e����ʁX�ɏ������邽�߁A�����x�N�^�[����v�f�̃O���[�v����������
		static std::vector<Shared<EnemyBullet>> sphereRoundBlue, cylinderRoundBlue, sphereRoundRed;

		if (sphereRoundBlue.empty() && cylinderRoundBlue.empty() && sphereRoundRed.empty()) {

			for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.size(); i++) {
				auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli[i];

				// �X�t�B�A�A��]�A��
				if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {
					sphereRoundBlue.push_back(bullet);
				}
				// �V�����_�[�A��]�A��
				else if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {
					cylinderRoundBlue.push_back(bullet);
				}
				// �X�t�B�A�A��]�A��
				else {
					sphereRoundRed.push_back(bullet);
				}
			}
		}

		const float ROUNDBULLET_RADIUS = 90.0f;
		const float ROUNDBULLET_MOVESPEED = 100.0f;
		tnl::Vector3 roundBullet_moveDirection;

		// �p�^�[���P�@�{�X�̎��͂��ʒu��]������e
		for (auto& bullet : sphereRoundBlue) {

			// ����]
			if (bullet->_id % 2 == 0) {
				bullet->_angle += deltaTime;
			}

			// �E��]
			if (bullet->_id % 2 != 0) {
				bullet->_angle -= deltaTime;
			}

			// X
			bullet->_mesh->pos_.x = bossPosition.x + ROUNDBULLET_RADIUS * cos(bullet->_angle);
			// Y
			bullet->_mesh->pos_.y = bossPosition.y;
			// Z
			bullet->_mesh->pos_.z = bossPosition.z + ROUNDBULLET_RADIUS * sin(bullet->_angle);

			// ��������
			roundBullet_moveDirection.x =
				bossPosition.x + cos(bullet->_angle) * ROUNDBULLET_RADIUS;

			roundBullet_moveDirection.z =
				bossPosition.z + sin(bullet->_angle) * ROUNDBULLET_RADIUS;

			//�@���K��
			roundBullet_moveDirection.normalize();

			//�@�X�V
			bullet->_mesh->pos_.x += roundBullet_moveDirection.x * deltaTime * ROUNDBULLET_MOVESPEED;
			bullet->_mesh->pos_.z += roundBullet_moveDirection.z * deltaTime * ROUNDBULLET_MOVESPEED;
		}


		// �p�^�[���Q�@���e�ɒǏ]���锒������
		for (auto& bullet : cylinderRoundBlue) {

			// ����]
			if (bullet->_id % 2 == 0) {
				bullet->_angle += deltaTime;
			}
			// �E��]
			else {
				bullet->_angle -= deltaTime;
			}

			//�@���e�Ƃ̊Ԋu
			const float SPACE_OFFSET = 150.f;

			// X
			bullet->_mesh->pos_.x = bossPosition.x + (ROUNDBULLET_RADIUS + SPACE_OFFSET) * cos(bullet->_angle);
			// Y
			bullet->_mesh->pos_.y = bossPosition.y;
			// Z
			bullet->_mesh->pos_.z = bossPosition.z + (ROUNDBULLET_RADIUS + SPACE_OFFSET) * sin(bullet->_angle);

			//�@�X�V
			bullet->_mesh->pos_.x += roundBullet_moveDirection.x * deltaTime * ROUNDBULLET_MOVESPEED;
			bullet->_mesh->pos_.z += roundBullet_moveDirection.z * deltaTime * ROUNDBULLET_MOVESPEED;
		}

		// �p�^�[���R�@�S���ʒe �P��ɂ�16�� * 3�A�����͂ɔ��ˁB������S�Z�b�g
		for (auto& bullet : sphereRoundRed) {

			const float EVERYDIRECTION_BULLETRADIUS = 100.0f;
			const float BULLET_SPEED = 200.0f;
			const float LIFETIME_LIMIT = 3.0f;
			const float STARTTIME_OFFSET = 0.5f;
			const int BULLET_NUM_PER_GROUP = 16;

			// �p�x
			float angle = (2.0f * tnl::PI / BULLET_NUM_PER_GROUP) * (bullet->_id % BULLET_NUM_PER_GROUP);

			//�@������
			if (bullet->_timer == 0.f) {

				tnl::Vector3 moveDir, spawnPos;

				spawnPos.x = bossPosition.x + EVERYDIRECTION_BULLETRADIUS * cos(angle);
				spawnPos.y = bossPosition.y;
				spawnPos.z = bossPosition.z + EVERYDIRECTION_BULLETRADIUS * sin(angle);

				moveDir.x = cos(angle) * EVERYDIRECTION_BULLETRADIUS;
				moveDir.z = sin(angle) * EVERYDIRECTION_BULLETRADIUS;

				moveDir.normalize();

				bullet->_mesh->pos_ = spawnPos;
				bullet->_moveDirection = moveDir;
			}

			//�@�^�C�}�[�N��
			bullet->_timer += deltaTime;

			//�@�O���[�v���Ƃɕ������e�̊e�X�V�J�n����
			float startTiming = STARTTIME_OFFSET * (bullet->_id / BULLET_NUM_PER_GROUP);

			if (bullet->_isActive && bullet->_timer >= startTiming) {

				//�@���Z�b�g
				if (bullet->_timer > LIFETIME_LIMIT) {

					bullet->_isActive = false;
					bullet->_timer = 0;

					bullet->_mesh->pos_.x = bossPosition.x + EVERYDIRECTION_BULLETRADIUS * cos(angle);
					bullet->_mesh->pos_.y = bossPosition.y;
					bullet->_mesh->pos_.z = bossPosition.z + EVERYDIRECTION_BULLETRADIUS * sin(angle);
				}
			}

			//�@�X�V
			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}
	}


	//�@����U���P-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_MetalFatigue_Patchouli(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.size(); i++) {

			auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli[i];

			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow) {

				//�@�e���Ƃ̊p�x����
				float angle_origin =
					(2.0f * tnl::PI / EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) *
					(i % EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI);

				const float RADIUS_ORIGIN = 150.0f;
				const float BULLET_MOVESPEED = 300.0f;
				const float BULLET_LIFETIME = 0.5f;
				const int BULLET_OFFSET = 8;

				tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

				// ���e �W��
				if (bullet->_id < BULLET_OFFSET) {

					// �^�C�}�[�N��
					bullet->_timer += deltaTime;
					bullet->_isActive = true;

					//�@�e���Z�b�g
					if (bullet->_timer > BULLET_LIFETIME) {

						bullet->_isActive = false;
						bullet->_mesh->pos_ = bossPosition;
						bullet->_timer = 0.f;
					}

					bullet->_moveDirection.x = bossPosition.x + cos(angle_origin) * RADIUS_ORIGIN;
					bullet->_moveDirection.y = bossPosition.y + sin(angle_origin) * RADIUS_ORIGIN;

					bullet->_moveDirection.normalize();

					//�@�X�V
					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_MOVESPEED;
				}

				//�@1�g 64��
				if (bullet->_id >= BULLET_OFFSET &&
					bullet->_id < BULLET_OFFSET + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) {

					WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, RADIUS_ORIGIN, 0.f);
				}

				//�@2�g 64��
				if (bullet->_id >= BULLET_OFFSET + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI &&
					bullet->_id < BULLET_OFFSET + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2)) {

					WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, RADIUS_ORIGIN, 0.1f);
				}

				//�@3�g 64��
				if (bullet->_id >= BULLET_OFFSET + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) * 2 &&
					bullet->_id < BULLET_OFFSET + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 3)) {

					WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, RADIUS_ORIGIN, 0.2f);
				}
			}
		}
	}

	//�@����U���P �����A�V�X�g�֐�------------------------------------------------------
	void BulletHell::WaveAssist_MetalFatigue_Patchouli(
		Shared<EnemyBullet>& bullet,
		const float deltaTime,
		const float angleOrigin,
		const float radiusOrigin,
		const float startMoveTime)
	{
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
		tnl::Vector3 bulletMoveOrbit = bullet->_moveDirection;

		const float OFFSET = 0.5f;

		// �^�C�}�[�N��
		bullet->_timer += deltaTime;

		// 1�@���e�Ɠ����悤�ɔ�ԁ@���C�t�^�C��0.5�b
		if (bullet->_timer > startMoveTime && bullet->_timer < startMoveTime + OFFSET) {

			bullet->_isActive = true;

			bulletMoveOrbit.x = bossPosition.x + cos(angleOrigin) * radiusOrigin;
			bulletMoveOrbit.y = bossPosition.y + sin(angleOrigin) * radiusOrigin;

			bullet->_moveDirection = bulletMoveOrbit;
		}
		// 2�@��~�ʒu���炳��ɑS�W�����ɕ��􂳂��Ĕ�΂��@���C�t�^�C�� 1.0�b
		if (bullet->_timer > startMoveTime + OFFSET && bullet->_timer < startMoveTime + (OFFSET * 2.f)) {

			float angle = (2.0f * tnl::PI / 8) * (bullet->_id / 8);

			bulletMoveOrbit.x = cos(angle) * radiusOrigin;
			bulletMoveOrbit.y = sin(angle) * radiusOrigin;
			bulletMoveOrbit.z = sin(angle) * radiusOrigin;

			bullet->_moveDirection = bulletMoveOrbit;
		}
		// 3�@���˂���Q�b��ɒe�����Z�b�g
		if (bullet->_timer > startMoveTime + (OFFSET * 4.f)) {

			bullet->_isActive = false;

			bullet->_mesh->pos_ = bossPosition;
			bullet->_timer = 0.f;
		}

		//�@�X�V
		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime;
	}


	//�@����U���Q ----------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_SilentSerena_Patchouli(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.size(); i++) {

			auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli[i];
			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			//�@�p�^�[���P�@�~������ˁB���ʍ��W��p���Ă���������֌���
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Sky) {

				const float RADIUS = 20.0f;
				const float LIFETIME_LIMIT = 5.0f;
				const float BULLET_MOVESPEED = 200.f;
				const int SHOT_DIRECTION_COUNT = 16;

				// ���ʊp
				float theta =
					(2.0f * tnl::PI / CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI) *
					(bullet->_id % CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI);

				// �V���p
				float phi = (tnl::PI / SHOT_DIRECTION_COUNT) * (bullet->_id % SHOT_DIRECTION_COUNT);

				// ������
				if (bullet->_timer == 0.f) {

					/*
					�@ ���@Direct X�̍��W�n�́@y���㉺�Az�����s���w���i�@���̃v���W�F�N�g���Y������@�j
					   �@�@�t���[�����[�N��Q�[���G���W���Ȃǂɂ���Ắ@z���㉺�Ay�����s��\���ꍇ������̂Œ���


						   3�����ɍ��W�@�i ���a�A�V�[�^�i�p�x�j�A�p�C�i�p�x�j�j

						�@ �e���~����ɓ��Ԋu�Ŕz�u
						�@ ���̏ꍇ�A�p�C�i�V���p�j �Ɓ@�V�[�^�i�Ίp�܂��͕��ʊp�Ƃ����j�̂Q�̊p�x���g�p
						�@�i �V���p�́@ sin(phi)�@�ł̂ݕ\�����@(cos �͕s�j
						�@
						�@ �V���p       == �k����Ƃ����_�܂ł̊p�x
						�@ ���ʊp(�Ίp) == ���_����_�܂ł̊p�x
						�@
						�@ ���܂��@���@�p == ����ʂ��琂�����܂ł̊p�x(�ʏ�A������)
					*/

					tnl::Vector3 spawnPos = bossPosition;

					bullet->_mesh->pos_.x = spawnPos.x + sin(phi) * cos(theta) * RADIUS;
					bullet->_mesh->pos_.y = spawnPos.y + sin(phi) * sin(theta) * RADIUS;
					bullet->_mesh->pos_.z = spawnPos.z + cos(phi) * RADIUS;

					bullet->_moveDirection.x = sin(phi) * cos(theta);
					bullet->_moveDirection.y = sin(phi) * sin(theta);
					bullet->_moveDirection.z = cos(phi);

					bullet->_moveDirection.normalize();
				}

				//�@�^�C�}�[�N��
				bullet->_timer += deltaTime;

				//�@�X�V
				if (bullet->_timer > 0.f && bullet->_timer < LIFETIME_LIMIT) {

					bullet->_isActive = true;

					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_MOVESPEED;
				}

				//�@���Z�b�g
				if (bullet->_timer > LIFETIME_LIMIT) {

					bullet->_isActive = false;

					bullet->_timer = 0.f;
					bullet->_mesh->pos_ = bossPosition;
				}
			}

			//�@�p�^�[���Q�@�v���C���[�ւ������߂Â��e
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue) {

				tnl::Vector3 moveDir = bullet->_moveDirection;

				const float LIFETIME_LIMIT = 7.0f;
				const float BULLET_MOVESPEED = 50.f;

				//�@������
				if (bullet->_timer == 0.f) {

					// �����_���x�N�^�[
					tnl::Vector3 randomVector = RandomValueGenerator::Vector(-400, 400, -50, 150, -250, 200);
					tnl::Vector3 spawnPos;

					spawnPos.x = bossPosition.x + randomVector.x;
					spawnPos.y = bossPosition.y + randomVector.y;
					spawnPos.z = bossPosition.z + randomVector.z;

					//�@�v���C���[�����擾
					moveDir = _player_ref->GetPos() - spawnPos;

					moveDir.normalize();

					bullet->_mesh->pos_ = spawnPos;
					bullet->_moveDirection = moveDir;

					bullet->_isActive = true;
				}

				//�@�^�C�}�[�N��
				bullet->_timer += deltaTime;

				//�@�X�V
				bullet->_mesh->pos_ += moveDir * deltaTime * BULLET_MOVESPEED;

				//�@���Z�b�g
				if (bullet->_timer > LIFETIME_LIMIT) {

					bullet->_mesh->pos_ = bossPosition;

					bullet->_timer = 0.f;
					bullet->_isActive = false;
				}
			}
		}
	}
}