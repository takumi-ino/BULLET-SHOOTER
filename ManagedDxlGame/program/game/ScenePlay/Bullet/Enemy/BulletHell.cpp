#include <random>
#include "BulletHell.h"
#include "BulletHellFactory.h"
#include "EnemyBullet.h"
#include "../../ScenePlay.h"
#include "../../Character/Player/Player.h"

static bool isDividedBulletGroup = false;
static bool isDivided_sphereRoundBullets_emeraldGreen = false;


BulletHell::BulletHell(const Shared<dxe::Mesh>& bossMesh, const Shared<Player>& player) : _bossMesh_ref(bossMesh) {
	_player_ref = player;
}



void BulletHell::ShotBulletHell_Normal_Patchouli(const float& delta_time) {


	for (int i = 0; i < ScenePlay::_bullet_normal_patchouli.size(); i++) {

		float roundBullet_radius = 90.0f;

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
		tnl::Vector3 roundBullet_moveDirection;

		// ‰’eAƒ{ƒX‚ÌŽüˆÍ‚ðù‰ñ‚·‚é
		if (ScenePlay::_bullet_normal_patchouli[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {


			// ¶‰ñ“]
			if (ScenePlay::_bullet_normal_patchouli[i]->_id % 2 == 0)
				ScenePlay::_bullet_normal_patchouli[i]->_angle += delta_time;

			// ‰E‰ñ“]
			if (ScenePlay::_bullet_normal_patchouli[i]->_id % 2 != 0)
				ScenePlay::_bullet_normal_patchouli[i]->_angle -= delta_time;

			// X
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.x =
				(bossPosition.x + roundBullet_radius) * cos(ScenePlay::_bullet_normal_patchouli[i]->_angle);
			// Y
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.y = bossPosition.y;
			// Z
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.z =
				bossPosition.z + roundBullet_radius * sin(ScenePlay::_bullet_normal_patchouli[i]->_angle);
			// “®‚­•ûŒü
			roundBullet_moveDirection.x =
				bossPosition.x + cos(ScenePlay::_bullet_normal_patchouli[i]->_angle) * roundBullet_radius;
			roundBullet_moveDirection.z =
				bossPosition.z + sin(ScenePlay::_bullet_normal_patchouli[i]->_angle) * roundBullet_radius;

			roundBullet_moveDirection.normalize();

			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.x += roundBullet_moveDirection.x * delta_time * 100;
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.z += roundBullet_moveDirection.z * delta_time * 100;
		}

		// ‰’e‚É’Ç]‚·‚éŒõü
		if (ScenePlay::_bullet_normal_patchouli[i]->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_White) {

			// ¶‰ñ“]
			if (ScenePlay::_bullet_normal_patchouli[i]->_id % 2 == 0)
				ScenePlay::_bullet_normal_patchouli[i]->_angle += delta_time;
			// ‰E‰ñ“]
			else ScenePlay::_bullet_normal_patchouli[i]->_angle -= delta_time;


			// X
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.x =
				bossPosition.x + roundBullet_radius * cos(ScenePlay::_bullet_normal_patchouli[i]->_angle);
			// Y
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.y = bossPosition.y;
			// Z
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.z =
				bossPosition.z + ScenePlay::_bullet_normal_patchouli[i]->_radius * sin(ScenePlay::_bullet_normal_patchouli[i]->_angle);


			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.x += roundBullet_moveDirection.x * delta_time * 100;
			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_.z += roundBullet_moveDirection.z * delta_time * 100;
		}


		// ‘S•ûˆÊ ‚P‰ñ‚É‚Â‚«16”­*3˜A‘±‚Ì’e‚ðŽüˆÍ‚É”­ŽËB‚»‚ê‚ð‚SƒZƒbƒg
		if (ScenePlay::_bullet_normal_patchouli[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_EmeraldGreen) {


			float every_direction_bullet_radius = 100.0f;
			float angle = (2.0f * tnl::PI / 16) * (ScenePlay::_bullet_normal_patchouli[i]->_id % 16);

			float offset = 0.5f;
			float timing = offset * (ScenePlay::_bullet_normal_patchouli[i]->_id / 16);


			ScenePlay::_bullet_normal_patchouli[i]->_timer += delta_time;

			tnl::Vector3 everyDirBullet_moveDirection = ScenePlay::_bullet_normal_patchouli[i]->_moveDirection;
			tnl::Vector3 spawn_pos = ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_;

			CheckLifeTimeDistance(ScenePlay::_bullet_normal_patchouli[i], spawn_pos, 500);

			if (!ScenePlay::_bullet_normal_patchouli[i]->_isActive &&
				ScenePlay::_bullet_normal_patchouli[i]->_timer >= timing) {


				spawn_pos.x = (bossPosition.x + every_direction_bullet_radius) * cos(angle);
				spawn_pos.y = bossPosition.y;
				spawn_pos.z = (bossPosition.z + every_direction_bullet_radius) * sin(angle);

				everyDirBullet_moveDirection.x =
					spawn_pos.x + cos(angle) * every_direction_bullet_radius;
				everyDirBullet_moveDirection.z =
					spawn_pos.z + sin(angle) * every_direction_bullet_radius;

				everyDirBullet_moveDirection.normalize();

				ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_ = spawn_pos;

				ScenePlay::_bullet_normal_patchouli[i]->_moveDirection = everyDirBullet_moveDirection;


				ScenePlay::_bullet_normal_patchouli[i]->_isActive = true;
				ScenePlay::_bullet_normal_patchouli[i]->_timer = 0;
			}

			ScenePlay::_bullet_normal_patchouli[i]->_mesh->pos_ +=
				everyDirBullet_moveDirection * delta_time * 200;
		}
	}
}




std::map<Shared<EnemyBullet>, float> BulletHell::bullet_timers;



void BulletHell::ShotBulletHell_MetalFatigue_Patchouli(const float& delta_time) {


	for (int i = 0; i < ScenePlay::_bullet_metalFatigue_patchouli.size(); i++) {

		if (ScenePlay::_bullet_metalFatigue_patchouli[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow) {

			float angle =
				(2.0f * tnl::PI / _EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) *
				(i % _EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI);

			float radius_origin = 150.0f;

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;
			tnl::Vector3 bullet_move_orbit_origin;
			tnl::Vector3 spawn_pos = bossPosition;

			CheckLifeTimeDistance(ScenePlay::_bullet_metalFatigue_patchouli[i], spawn_pos, 350);

			// ‰’e
			if (ScenePlay::_bullet_metalFatigue_patchouli[i]->_id < 8 && !ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive) {

				ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive = true;


				ScenePlay::_bullet_metalFatigue_patchouli[i]->_mesh->pos_ = spawn_pos;

				bullet_move_orbit_origin.x = spawn_pos.x + cos(angle) * radius_origin;
				bullet_move_orbit_origin.y = spawn_pos.y + sin(angle) * radius_origin;


				bullet_move_orbit_origin.normalize();

				ScenePlay::_bullet_metalFatigue_patchouli[i]->_moveDirection = bullet_move_orbit_origin;

			}
			else if (ScenePlay::_bullet_metalFatigue_patchouli[i]->_id < 8 && ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive) {


				if (!ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive) {
					ScenePlay::_bullet_metalFatigue_patchouli[i]->_mesh->pos_ = bossPosition;
					ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive = true;

				}
				else
					ScenePlay::_bullet_metalFatigue_patchouli[i]->_mesh->pos_ +=
					ScenePlay::_bullet_metalFatigue_patchouli[i]->_moveDirection * delta_time * 300.0f;
			}


			float delay_wave2 = 2.5f;   // Sphere.Round.Wave1
			float delay_wave3 = 3.5f;   // Sphere.Round.Wave2

			// Še’eŽ©g‚ðƒL[‚Æ‚µ‚Ä‚»‚ê‚¼‚ê‚ªŒÅ—L‚ÌŽžŠÔ‚ðŒv‘ªEŠÇ—‚·‚é
			bullet_timers[ScenePlay::_bullet_metalFatigue_patchouli[i]] += delta_time;

			// Žè‡2 (•ª—ô’e‚»‚Ì1j
			if (
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_id > 8 &&
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_id < _SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI + 8 &&
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive) {


				CheckLifeTimeDistance(ScenePlay::_bullet_metalFatigue_patchouli[i], ScenePlay::_bullet_metalFatigue_patchouli[i]->_mesh->pos_, 400);

				ScenePlay::_bullet_metalFatigue_patchouli[i]->_mesh->pos_ =
					bossPosition + tnl::Vector3(cos(angle) * radius_origin, sin(angle) * radius_origin, 0.0f);

				ScenePlay::_bullet_metalFatigue_patchouli[i]->_moveDirection =
					tnl::Vector3(cos(angle) * 300.0f, sin(angle) * 300.0f, 0.0f);

				if (GetCurrentBulletMovedDistance(ScenePlay::_bullet_metalFatigue_patchouli[i]) > 150) {

					for (int j = 0; j < _SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI; j++) {
						// •ª—ô’e‚ÌƒCƒ“ƒfƒbƒNƒX‚ðŒvŽZ
						int k = 8 + j;

						// •ª—ô’e‚Ì”­ŽËŠp“x‚ðŒvŽZ
						float split_angle = (2.0f * tnl::PI / 8) * (j % _SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI);

						if (i == k)
							ScenePlay::_bullet_metalFatigue_patchouli[i]->_moveDirection =
							tnl::Vector3(cos(split_angle) * 200.0f, sin(split_angle) * 200.0f, 0.0f);
					}
				}

				ScenePlay::_bullet_metalFatigue_patchouli[i]->_mesh->pos_ +=
					ScenePlay::_bullet_metalFatigue_patchouli[i]->_moveDirection * delta_time * 300.0f;
			}
			// Žè‡3
			if (bullet_timers[ScenePlay::_bullet_metalFatigue_patchouli[i]] >= delay_wave2 &&
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_id > _SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI + 8 &&
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_id < (_SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2) + 8 &&
				!ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive) {

			}
			// Žè‡4
			if (bullet_timers[ScenePlay::_bullet_metalFatigue_patchouli[i]] >= delay_wave3 &&
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_id > (_SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2) + 8 &&
				ScenePlay::_bullet_metalFatigue_patchouli[i]->_id < (_SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 3) + 8 &&
				!ScenePlay::_bullet_metalFatigue_patchouli[i]->_isActive) {

			}
		}
	}
}




void BulletHell::ShotBulletHell_SilentSerena_Patchouli(const float& delta_time) {

	for (int i = 0; i < ScenePlay::_bullet_silentSerena_patchouli.size(); i++) {

		std::random_device rd;
		std::mt19937 mt(rd());


		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		if (ScenePlay::_bullet_silentSerena_patchouli[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_EmeraldGreen) {

			tnl::Vector3 spawn_pos = bossPosition;


			CheckLifeTimeDistance(ScenePlay::_bullet_silentSerena_patchouli[i], spawn_pos, 200);
			int shotNum = 0;
			float radius = 150.0f;

			std::uniform_int_distribution<int> rnd_val(-250, 50);
			std::uniform_int_distribution<int> rnd_valZ(-150, 150);


			tnl::Vector3 everyDirBullet_moveDirection =
				ScenePlay::_bullet_silentSerena_patchouli[i]->_moveDirection;


			ScenePlay::_bullet_silentSerena_patchouli[i]->_timer += delta_time;


			if (!ScenePlay::_bullet_silentSerena_patchouli[i]->_isActive) {

				if ((int)ScenePlay::_bullet_silentSerena_patchouli[i]->_timer % 5 == 0) {

					// ’e‚ð‰~Žüã‚É“™ŠÔŠu‚Å”z’u‚·‚é
					float angle = (2.0f * tnl::PI / 80) * (shotNum % 80);

					spawn_pos.x += cos(angle) + rnd_val(mt);
					spawn_pos.y += sin(angle) + rnd_val(mt);
					spawn_pos.z += sin(angle) + rnd_valZ(mt);

					everyDirBullet_moveDirection.z = sin(angle) * radius + rnd_val(mt);
					everyDirBullet_moveDirection.x = cos(angle) * radius + rnd_val(mt);
					everyDirBullet_moveDirection.y = sin(angle) * radius + rnd_valZ(mt);

					everyDirBullet_moveDirection.normalize();

					ScenePlay::_bullet_silentSerena_patchouli[i]->_mesh->pos_ = spawn_pos;

					ScenePlay::_bullet_silentSerena_patchouli[i]->_moveDirection = everyDirBullet_moveDirection;

					ScenePlay::_bullet_silentSerena_patchouli[i]->_isActive = true;

					ScenePlay::_bullet_silentSerena_patchouli[i]->_timer = 0;

					shotNum = (shotNum + 1) % 80;
				}
			}


			ScenePlay::_bullet_silentSerena_patchouli[i]->_mesh->pos_ +=
				ScenePlay::_bullet_silentSerena_patchouli[i]->_moveDirection * delta_time * 200;
		}

		if (ScenePlay::_bullet_silentSerena_patchouli[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue) {

			tnl::Vector3 spawn_pos = ScenePlay::_bullet_silentSerena_patchouli[i]->_mesh->pos_;

			CheckLifeTimeDistance(ScenePlay::_bullet_silentSerena_patchouli[i], spawn_pos, 600);

			std::uniform_real_distribution<float> dist_x(-300, 300);
			std::uniform_real_distribution<float> dist_y(-50.0, 100.0);
			std::uniform_real_distribution<float> dist_z(-300, 300);

			tnl::Vector3 move_dir = ScenePlay::_bullet_silentSerena_patchouli[i]->_moveDirection;

			if (!ScenePlay::_bullet_silentSerena_patchouli[i]->_isActive) {

				spawn_pos.x = bossPosition.x + dist_x(mt);
				spawn_pos.y = bossPosition.y + dist_y(mt);
				spawn_pos.z = bossPosition.z + dist_z(mt);

				move_dir = _player_ref->GetPos() - spawn_pos;

				move_dir.normalize();

				ScenePlay::_bullet_silentSerena_patchouli[i]->_mesh->pos_ = spawn_pos;
				ScenePlay::_bullet_silentSerena_patchouli[i]->_moveDirection = move_dir;

				ScenePlay::_bullet_silentSerena_patchouli[i]->_isActive = true;
			}

			ScenePlay::_bullet_silentSerena_patchouli[i]->_mesh->pos_ +=
				move_dir * delta_time * 250.f;
		}
	}
}



void BulletHell::ShotBulletHell_Normal_Cirno(const float& delta_time) {

	for (int i = 0; i < ScenePlay::_bullet_normal_cirno.size(); i++) {

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// ƒVƒ‡ƒbƒgƒKƒ“i3 + N)s (N‚Í0`5‚Ü‚Å)A‚U—ñ‚Ì’e‚ðƒvƒŒƒCƒ„[‚Ö”­ŽË (99”­j
		if (ScenePlay::_bullet_normal_cirno[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

			float radius = 100.0f;

			tnl::Vector3 spawn_pos = ScenePlay::_bullet_normal_cirno[i]->_mesh->pos_;

			CheckLifeTimeDistance(ScenePlay::_bullet_normal_cirno[i], spawn_pos, 400);

			ScenePlay::_bullet_normal_cirno[i]->_timer += delta_time;

			if (!ScenePlay::_bullet_normal_cirno[i]->_isActive &&
				(int)ScenePlay::_bullet_normal_cirno[i]->_timer % 10 == 0) {

				tnl::Vector3 move_dir = (_player_ref->GetPos() - bossPosition) * radius;
				move_dir.normalize();

				spawn_pos.x = _bossMesh_ref->rot_.x;
				spawn_pos.y = _bossMesh_ref->rot_.y;
				spawn_pos.z = _bossMesh_ref->rot_.z;
				spawn_pos.z += 25.f;

				ScenePlay::_bullet_normal_cirno[i]->_mesh->pos_ = spawn_pos;

				ScenePlay::_bullet_normal_cirno[i]->_moveDirection = move_dir;

				ScenePlay::_bullet_normal_cirno[i]->_isActive = true;
				ScenePlay::_bullet_normal_cirno[i]->_timer = 0;
			}

			ScenePlay::_bullet_normal_cirno[i]->_mesh->pos_ +=
				ScenePlay::_bullet_normal_cirno[i]->_moveDirection * delta_time * 200;
		}
		if (ScenePlay::_bullet_normal_cirno[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {


			float radius = 100.0f;
			float angle = (2.0f * tnl::PI / 16) * (ScenePlay::_bullet_normal_cirno[i]->_id % 16);

			float offset = 0.5f;


			tnl::Vector3 spawn_pos = ScenePlay::_bullet_normal_cirno[i]->_mesh->pos_;

			CheckLifeTimeDistance(ScenePlay::_bullet_normal_cirno[i], spawn_pos, 250);

			ScenePlay::_bullet_normal_cirno[i]->_timer += delta_time;

			if (!ScenePlay::_bullet_normal_cirno[i]->_isActive &&
				(int)ScenePlay::_bullet_normal_cirno[i]->_timer % 5 == 0) {

				tnl::Vector3 move_dir = ScenePlay::_bullet_normal_cirno[i]->_moveDirection;

				spawn_pos.x = (bossPosition.x + radius) * cos(angle);
				spawn_pos.y = bossPosition.y;
				spawn_pos.z = (bossPosition.z + radius) * sin(angle);

				move_dir.x = spawn_pos.x + cos(angle) * radius;
				move_dir.z = spawn_pos.z + sin(angle) * radius;

				move_dir.normalize();

				ScenePlay::_bullet_normal_cirno[i]->_mesh->pos_ = spawn_pos;

				ScenePlay::_bullet_normal_cirno[i]->_moveDirection = move_dir;


				ScenePlay::_bullet_normal_cirno[i]->_isActive = true;
				ScenePlay::_bullet_normal_cirno[i]->_timer = 0;
			}

			ScenePlay::_bullet_normal_cirno[i]->_mesh->pos_ +=
				ScenePlay::_bullet_normal_cirno[i]->_moveDirection * delta_time * 200;
		}
	}
}



void BulletHell::ShotBulletHell_IcicleFall_Cirno(const float& delta_time) {

	for (int i = 0; i < ScenePlay::_bullet_icicleFall_cirno.size(); i++) {

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;


		if (ScenePlay::_bullet_icicleFall_cirno[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

			tnl::Vector3 spawn_pos = ScenePlay::_bullet_icicleFall_cirno[i]->_mesh->pos_;

			CheckLifeTimeDistance(ScenePlay::_bullet_icicleFall_cirno[i], spawn_pos, 250);

			if (!ScenePlay::_bullet_icicleFall_cirno[i]->_isActive) {

				float radius = 300 + (i % 5) * 15;
				float angle = (tnl::PI / 20) * (ScenePlay::_bullet_icicleFall_cirno[i]->_id % 20);
				tnl::Vector3 move_dir;

				spawn_pos.x = _bossMesh_ref->rot_.x * cos(angle);
				spawn_pos.y = _bossMesh_ref->rot_.y;
				spawn_pos.z = _bossMesh_ref->rot_.z * sin(angle) - 15;


				move_dir.x = cos(angle) * radius;
				move_dir.normalize();

				ScenePlay::_bullet_icicleFall_cirno[i]->_mesh->pos_ = spawn_pos;
				ScenePlay::_bullet_icicleFall_cirno[i]->_moveDirection = move_dir;
			}

			ScenePlay::_bullet_icicleFall_cirno[i]->_mesh->pos_ +=
				ScenePlay::_bullet_icicleFall_cirno[i]->_moveDirection * delta_time * 200;
		}
		if (ScenePlay::_bullet_icicleFall_cirno[i]->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow) {

		}
	}
}


void BulletHell::ShotBulletHell_PerfectFreeze_Cirno(const float& delta_time) {

}


void BulletHell::ShotBulletHell_Normal_Suwako(const float& delta_time) {


}


void BulletHell::ShotBulletHell_IronRingOfMoriya_Suwako(const float& delta_time) {


}


void BulletHell::ShotBulletHell_KeroChanStandsFirm_AgainstTheStorm_Suwako(const float& delta_time) {

}


void BulletHell::CheckLifeTimeDistance(Shared<EnemyBullet>& bullet, const tnl::Vector3& pos, const float limit_distance)
{
	static tnl::Vector3 start = pos;

	float dx = bullet->_mesh->pos_.x - start.x;
	float dy = bullet->_mesh->pos_.y - start.y;
	float dz = bullet->_mesh->pos_.z - start.z;

	float current_distance = sqrt(dx * dx + dy * dy + dz * dz);

	if (abs(current_distance) > limit_distance)
		bullet->_isActive = false;
}

const float& BulletHell::GetCurrentBulletMovedDistance(const Shared<EnemyBullet>& bullet) {

	tnl::Vector3 start = bullet->_mesh->pos_;

	float dx = bullet->_mesh->pos_.x - start.x;
	float dy = bullet->_mesh->pos_.y - start.y;
	float dz = bullet->_mesh->pos_.z - start.z;

	float current_distance = sqrt(dx * dx + dy * dy + dz * dz);

	return current_distance;
}