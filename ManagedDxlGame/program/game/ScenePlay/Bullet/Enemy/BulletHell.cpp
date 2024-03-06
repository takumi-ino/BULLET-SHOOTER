#include "BulletHell.h"
#include "BulletHellFactory.h"
#include "EnemyBullet.h"
#include "../../ScenePlay.h"
#include "../../Character/Player/Player.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "../game/ScenePlay/Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"



void BulletHell::ShotBulletHell_Normal_Patchouli(const float& delta_time) {
	
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

	float roundBullet_radius = 90.0f;

	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
	tnl::Vector3 roundBullet_moveDirection;


	for (auto& bullet : sphereRoundBlue) {

		// ç∂âÒì]
		if (bullet->_id % 2 == 0)
			bullet->_angle += delta_time;

		// âEâÒì]
		if (bullet->_id % 2 != 0)
			bullet->_angle -= delta_time;

		// X
		bullet->_mesh->pos_.x = bossPosition.x + roundBullet_radius * cos(bullet->_angle);
		// Y
		bullet->_mesh->pos_.y = bossPosition.y;
		// Z
		bullet->_mesh->pos_.z = bossPosition.z + roundBullet_radius * sin(bullet->_angle);
		// ìÆÇ≠ï˚å¸
		roundBullet_moveDirection.x =
			bossPosition.x + cos(bullet->_angle) * roundBullet_radius;
		roundBullet_moveDirection.z =
			bossPosition.z + sin(bullet->_angle) * roundBullet_radius;

		roundBullet_moveDirection.normalize();

		bullet->_mesh->pos_.x += roundBullet_moveDirection.x * delta_time * 100;
		bullet->_mesh->pos_.z += roundBullet_moveDirection.z * delta_time * 100;
	}

	for (auto& bullet : cylinderRoundBlue) {

		// èâíeÇ…í«è]Ç∑ÇÈåıê¸

		// ç∂âÒì]
		if (bullet->_id % 2 == 0)
			bullet->_angle += delta_time;
		// âEâÒì]
		else
			bullet->_angle -= delta_time;

		// X
		bullet->_mesh->pos_.x = bossPosition.x + (roundBullet_radius + 150) * cos(bullet->_angle);
		// Y
		bullet->_mesh->pos_.y = bossPosition.y;
		// Z
		bullet->_mesh->pos_.z = bossPosition.z + (roundBullet_radius + 150) * sin(bullet->_angle);

		bullet->_mesh->pos_.x += roundBullet_moveDirection.x * delta_time * 100;
		bullet->_mesh->pos_.z += roundBullet_moveDirection.z * delta_time * 100;
	}

	// ëSï˚à  ÇPâÒÇ…Ç¬Ç´16î≠*3òAë±ÇÃíeÇé¸àÕÇ…î≠éÀÅBÇªÇÍÇÇSÉZÉbÉg
	for (auto& bullet : sphereRoundRed) {

		float every_direction_bullet_radius = 100.0f;
		float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

		float offset = 0.5f;
		float timing = offset * (bullet->_id / 16);


		if (bullet->_isActive == 0) {

			tnl::Vector3 moveDir, spawn_pos;

			spawn_pos.x = bossPosition.x + every_direction_bullet_radius * cos(angle);
			spawn_pos.y = bossPosition.y;
			spawn_pos.z = bossPosition.z + every_direction_bullet_radius * sin(angle);

			moveDir.x = cos(angle) * every_direction_bullet_radius;
			moveDir.z = sin(angle) * every_direction_bullet_radius;

			moveDir.normalize();

			bullet->_mesh->pos_ = spawn_pos;

			bullet->_moveDirection = moveDir;
		}

		bullet->_timer += delta_time;

		if (bullet->_isActive && bullet->_timer >= timing) {

			if (bullet->_timer > 3) {

				bullet->_isActive = false;
				bullet->_timer = 0;

				bullet->_mesh->pos_.x = bossPosition.x + every_direction_bullet_radius * cos(angle);
				bullet->_mesh->pos_.y = bossPosition.y;
				bullet->_mesh->pos_.z = bossPosition.z + every_direction_bullet_radius * sin(angle);
			}
		}

		bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
	}
}



void BulletHell::ShotBulletHell_MetalFatigue_Patchouli(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.size(); i++) {

		auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli[i];

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow) {

			float angle_origin =
				(2.0f * tnl::PI / EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) *
				(i % EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI);

			float radius_origin = 150.0f;

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			// èâíe ÇWå¬
			if (bullet->_id < 8) {

				bullet->_timer += delta_time;
				bullet->_isActive = true;

				if (bullet->_timer > 0.5) {

					bullet->_isActive = false;
					bullet->_mesh->pos_ = bossPosition;
					bullet->_timer = 0;
				}

				tnl::Vector3 bullet_move_orbit_origin;

				bullet_move_orbit_origin.x = bossPosition.x + cos(angle_origin) * radius_origin;
				bullet_move_orbit_origin.y = bossPosition.y + sin(angle_origin) * radius_origin;

				bullet_move_orbit_origin.normalize();

				bullet->_moveDirection = bullet_move_orbit_origin;

				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 300.0f;
			}
			//Å@1îg 64å¬
			if (bullet->_id >= 8 && bullet->_id < 8 + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) {

				WaveAssist_MetalFatigue_Patchouli(bullet, bossPosition, delta_time, angle_origin, radius_origin, 0.f);
			}

			if (bullet->_id >= 8 + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI &&
				bullet->_id < 8 + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2)) {

				WaveAssist_MetalFatigue_Patchouli(bullet, bossPosition, delta_time, angle_origin, radius_origin, 0.1f);
			}

			if (bullet->_id >= 8 + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) * 2 &&
				bullet->_id < 8 + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 3)) {

				WaveAssist_MetalFatigue_Patchouli(bullet, bossPosition, delta_time, angle_origin, radius_origin, 0.2f);
			}
		}
	}
}



void BulletHell::WaveAssist_MetalFatigue_Patchouli(
	Shared<EnemyBullet>& bullet,
	tnl::Vector3& bossPosition, 
	const float& delta_time, 
	float angle_origin, 
	float radius_origin,
	float startMoveTime)
{
	tnl::Vector3 bullet_move_orbit_wave = bullet->_moveDirection;

	if (0 == bullet->_timer) {
		// íeÇÉ{ÉXÇÃà íuÇ÷ÉZÉbÉg
		bullet->_mesh->pos_ = bossPosition;
	}

	bullet->_timer += delta_time;

	// 1
	if (bullet->_timer > startMoveTime && bullet->_timer < startMoveTime + 0.5) {

		bullet->_isActive = true;

		bullet_move_orbit_wave.x = bossPosition.x + cos(angle_origin) * radius_origin;
		bullet_move_orbit_wave.y = bossPosition.y + sin(angle_origin) * radius_origin;

		bullet->_moveDirection = bullet_move_orbit_wave;
	}
	// 2
	if (bullet->_timer >= startMoveTime + 0.5 && bullet->_timer < startMoveTime + 1.0) {

		float angle = (2.0f * tnl::PI / 8) * (bullet->_id / 8);

		bullet_move_orbit_wave.x = cos(angle) * radius_origin;
		bullet_move_orbit_wave.y = sin(angle) * radius_origin;
		bullet_move_orbit_wave.z = sin(angle) * radius_origin;


		bullet->_moveDirection = bullet_move_orbit_wave;
	}
	// 3
	if (bullet->_timer > startMoveTime + 2) {

		bullet->_isActive = false;

		bullet->_mesh->pos_ = bossPosition;
		bullet->_timer = 0;
	}

	bullet->_mesh->pos_ += bullet->_moveDirection * delta_time;
}




void BulletHell::ShotBulletHell_SilentSerena_Patchouli(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.size(); i++) {

		auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Sky) {

			// íeÇâ~é¸è„Ç…ìôä‘äuÇ≈îzíuÇ∑ÇÈ
			float angle = (2.0f * tnl::PI / CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI) * (bullet->_id % CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI);
			float phi = (tnl::PI / 16) * (bullet->_id % 16);

			float radius = 20.0f;

			if (bullet->_timer == 0) {

				tnl::Vector3 spawnPos = bossPosition;

				bullet->_mesh->pos_.x = spawnPos.x + sin(phi) * cos(angle) * radius;
				bullet->_mesh->pos_.y = spawnPos.y + sin(phi) * sin(angle) * radius;
				bullet->_mesh->pos_.z = spawnPos.z + cos(phi) * radius;

				bullet->_moveDirection.x = sin(phi) * cos(angle);
				bullet->_moveDirection.y = sin(phi) * sin(angle);
				bullet->_moveDirection.z = cos(phi);

				bullet->_moveDirection.normalize();
			}

			bullet->_timer += delta_time;

			if (bullet->_timer > 0 && bullet->_timer < 5) {

				bullet->_isActive = true;

				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
			}


			if (bullet->_timer > 6) {
				bullet->_isActive = false;

				bullet->_timer = 0;
				bullet->_mesh->pos_ = bossPosition;
			}
		}


		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue) {

			std::mt19937 mt(rd());

			std::uniform_real_distribution<float> dist_x(-400, 400);
			std::uniform_real_distribution<float> dist_y(-50.0, 150.0);
			std::uniform_real_distribution<float> dist_z(-250, 200);

			tnl::Vector3 move_dir = bullet->_moveDirection;

			if (bullet->_timer == 0) {
				tnl::Vector3 spawn_pos;

				spawn_pos.x = bossPosition.x + dist_x(mt);
				spawn_pos.y = bossPosition.y + dist_y(mt);
				spawn_pos.z = bossPosition.z + dist_z(mt);

				move_dir = _player_ref->GetPos() - spawn_pos;

				move_dir.normalize();

				bullet->_mesh->pos_ = spawn_pos;
				bullet->_moveDirection = move_dir;

				bullet->_isActive = true;
			}

			bullet->_timer += delta_time;

			bullet->_mesh->pos_ += move_dir * delta_time * 50.f;

			if (bullet->_timer > 7) {

				bullet->_mesh->pos_ = bossPosition;

				bullet->_timer = 0;
				bullet->_isActive = false;
			}
		}
	}
}



void BulletHell::ShotBulletHell_Normal_Cirno(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_Cirno::_bullet_normal_cirno.size(); i++) {

		auto bullet = EnemyBoss_Cirno::_bullet_normal_cirno[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// ÉVÉáÉbÉgÉKÉìÅi2 + N)çs (NÇÕ0Å`5Ç‹Ç≈)ÅAÇUóÒÇÃíeÇÉvÉåÉCÉÑÅ[Ç÷î≠éÀ (81î≠Åj
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_Blue) {

			int wave = bullet->_id / (STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO / 3);


			if (bullet->_timer == 0) {

				tnl::Vector3 spawn_pos = bossPosition;

				float angle = (tnl::PI / 8) * (bullet->_id % 8);

				tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos;

				moveDir.x += cos(angle) * 70;

				moveDir.normalize();

				bullet->_moveDirection = moveDir;

				bullet->_mesh->pos_ = spawn_pos;
			}

			switch (wave)
			{
			case 0:
			{
				bullet->_timer += delta_time;

				float offset = 0.1f;
				float timing = offset * (bullet->_id / 8);

				if (bullet->_timer >= timing) {
					bullet->_isActive = true;

					bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;

					if (bullet->_timer > 4) {
						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}

			break;
			case 1:
			{
				bullet->_timer += delta_time;

				float offset = 0.1f;
				float timing = offset * (bullet->_id / 8);

				if (bullet->_timer >= timing + 2) {
					bullet->_isActive = true;

					bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;

					if (bullet->_timer > 6) {
						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			break;
			case 2:
			{
				bullet->_timer += delta_time;

				float offset = 0.1f;
				float timing = offset * (bullet->_id / 8);

				if (bullet->_timer >= timing + 4) {
					bullet->_isActive = true;

					bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;

					if (bullet->_timer > 8) {
						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			break;
			}
		}
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {


			float radius = 100.0f;
			float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

			float offset = 0.5f;
			float timing = offset * (bullet->_id / 16);

			bullet->_timer += delta_time;

			if (bullet->_timer >= timing) {
				bullet->_isActive = true;

				tnl::Vector3 moveDir, spawn_pos;

				spawn_pos.x = bossPosition.x + radius * cos(angle);
				spawn_pos.y = bossPosition.y;
				spawn_pos.z = bossPosition.z + radius * sin(angle);

				moveDir.x = cos(angle) * radius;
				moveDir.z = sin(angle) * radius;

				moveDir.normalize();

				bullet->_mesh->pos_ = spawn_pos;

				bullet->_moveDirection = moveDir;

				if (bullet->_timer > 3) {

					bullet->_isActive = false;
					bullet->_timer = 0;

					spawn_pos.x = bossPosition.x + radius * cos(angle);
					spawn_pos.y = bossPosition.y;
					spawn_pos.z = bossPosition.z + radius * sin(angle);
				}
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
		}
	}
}




void BulletHell::ShotBulletHell_IcicleFall_Cirno(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_Cirno::_bullet_icicleFall_cirno.size(); i++) {

		auto bullet = EnemyBoss_Cirno::_bullet_icicleFall_cirno[i];

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		float radius = 300.f + (i % 5) * 15.f;
		float angle = (tnl::PI / 10) * (bullet->_id % 10);

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {


			int shot = bullet->_id / 10; // íeÇÃÉVÉáÉbÉgî‘çÜÅi0Å`9Åj
			float delay = 0.5f; // ÉVÉáÉbÉgä‘ÇÃíxâÑéûä‘
			if (shot == 0) // ç≈èâÇÃÉVÉáÉbÉgÇÕÇ∑ÇÆÇ…î≠éÀ
			{
				bullet->_timer = 0;
			}
			else
			{
				bullet->_timer = -delay * shot;
			}

			bullet->_timer += delta_time;

			if (bullet->_timer > 0)
			{
				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
				bullet->_isActive = true;
			}
			if (bullet->_timer >= 1 && bullet->_timer < 2)
			{
				bullet->_moveDirection = tnl::Vector3(0, 0, 0);
			}
			if (bullet->_timer >= 2) {

				tnl::Vector3 target_pos;
				target_pos.x = bossPosition.x;
				target_pos.y = bossPosition.y;
				target_pos.z = bossPosition.z - 100;
				target_pos.normalize();

				tnl::Vector3 diff = target_pos - bullet->_mesh->pos_;

				float adjust = 0.1f;
				bullet->_moveDirection += diff * adjust;
				bullet->_moveDirection.normalize();

				bullet->_moveDirection *= 50;
			}
			if (bullet->_timer > 7) // É^ÉCÉ}Å[Ç™4ïbÇí¥Ç¶ÇΩÇÁè¡ãé
			{
				bullet->_isActive = false; // íeÇñ≥å¯Ç…Ç∑ÇÈ
				bullet->_timer = 0; // É^ÉCÉ}Å[ÇÉäÉZÉbÉgÇ∑ÇÈ
				bullet->_mesh->pos_ = bossPosition; // íeÇÃà íuÇå≥Ç…ñﬂÇ∑
			}
		}

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow) {

			float straight_bullet_radius = 100.0f;

			float offset = 0.5f; // íeÇÃÇ∏ÇÍïù
			float phi = (tnl::PI / 8) * (bullet->_id % 8);
			bullet->_timer += delta_time;

			if (bullet->_id < 5) {

				if (bullet->_timer == 0) {
					tnl::Vector3 move_dir = _player_ref->GetPos() - bossPosition;
					move_dir.x += cos(phi) * 70;

					move_dir.normalize();

					bullet->_moveDirection = move_dir;
					bullet->_mesh->pos_.x = -50 + cos(phi) * straight_bullet_radius;

					bullet->_isActive = true;
				}


				if (bullet->_timer > 5) {

					bullet->_isActive = false;
					bullet->_timer = 0;
					bullet->_mesh->pos_ = bossPosition;
				}
			}
			if (bullet->_id >= 5 && bullet->_id < 10) {

				if ((int)bullet->_timer == 1) {
					tnl::Vector3 move_dir = _player_ref->GetPos() - bossPosition;
					move_dir.x += cos(phi) * 70;

					move_dir.normalize();

					bullet->_moveDirection = move_dir;
					bullet->_mesh->pos_.x = -50 + cos(phi) * straight_bullet_radius;

					bullet->_isActive = true;
				}
				if (bullet->_timer > 5 + 1) {

					bullet->_isActive = false;
					bullet->_timer = 0;
					bullet->_mesh->pos_ = bossPosition;
				}

			}
			if (bullet->_id >= 10 &&
				bullet->_id < 15) {

				if ((int)bullet->_timer == 2) {
					tnl::Vector3 move_dir = _player_ref->GetPos() - bossPosition;
					move_dir.x += cos(phi) * 70;

					move_dir.normalize();

					bullet->_moveDirection = move_dir;
					bullet->_mesh->pos_.x = -50 + cos(phi) * straight_bullet_radius;

					bullet->_isActive = true;
				}
				if (bullet->_timer > 5 + 2) {

					bullet->_isActive = false;
					bullet->_timer = 0;
					bullet->_mesh->pos_ = bossPosition;

				}
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
		}
	}
}


void BulletHell::ShotBulletHell_PerfectFreeze_Cirno(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.size(); i++) {

		auto bullet = EnemyBoss_Cirno::_bullet_perfectFreeze_cirno[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_White) {

			std::mt19937 mt(rd());

			int shotNum = 0;
			float radius = 150.0f;
			tnl::Vector3 everyDirBullet_moveDirection;

			if (bullet->_timer == 0) {

				tnl::Vector3 spawn_pos = bossPosition;

				std::uniform_int_distribution<int> rnd_val(-250, 50);
				std::uniform_int_distribution<int> rnd_valZ(-150, 150);

				// íeÇâ~é¸è„Ç…ìôä‘äuÇ≈îzíuÇ∑ÇÈ
				float angle = (2.0f * tnl::PI / 80) * (shotNum % 80);
				spawn_pos.x += cos(angle) + rnd_val(mt);
				spawn_pos.y += sin(angle) + rnd_val(mt);
				spawn_pos.z += sin(angle) + rnd_valZ(mt);


				everyDirBullet_moveDirection.x = cos(angle) * radius + rnd_val(mt);
				everyDirBullet_moveDirection.y = sin(angle) * radius + rnd_valZ(mt);
				everyDirBullet_moveDirection.z = sin(angle) * radius + rnd_val(mt);

				everyDirBullet_moveDirection.normalize();

				bullet->_isActive = true;
				bullet->_moveDirection = everyDirBullet_moveDirection;
				bullet->_mesh->pos_ = spawn_pos;
			}

			bullet->_timer += delta_time;


			if (bullet->_timer < 2) {

				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 0.1f;
			}
			else if (bullet->_timer > 2 && bullet->_timer < 5) {

				bullet->_moveDirection = tnl::Vector3(0, 0, 0); // íeÇÃë¨ìxÇ0Ç…Ç∑ÇÈ
			}
			else if ((int)bullet->_timer == 5) {

				std::uniform_int_distribution<int> rnd_val(-250, 50);
				std::uniform_int_distribution<int> rnd_valZ(-150, 150);

				// íeÇâ~é¸è„Ç…ìôä‘äuÇ≈îzíuÇ∑ÇÈ
				float angle = (2.0f * tnl::PI / 80) * (shotNum % 80);
				everyDirBullet_moveDirection.x = cos(angle) * radius + rnd_val(mt);
				everyDirBullet_moveDirection.y = sin(angle) * radius + rnd_valZ(mt);
				everyDirBullet_moveDirection.z = sin(angle) * radius + rnd_val(mt);

				everyDirBullet_moveDirection.normalize();
				bullet->_moveDirection = everyDirBullet_moveDirection;
				shotNum = (shotNum + 1) % 80;
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;

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

			bullet->_timer += delta_time;

			float offset = 0.1f;
			float timing = offset * (bullet->_id / 8);

			if (bullet->_timer >= timing) {

				bullet->_isActive = true;

				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;
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




void BulletHell::ShotBulletHell_Normal_Suwako(const float& delta_time) {
	// ëSï˚à  ÇPâÒÇ…Ç¬Ç´ÇUÇSî≠ê∂ê¨ÅAÇWï˚å¸Ç÷ñ≥å¿Ç…îÚÇŒÇµë±ÇØÇÈ

	for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.size(); i++) {

		auto bullet = EnemyBoss_MoriyaSuwako::_bullet_normal_suwako[i];
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// âEâÒÇË
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {


			if (bullet->_timer == 0) {

				float every_direction_bullet_radius = 100.0f;
				float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

				tnl::Vector3 spawn_pos = bossPosition;
				tnl::Vector3 moveDirection;

				spawn_pos.x = cos(angle) * bullet->_id;

				spawn_pos.x += bullet->_id * 1.1f;
				spawn_pos.z -= bullet->_id * 1.1f;

				moveDirection.x = cos(angle) * every_direction_bullet_radius;
				moveDirection.z = sin(angle) * every_direction_bullet_radius;

				moveDirection.normalize();

				bullet->_mesh->pos_ = spawn_pos;
				bullet->_moveDirection = moveDirection;
			}


			bullet->_timer += delta_time;

			int row = bullet->_id / 64;

			if (row == 0)
			{
				if (bullet->_timer > 0) {

					bullet->_isActive = true;

					if (bullet->_timer > 1) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 1)
			{
				if (bullet->_timer > 1) {

					bullet->_isActive = true;

					if (bullet->_timer > 2) {
						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 2)
			{
				if (bullet->_timer > 2) {

					bullet->_isActive = true;

					if (bullet->_timer > 3) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 3)
			{
				if (bullet->_timer > 3) {

					bullet->_isActive = true;

					if (bullet->_timer > 4) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 4)
			{
				if (bullet->_timer > 4) {

					bullet->_isActive = true;

					if (bullet->_timer > 5) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}

			bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;
		}

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {


			if (bullet->_timer == 0) {

				float every_direction_bullet_radius = 100.0f;
				float angle = (2.0f * tnl::PI / 16) * (bullet->_id % 16);

				tnl::Vector3 spawn_pos = bossPosition;
				tnl::Vector3 moveDirection;

				spawn_pos.x = sin(angle) * -bullet->_id;

				spawn_pos.x += bullet->_id * 1.1f;
				spawn_pos.z -= bullet->_id * 1.1f;

				moveDirection.x -= cos(angle) * every_direction_bullet_radius;
				moveDirection.z -= sin(angle) * every_direction_bullet_radius;

				moveDirection.normalize();

				bullet->_mesh->pos_ = spawn_pos;

				bullet->_moveDirection = moveDirection;
			}


			bullet->_timer += delta_time;

			int row = bullet->_id / 64;

			if (row == 0)
			{
				if (bullet->_timer > 0) {

					bullet->_isActive = true;

					if (bullet->_timer > 1) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 1)
			{
				if (bullet->_timer > 1) {

					bullet->_isActive = true;

					if (bullet->_timer > 2) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 2)
			{
				if (bullet->_timer > 2) {

					bullet->_isActive = true;

					if (bullet->_timer > 3) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 3)
			{
				if (bullet->_timer > 3) {

					bullet->_isActive = true;

					if (bullet->_timer > 4) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}
			if (row == 4)
			{
				if (bullet->_timer > 4) {

					bullet->_isActive = true;

					if (bullet->_timer > 5) {

						bullet->_isActive = false;
						bullet->_timer = 0;
						bullet->_mesh->pos_ = bossPosition;
					}
				}
			}


			bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 500;
		}
	}
}


void BulletHell::ShotBulletHell_IronRingOfMoriya_Suwako(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.size(); i++) {

		auto bullet = EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako[i];

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
		float bullet_speed = 150.f;

		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {


			float circle1_radius = 80.0f;
			float circle2_radius = 100.0f;

			float angle = (2.0f * tnl::PI / 45) * (bullet->_id % 45);

			int row = bullet->_id / 90;

			if (row == 0)
			{

				if (bullet->_id < 45) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 45 &&
					bullet->_id < 90) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}
			}
			if (row == 1)
			{
				if (bullet->_id >= 90 &&
					bullet->_id > 135) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 135 &&
					bullet->_id < 180) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 2)
			{
				if (bullet->_id >= 180 &&
					bullet->_id > 225) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 225 &&
					bullet->_id < 270) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}
			}
			if (row == 3)
			{
				if (bullet->_id >= 270 &&
					bullet->_id > 315) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 315 &&
					bullet->_id < 360) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 4)
			{
				if (bullet->_id >= 360 &&
					bullet->_id > 405) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 405 &&
					bullet->_id < 450) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 5)
			{
				if (bullet->_id >= 450 &&
					bullet->_id > 495) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 495 &&
					bullet->_id < 540) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 6)
			{
				if (bullet->_id >= 540 &&
					bullet->_id > 585) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 585 &&
					bullet->_id < 630) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 7)
			{
				if (bullet->_id >= 630 &&
					bullet->_id > 675) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 675 &&
					bullet->_id < 720) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 8)
			{
				if (bullet->_id >= 720 &&
					bullet->_id > 765) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 765 &&
					bullet->_id < 810) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

			}
			if (row == 9)
			{
				if (bullet->_id >= 810 &&
					bullet->_id > 855) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle1_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}

				if (bullet->_id >= 855 &&
					bullet->_id < 900) {

					WaveAssist_IronRingOfMoriya_Suwako(
						bullet,
						circle2_radius,
						angle,
						delta_time,
						bullet_speed,
						row);
				}
			}
		}
	}
}



void BulletHell::WaveAssist_IronRingOfMoriya_Suwako(
	Shared<EnemyBullet>& bullet,
	float circle_radius,
	float angle,
	float delta_time,
	float bullet_speed,
	int startMove_time)
{
	tnl::Vector3 bossPosition = _bossMesh_ref->pos_;


	if (bullet->_timer == 0) {
		tnl::Vector3 spawn_pos = bossPosition;
		spawn_pos.x += circle_radius * cos(angle);
		spawn_pos.z += circle_radius * sin(angle);

		bullet->_moveDirection = _player_ref->GetPos() - spawn_pos;
		bullet->_moveDirection += spawn_pos;
		bullet->_moveDirection.normalize();

		bullet->_mesh->pos_ = spawn_pos;
	}

	bullet->_timer += delta_time;

	if (bullet->_timer > startMove_time) {
		bullet->_isActive = true;

		bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * bullet_speed;

	}
	else {
		bullet->_isActive = false;
	}


	if (bullet->_timer > startMove_time + 10) {
		bullet->_isActive = false;
		bullet->_timer = 0;

		bullet->_mesh->pos_.x = bossPosition.x + circle_radius * cos(angle);
		bullet->_mesh->pos_.y = bossPosition.y;
		bullet->_mesh->pos_.z = bossPosition.z + circle_radius * sin(angle);
	}
}

tnl::Vector3 upward_velocity = { 0,150,100 };

void BulletHell::ShotBulletHell_KeroChanStandsFirm_AgainstTheStorm_Suwako(const float& delta_time) {

	for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size(); i++) {

		auto bullet = EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako[i];

		std::mt19937 mt(rd());

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
		tnl::Vector3 spawn_pos = bossPosition;

		// è„Ç÷ï˙Ç¡ÇƒÇ©ÇÁâ∫Ç÷ç~ÇÎÇ∑
		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

			if (bullet->_timer == 0) {

				float angle = (tnl::PI / 10) * (bullet->_id % 10);

				tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos;

				std::uniform_real_distribution<float> diff_x(-500.f, 500.f);
				std::uniform_real_distribution<float> diff_y(50, 150.f);
				std::uniform_real_distribution<float> diff_z(-200.f, 0.f);

				tnl::Vector3 offset = { diff_x(mt) ,diff_y(mt),diff_z(mt) };

				moveDir += spawn_pos + offset;
				moveDir.y += upward_velocity.y;


				moveDir.normalize();

				bullet->_moveDirection = moveDir;

				bullet->_mesh->pos_ = spawn_pos + tnl::Vector3(0, 50, 0);
			}

			bullet->_timer += delta_time;

			float offset = 0.1f;
			float timing = offset * (bullet->_id / 10);

			if (bullet->_timer >= timing) {
				bullet->_isActive = true;

				float scale = -9.8f;
				float increaseRate = 1000.0f;
				scale += increaseRate * delta_time;

				tnl::Vector3 gravity = { 0,scale,0 };


				bullet->_mesh->pos_ += gravity * delta_time;

				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
			}

			if (bullet->_timer > 10) {

				bullet->_isActive = false;

				bullet->_timer = 0;

				bullet->_mesh->pos_ = spawn_pos;
			}
		}


		if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_EmeraldGreen) {


			if (bullet->_timer == 0) {

				float angle = (tnl::PI / 10) * (bullet->_id % 10);

				tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos;


				std::uniform_real_distribution<float> diff_x(-500.f, 500.f);
				std::uniform_real_distribution<float> diff_y(50, 150.f);
				std::uniform_real_distribution<float> diff_z(-200.f, 0.f);

				tnl::Vector3 offset = { diff_x(mt) ,diff_y(mt),diff_z(mt) };

				moveDir += spawn_pos + offset;
				tnl::Vector3 upward_velocity = { 0,150,0 };
				moveDir.y += upward_velocity.y;


				moveDir.normalize();

				bullet->_moveDirection = moveDir;

				bullet->_mesh->pos_ = spawn_pos;
			}

			bullet->_timer += delta_time;

			float offset = 0.1f;
			float timing = offset * (bullet->_id / 10);

			if (bullet->_timer >= timing) {
				bullet->_isActive = true;

				float scale = -9.8f;
				float increaseRate = 1000.0f;
				scale += increaseRate * delta_time;

				tnl::Vector3 gravity = { 0,scale,0 };


				bullet->_mesh->pos_ += gravity * delta_time;

				bullet->_mesh->pos_ += bullet->_moveDirection * delta_time * 200;
			}

			if (bullet->_timer > 10) {

				bullet->_isActive = false;

				bullet->_timer = 0;

				bullet->_mesh->pos_ = spawn_pos;
			}
		}
	}
}