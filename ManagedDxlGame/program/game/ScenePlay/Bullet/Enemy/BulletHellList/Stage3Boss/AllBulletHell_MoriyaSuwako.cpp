#include "../../BulletHell.h"
#include "../../BulletHellsBulletNumInfo.h"
#include "../../EnemyBullet.h"
#include "../../../../Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"
#include "../../../../RandomValue/RandomValueGenerator.h"


/*
�@�@�@�@�E�֐��̐錾�́@BulletHell.h�@�ɂ���܂��B
  �@�@  �E�����ł̓X�e�[�W�R�{�X�A�u�k��z�K�q�v�̒e���֐���`�����Ă��܂��B
*/


namespace inl {

	// �ʏ�U��-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_Normal_Suwako(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.size(); i++) {

			auto bullet = EnemyBoss_MoriyaSuwako::_bullet_normal_suwako[i];
			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			const float RADIUS = 100.0f;         // ���a
			const float BULLET_SPEED = 500.0f;   // ����
			const int BULLETNUM_EACH_WAVE = 16;  // 1��̔��˂Ŏg�p����e�̐�


			// �ԁB �S���� �P��ɂ��U�S�������A�W�����֖����ɔ�΂�������-------------------------------
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {

				//�@������
				if (bullet->_timer == 0.f) {

					float angle = (2.0f * tnl::PI / BULLETNUM_EACH_WAVE) * (bullet->_id);

					//�@�����ʒu
					tnl::Vector3 spawnPos = bossPosition;
					spawnPos.x += cos(angle) * bullet->_id;
					spawnPos.z += sin(angle) * bullet->_id;

					//�@�ړ�����
					tnl::Vector3 moveDirection;
					moveDirection.x += -sin(angle) * RADIUS;
					moveDirection.z += cos(angle) * RADIUS;

					moveDirection.normalize();

					bullet->_mesh->pos_ = spawnPos;
					bullet->_moveDirection = moveDirection;
				}

				//�@����
				float lifeTime = 0.f;

				//�@�^�C�}�[�N��
				bullet->_timer += deltaTime;

				// �e��ID���ƂɃO���[�v�������s���A�O���[�v���ƂɍX�V����
				int wave = bullet->_id / 56;

				//�@��P�g---------------------------------------
				if (wave == 0)
				{
					if (bullet->_timer > 0.f)
						ResetAssist_Normal_Suwako(bullet, lifeTime);
				}
				//�@��Q�g---------------------------------------
				if (wave == 1)
				{
					if (bullet->_timer > 1.f) {
						lifeTime = 1.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��R�g---------------------------------------
				if (wave == 2)
				{
					if (bullet->_timer > 2.f) {

						lifeTime = 2.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��S�g---------------------------------------
				if (wave == 3)
				{
					if (bullet->_timer > 3.f) {

						lifeTime = 3.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��T�g---------------------------------------
				if (wave == 4)
				{
					if (bullet->_timer > 4.f) {

						lifeTime = 4.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}

				// �X�V
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
			}

			// �B �S���� �P��ɂ��U�S�������A�W�����֖����ɔ�΂�������--------------------------------
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {

				// ������
				if (bullet->_timer == 0.f) {

					float angle = (2.0f * tnl::PI / BULLETNUM_EACH_WAVE) * (bullet->_id);

					//�@�����ʒu
					tnl::Vector3 spawnPos = bossPosition;
					spawnPos.x -= cos(angle) * bullet->_id;
					spawnPos.z -= sin(angle) * bullet->_id;

					//�@�ړ�����
					tnl::Vector3 moveDirection;
					moveDirection.x -= -sin(angle) * RADIUS;
					moveDirection.z -= cos(angle) * RADIUS;

					moveDirection.normalize();

					bullet->_mesh->pos_ = spawnPos;
					bullet->_moveDirection = moveDirection;
				}

				// �^�C�}�[�N��
				bullet->_timer += deltaTime;

				// �e�̎���
				float lifeTime = 0.f;

				// �e��ID���ƂɃO���[�v�������s���A�O���[�v���ƂɍX�V����
				int wave = bullet->_id / 56;

				//�@��P�g---------------------------------------
				if (wave == 0)
				{
					if (bullet->_timer > 0.f) {

						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��Q�g---------------------------------------
				if (wave == 1)
				{
					if (bullet->_timer > 1.f) {

						lifeTime = 1.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��R�g---------------------------------------
				if (wave == 2)
				{
					if (bullet->_timer > 2.f) {

						lifeTime = 2.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��S�g---------------------------------------
				if (wave == 3)
				{
					if (bullet->_timer > 3.f) {

						lifeTime = 3.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//�@��T�g---------------------------------------
				if (wave == 4)
				{
					if (bullet->_timer > 4.f) {

						lifeTime = 4.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}

				//�@�X�V
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
			}
		}
	}


	//�@�ʏ�U���p�^�[���i���Z�b�g�����j------------------------------------------------------------
	void BulletHell::ResetAssist_Normal_Suwako(Shared<EnemyBullet>& bullet, const float timeLimit)
	{
		bullet->_isActive = true;

		if (bullet->_timer > timeLimit + 1.f) {

			bullet->_isActive = false;
			bullet->_timer = 0;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}


	// ����U���P--------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_IronRingOfMoriya_Suwako(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.size(); i++) {

			auto bullet = EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako[i];

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			//�@�~��̔z�u��ۂ����܂܃v���C���[��ǂ�������e
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {

				//�@�e�͓����ɂS�T���A�O���ɂS�T���A�v�X�O����1�̉�Ƃ���

				const int BULLETCOUNT_EACHCIRCLE = 45;  // 1�Ǝ�45��
				const float CIRCLE1_RADIUS = 70.0f;     // �����̉~
				const float CIRCLE2_RADIUS = 100.0f;    // �O���̉~
				const float BULLET_SPEED = 150.f;       // �e�̑���

				float angle = (2.0f * tnl::PI / BULLETCOUNT_EACHCIRCLE) * bullet->_id;

				// �e��9�̃O���[�v�ɕ�����
				int wave = bullet->_id / 90;

				//�@��P�g---------------------------------------
				if (wave == 0)
				{
					// ����----------------------------------------------------------
					if (bullet->_id < 45) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 45 && bullet->_id < 90) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
				}
				//�@��Q�g---------------------------------------
				if (wave == 1)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 90 && bullet->_id > 135) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 135 && bullet->_id < 180) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}

				}
				//�@��R�g---------------------------------------
				if (wave == 2)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 180 && bullet->_id > 225) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 225 && bullet->_id < 270) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
				}
				//�@��S�g---------------------------------------
				if (wave == 3)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 270 && bullet->_id > 315) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 315 && bullet->_id < 360) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}

				}
				//�@��T�g---------------------------------------
				if (wave == 4)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 360 && bullet->_id > 405) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 405 && bullet->_id < 450) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}

				}
				//�@��U�g---------------------------------------
				if (wave == 5)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 450 && bullet->_id > 495) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 495 && bullet->_id < 540) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
				}
				//�@��V�g---------------------------------------
				if (wave == 6)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 540 && bullet->_id > 585) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 585 && bullet->_id < 630) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
				}
				//�@��W�g---------------------------------------
				if (wave == 7)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 630 && bullet->_id > 675) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 675 && bullet->_id < 720) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
				}
				//�@��X�g---------------------------------------
				if (wave == 8)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 720 && bullet->_id > 765) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 765 && bullet->_id < 810) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
				}
				//�@��P�O�g---------------------------------------
				if (wave == 9)
				{
					// ����----------------------------------------------------------
					if (bullet->_id >= 810 && bullet->_id > 855) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE1_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
					}
					// �O��----------------------------------------------------------
					if (bullet->_id >= 855 && bullet->_id < 900) {

						WaveAssist_IronRingOfMoriya_Suwako(
							bullet,
							CIRCLE2_RADIUS,
							angle,
							deltaTime,
							BULLET_SPEED,
							wave
						);
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
		tnl::Vector3 spawnPos = bossPosition;
		tnl::Vector3 targetDir;

		// ������
		if (bullet->_timer == 0.f) {

			spawnPos.x += circleRadius * cos(angle);
			spawnPos.z += circleRadius * sin(angle);
			bullet->_mesh->pos_ = spawnPos;
		}

		// �^�C�}�[�N��
		bullet->_timer += deltaTime;

		//�@�ړ��J�n
		if (bullet->_timer > startMoveTime) {

			bullet->_isActive = true;

			// �v���C���[�����擾
			targetDir = _player_ref->GetPos() - spawnPos;
			targetDir.normalize();

			//�@�v���C���[��ǂ�������i�@���`��ԁ@�j
			bullet->_moveDirection = tnl::Vector3::UniformLerp(
				bullet->_mesh->pos_,
				targetDir * 0.8f,
				1.5f,
				bullet->_timer
			);

			//�@�X�V
			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
		}
		//�@���Z�b�g
		if (bullet->_timer > startMoveTime + 10.f) {

			bullet->_isActive = false;
			bullet->_timer = 0.f;
		}
	}


	void BulletHell::ShotBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size(); i++) {

			auto bullet = EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako[i];

			// ��֕����Ă��牺�֍~�낷
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

				// ������
				if (bullet->_timer == 0.f) {
					InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(deltaTime, 50.f, bullet, true);
				}

				//�@�X�V
				bullet->_timer += deltaTime;
				UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(bullet, deltaTime);
			}

			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_EmeraldGreen) {

				// ������
				if (bullet->_timer == 0.f) {
					InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(deltaTime, 30.f, bullet, false);
				}

				//�@�X�V
				bullet->_timer += deltaTime;
				UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(bullet, deltaTime);
			}
		}
	}


	void BulletHell::InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
		const float deltaTime,
		const float upwardPower,
		Shared<EnemyBullet>& bullet,
		const bool isOscillate)
	{
		// �����ʒu
		tnl::Vector3 spawnPos = _bossMesh_ref->pos_;
		//�@�����_���x�N�^�[����
		tnl::Vector3 randomVector = RandomValueGenerator::Vector(-500, 500, 50, 150, -200, 0);

		// �ړ�����
		tnl::Vector3 moveDir;
		moveDir += spawnPos + randomVector;
		moveDir.y += upwardPower;          // �O�����֗͂�������
		moveDir.normalize();

		bullet->_moveDirection = moveDir;

		if (isOscillate) {

			const float OSCILLATE_RATE{ 1.3f };
			float bloom = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0, OSCILLATE_RATE, deltaTime);

			// �����ʒu�����E�ɓ���������
			bullet->_mesh->pos_ = spawnPos + tnl::Vector3(bloom * 3, 50, 0);
		}
		else {

			//�@�e�����ʒu���{�X�̓���ɐݒ�
			bullet->_mesh->pos_ = spawnPos + tnl::Vector3(0, 50, 0);
		}
	}


	void BulletHell::UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
		Shared<EnemyBullet>& bullet,
		const float deltaTime)
	{
		const float GRAVITY_ACCELARATION = -9.8f;  // �d�͉����x
		const float BULLET_SPEED = 200.0f;         // �e�̃X�s�[�h
		const float LIFE_TIME = 6.0f;              // �e�̎���
		const float OFFSET = 0.1f;                 // ���ˊԊu�x���p�W��
		const int BULLET_NUM_EACH_GROUP = 10;      // �P�񂲂ƂɌ��e�̐�

		// ���˃^�C�~���O
		float startTiming = OFFSET * (bullet->_id / BULLET_NUM_EACH_GROUP);

		//�@�X�V�J�n
		if (bullet->_timer >= startTiming) {

			bullet->_isActive = true;

			tnl::Vector3 gravity = { 0.f, GRAVITY_ACCELARATION * deltaTime, 0.f };

			bullet->_moveDirection += gravity * deltaTime;
			bullet->_moveDirection.normalize();

			//�@�X�V
			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}

		//�@���Z�b�g
		if (bullet->_timer > startTiming + LIFE_TIME) {

			bullet->_isActive = false;

			bullet->_timer = 0.f;

			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}
}