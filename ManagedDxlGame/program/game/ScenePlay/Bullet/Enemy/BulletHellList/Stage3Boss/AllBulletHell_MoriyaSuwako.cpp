#include "../../BulletHell.h"
#include "../../BulletHellsBulletNumInfo.h"
#include "../../EnemyBullet.h"
#include "../../../../Character/Enemy/EnemyBoss/EnemyBoss_MoriyaSuwako.h"
#include "../../../../RandomValue/RandomValueGenerator.h"


/*
　　　　・関数の宣言は　BulletHell.h　にあります。
  　　  ・ここではステージ３ボス、「洩矢諏訪子」の弾幕関数定義をしています。
*/


namespace inl {

	// 通常攻撃-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_Normal_Suwako(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_normal_suwako.size(); i++) {

			auto bullet = EnemyBoss_MoriyaSuwako::_bullet_normal_suwako[i];
			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			const float RADIUS = 100.0f;         // 半径
			const float BULLET_SPEED = 500.0f;   // 速さ
			const int BULLETNUM_EACH_WAVE = 16;  // 1回の発射で使用する弾の数


			// 赤。 全方位 １回につき６４発生成、８方向へ無限に飛ばし続ける-------------------------------
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {

				//　初期化
				if (bullet->_timer == 0.f) {

					float angle = (2.0f * tnl::PI / BULLETNUM_EACH_WAVE) * (bullet->_id);

					//　生成位置
					tnl::Vector3 spawnPos = bossPosition;
					spawnPos.x += cos(angle) * bullet->_id;
					spawnPos.z += sin(angle) * bullet->_id;

					//　移動方向
					tnl::Vector3 moveDirection;
					moveDirection.x += -sin(angle) * RADIUS;
					moveDirection.z += cos(angle) * RADIUS;

					moveDirection.normalize();

					bullet->_mesh->pos_ = spawnPos;
					bullet->_moveDirection = moveDirection;
				}

				//　寿命
				float lifeTime = 0.f;

				//　タイマー起動
				bullet->_timer += deltaTime;

				// 弾のIDごとにグループ分けを行い、グループごとに更新する
				int wave = bullet->_id / 56;

				//　第１波---------------------------------------
				if (wave == 0)
				{
					if (bullet->_timer > 0.f)
						ResetAssist_Normal_Suwako(bullet, lifeTime);
				}
				//　第２波---------------------------------------
				if (wave == 1)
				{
					if (bullet->_timer > 1.f) {
						lifeTime = 1.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第３波---------------------------------------
				if (wave == 2)
				{
					if (bullet->_timer > 2.f) {

						lifeTime = 2.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第４波---------------------------------------
				if (wave == 3)
				{
					if (bullet->_timer > 3.f) {

						lifeTime = 3.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第５波---------------------------------------
				if (wave == 4)
				{
					if (bullet->_timer > 4.f) {

						lifeTime = 4.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}

				// 更新
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
			}

			// 青。 全方位 １回につき６４発生成、８方向へ無限に飛ばし続ける--------------------------------
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {

				// 初期化
				if (bullet->_timer == 0.f) {

					float angle = (2.0f * tnl::PI / BULLETNUM_EACH_WAVE) * (bullet->_id);

					//　生成位置
					tnl::Vector3 spawnPos = bossPosition;
					spawnPos.x -= cos(angle) * bullet->_id;
					spawnPos.z -= sin(angle) * bullet->_id;

					//　移動方向
					tnl::Vector3 moveDirection;
					moveDirection.x -= -sin(angle) * RADIUS;
					moveDirection.z -= cos(angle) * RADIUS;

					moveDirection.normalize();

					bullet->_mesh->pos_ = spawnPos;
					bullet->_moveDirection = moveDirection;
				}

				// タイマー起動
				bullet->_timer += deltaTime;

				// 弾の寿命
				float lifeTime = 0.f;

				// 弾のIDごとにグループ分けを行い、グループごとに更新する
				int wave = bullet->_id / 56;

				//　第１波---------------------------------------
				if (wave == 0)
				{
					if (bullet->_timer > 0.f) {

						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第２波---------------------------------------
				if (wave == 1)
				{
					if (bullet->_timer > 1.f) {

						lifeTime = 1.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第３波---------------------------------------
				if (wave == 2)
				{
					if (bullet->_timer > 2.f) {

						lifeTime = 2.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第４波---------------------------------------
				if (wave == 3)
				{
					if (bullet->_timer > 3.f) {

						lifeTime = 3.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}
				//　第５波---------------------------------------
				if (wave == 4)
				{
					if (bullet->_timer > 4.f) {

						lifeTime = 4.f;
						ResetAssist_Normal_Suwako(bullet, lifeTime);
					}
				}

				//　更新
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
			}
		}
	}


	//　通常攻撃パターン（リセット処理）------------------------------------------------------------
	void BulletHell::ResetAssist_Normal_Suwako(Shared<EnemyBullet>& bullet, const float timeLimit)
	{
		bullet->_isActive = true;

		if (bullet->_timer > timeLimit + 1.f) {

			bullet->_isActive = false;
			bullet->_timer = 0;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}


	// 特殊攻撃１--------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_IronRingOfMoriya_Suwako(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako.size(); i++) {

			auto bullet = EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako[i];

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			//　円状の配置を保ったままプレイヤーを追いかける弾
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red) {

				//　弾は内側に４５発、外側に４５発、計９０発で1つの塊とする

				const int BULLETCOUNT_EACHCIRCLE = 45;  // 1照射45発
				const float CIRCLE1_RADIUS = 70.0f;     // 内側の円
				const float CIRCLE2_RADIUS = 100.0f;    // 外側の円
				const float BULLET_SPEED = 150.f;       // 弾の速さ

				float angle = (2.0f * tnl::PI / BULLETCOUNT_EACHCIRCLE) * bullet->_id;

				// 弾を9個のグループに分ける
				int wave = bullet->_id / 90;

				//　第１波---------------------------------------
				if (wave == 0)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第２波---------------------------------------
				if (wave == 1)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第３波---------------------------------------
				if (wave == 2)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第４波---------------------------------------
				if (wave == 3)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第５波---------------------------------------
				if (wave == 4)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第６波---------------------------------------
				if (wave == 5)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第７波---------------------------------------
				if (wave == 6)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第８波---------------------------------------
				if (wave == 7)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第９波---------------------------------------
				if (wave == 8)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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
				//　第１０波---------------------------------------
				if (wave == 9)
				{
					// 内側----------------------------------------------------------
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
					// 外側----------------------------------------------------------
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

		// 初期化
		if (bullet->_timer == 0.f) {

			spawnPos.x += circleRadius * cos(angle);
			spawnPos.z += circleRadius * sin(angle);
			bullet->_mesh->pos_ = spawnPos;
		}

		// タイマー起動
		bullet->_timer += deltaTime;

		//　移動開始
		if (bullet->_timer > startMoveTime) {

			bullet->_isActive = true;

			// プレイヤー方向取得
			targetDir = _player_ref->GetPos() - spawnPos;
			targetDir.normalize();

			//　プレイヤーを追いかける（　線形補間　）
			bullet->_moveDirection = tnl::Vector3::UniformLerp(
				bullet->_mesh->pos_,
				targetDir * 0.8f,
				1.5f,
				bullet->_timer
			);

			//　更新
			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
		}
		//　リセット
		if (bullet->_timer > startMoveTime + 10.f) {

			bullet->_isActive = false;
			bullet->_timer = 0.f;
		}
	}


	void BulletHell::ShotBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako.size(); i++) {

			auto bullet = EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako[i];

			// 上へ放ってから下へ降ろす
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

				// 初期化
				if (bullet->_timer == 0.f) {
					InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(deltaTime, 50.f, bullet, true);
				}

				//　更新
				bullet->_timer += deltaTime;
				UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(bullet, deltaTime);
			}

			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_EmeraldGreen) {

				// 初期化
				if (bullet->_timer == 0.f) {
					InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(deltaTime, 30.f, bullet, false);
				}

				//　更新
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
		// 生成位置
		tnl::Vector3 spawnPos = _bossMesh_ref->pos_;
		//　ランダムベクター生成
		tnl::Vector3 randomVector = RandomValueGenerator::Vector(-500, 500, 50, 150, -200, 0);

		// 移動方向
		tnl::Vector3 moveDir;
		moveDir += spawnPos + randomVector;
		moveDir.y += upwardPower;          // 前方向へ力を加える
		moveDir.normalize();

		bullet->_moveDirection = moveDir;

		if (isOscillate) {

			const float OSCILLATE_RATE{ 1.3f };
			float bloom = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0, OSCILLATE_RATE, deltaTime);

			// 生成位置を左右に動かしたい
			bullet->_mesh->pos_ = spawnPos + tnl::Vector3(bloom * 3, 50, 0);
		}
		else {

			//　弾生成位置をボスの頭上に設定
			bullet->_mesh->pos_ = spawnPos + tnl::Vector3(0, 50, 0);
		}
	}


	void BulletHell::UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
		Shared<EnemyBullet>& bullet,
		const float deltaTime)
	{
		const float GRAVITY_ACCELARATION = -9.8f;  // 重力加速度
		const float BULLET_SPEED = 200.0f;         // 弾のスピード
		const float LIFE_TIME = 6.0f;              // 弾の寿命
		const float OFFSET = 0.1f;                 // 発射間隔遅延用係数
		const int BULLET_NUM_EACH_GROUP = 10;      // １回ごとに撃つ弾の数

		// 発射タイミング
		float startTiming = OFFSET * (bullet->_id / BULLET_NUM_EACH_GROUP);

		//　更新開始
		if (bullet->_timer >= startTiming) {

			bullet->_isActive = true;

			tnl::Vector3 gravity = { 0.f, GRAVITY_ACCELARATION * deltaTime, 0.f };

			bullet->_moveDirection += gravity * deltaTime;
			bullet->_moveDirection.normalize();

			//　更新
			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}

		//　リセット
		if (bullet->_timer > startTiming + LIFE_TIME) {

			bullet->_isActive = false;

			bullet->_timer = 0.f;

			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}
}