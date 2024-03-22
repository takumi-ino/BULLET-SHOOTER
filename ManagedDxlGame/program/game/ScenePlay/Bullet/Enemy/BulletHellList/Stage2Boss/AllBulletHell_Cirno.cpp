#include "../../BulletHell.h"
#include "../../BulletHellsBulletNumInfo.h"
#include "../../EnemyBullet.h"
#include "../../../../Character/Enemy/EnemyBoss/EnemyBoss_Cirno.h"
#include "AllBulletHell_Cirno.h"
#include "../../../../RandomValue/RandomValueGenerator.h"

/*
　　　　・関数の宣言は　BulletHell.h　にあります。
  　　  ・ここではステージ２ボス、「チルノ」の弾幕関数定義をしています。
*/


namespace inl {

	//　通常攻撃-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_Normal_Cirno(const float deltaTime) {

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		for (int i = 0; i < EnemyBoss_Cirno::_bullet_normal_cirno.size(); i++) {

			auto bullet = EnemyBoss_Cirno::_bullet_normal_cirno[i];

			//　パターン1　ショットガン（2 + N)行 (Nは0～5まで)、６列の弾をプレイヤーへ発射 (81発）
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Straight_Blue) {

				const int BULLET_WAVE_COUNT = 3;
				const int BULLET_NUM_EACH_COLUMN = 8;
				const float OFFSET = 0.1f;

				// 発射間隔をあけるための波をつくる
				int wave = bullet->_id / (STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO / BULLET_WAVE_COUNT);

				// タイマー起動
				bullet->_timer += deltaTime;

				//　各波の発射間隔を空けるオフセット
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

			// パターン２　全方位弾 １回につき16発
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {

				const float RADIUS = 100.0f;         // 半径
				const float LIFETIME_LIMIT = 2.0f;   // 寿命
				const float BULLET_SPEED = 500.0f;   // 速さ

				const int BULLETNUM_EACH_WAVE = 16;  // １度に撃つ弾の数
				const float OFFSET = 0.5f;           // 発射間隔調整係数

				//　発射間隔
				float startTiming = OFFSET * (bullet->_id / BULLETNUM_EACH_WAVE);

				//　角度
				float angle = (2.0f * tnl::PI / BULLETNUM_EACH_WAVE) * (bullet->_id % BULLETNUM_EACH_WAVE);

				//　初期化
				if (bullet->_timer == 0.f) {

					tnl::Vector3 moveDir, spawnPos;

					//　生成位置
					spawnPos.x = bossPosition.x + RADIUS * cos(angle);
					spawnPos.y = bossPosition.y;
					spawnPos.z = bossPosition.z + RADIUS * sin(angle);

					//　方向
					moveDir.x = cos(angle) * RADIUS;
					moveDir.z = sin(angle) * RADIUS;

					moveDir.normalize();

					bullet->_mesh->pos_ = spawnPos;

					bullet->_moveDirection = moveDir;

					bullet->_isActive = true;
				}

				// タイマー起動
				bullet->_timer += deltaTime;

				//　リセット
				if (bullet->_isActive &&
					bullet->_timer >= startTiming &&
					bullet->_timer > LIFETIME_LIMIT)
				{

					bullet->_isActive = false;
					bullet->_timer = 0;

					//　位置
					bullet->_mesh->pos_.x = bossPosition.x + RADIUS * cos(angle);
					bullet->_mesh->pos_.y = bossPosition.y;
					bullet->_mesh->pos_.z = bossPosition.z + RADIUS * sin(angle);

					tnl::Vector3 moveDir;

					//　方向
					moveDir.x = cos(angle) * RADIUS;
					moveDir.z = sin(angle) * RADIUS;
					moveDir.normalize();

					bullet->_moveDirection = moveDir;
				}

				// 更新
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
			}
		}
	}

	//　通常攻撃アシスト関数--------------------------------------------------------------------------------------------------------------
	void BulletHell::WaveAssist_Normal_Cirno(
		Shared<EnemyBullet>& bullet,
		const float timing,
		const float delayOffset,
		const float deltaTime)
	{
		const float WIDTH = 70.0f;         // 弾１行ｎ列の列の最大幅
		const float DEFAULT_LIMIT = 4.0f;  // 弾のリセット基準タイミング
		const float BULLET_SPEED = 500.f;  // 弾の速さ

		//　弾ごとに時差更新
		if (bullet->_timer >= timing + delayOffset) {

			bullet->_isActive = true;

			tnl::Vector3 spawn_pos = _bossMesh_ref->pos_;             // 生成位置
			tnl::Vector3 moveDir = _player_ref->GetPos() - spawn_pos; // 方向
			float angle = (tnl::PI / 8) * (bullet->_id % 8);          // 角度

			moveDir.x += cos(angle) * WIDTH;

			moveDir.normalize();

			bullet->_moveDirection = moveDir;

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}

		//　リセット
		if (bullet->_timer > DEFAULT_LIMIT + delayOffset) {

			bullet->_isActive = false;
			bullet->_timer = 0.f;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}


	//　特殊攻撃１-----------------------------------------------------------------------------------------------------------------

	float assistTimer_icicleFall_straightBlue_cirno = 0.f;
	float assistTimer_icicleFall_straightYellow_cirno = 0.f;

	void BulletHell::ShotBulletHell_IcicleFall_Cirno(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_Cirno::_bullet_icicleFall_cirno.size(); i++) {
			auto bullet = EnemyBoss_Cirno::_bullet_icicleFall_cirno[i];

			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			float angleToPlayer =    // プレイヤーとの角度
				atan2(_player_ref->GetPos().z - bossPosition.z, 
					_player_ref->GetPos().x - bossPosition.x
				);


			// 	パターン1　左右に発射後、内側へ方向転換する弾
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

				const float LIFETIME_LIMIT = 2.0f;
				const float ANGLE_OFFSET = 45.f;

				const float BULLET_SPEED = 20.f;
				const float DELAY_OFFSET = 15.f;
				const int BULLET_NUM_EACHLINE = 10;

				float angleOffset =    // 弾を左と右で10個ずつグループに分けて更新
					(bullet->_id >= 10 && bullet->_id < 20) ||    // 0～20
					(bullet->_id >= 30 && bullet->_id < 40) ||	  // 20～40
					(bullet->_id >= 50 && bullet->_id < 60) ||	  // 40～60
					(bullet->_id >= 70 && bullet->_id < 89) ||	  // 60～80
					(bullet->_id >= 100 && bullet->_id < 109) ||  // 80～100
					(bullet->_id >= 110 && bullet->_id < 119) ?	  // 100～120
					tnl::ToRadian(angleToPlayer - ANGLE_OFFSET) :
					tnl::ToRadian(angleToPlayer + ANGLE_OFFSET);

				//　タイマー起動
				assistTimer_icicleFall_straightBlue_cirno += deltaTime;


				// プレイヤー方向かつ左右のオフセットを加えた角度を取得
				float newAngle = angleToPlayer + angleOffset;


				//　第１波
				if (assistTimer_icicleFall_straightBlue_cirno > 0.f) {

					// 0 ～ 19 の弾-------------------------------------------------------------------------
					if (bullet->_id < 20) {

						// 角度
						float radius =
							(2.f * tnl::PI) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							BULLET_SPEED;

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						bullet->_timer += deltaTime;

						//　更新
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 1.0f, deltaTime);
					}
				}

				const float RADIUS_OFFSET = 0.5f;

				//　第２波
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET) {

					// 20 ～ 39 の弾---------------------------------------------------------------------------------
					if (bullet->_id >= 20 && bullet->_id < 40) {

						// 角度
						float radius =
							(2.f * tnl::PI / RADIUS_OFFSET) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 3.f);

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 1.5f, deltaTime);
					}
				}

				//　第３波
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 2.f) {

					// 40 ～ 59 の弾----------------------------------------------------------------------------------
					if (bullet->_id >= 40 && bullet->_id < 60) {

						// 角度
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 2.f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 5.f);

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 2.0f, deltaTime);
					}
				}

				//　第４波
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 3.f) {

					// 60 ～ 79 の弾-----------------------------------------------------------------------------------
					if (bullet->_id >= 60 && bullet->_id < 80) {

						// 角度
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 2.5f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 7.5f);

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 2.5f, deltaTime);
					}
				}

				//　第５波
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 4.f) {

					// 80 ～ 99 の弾----------------------------------------------------------------------------------
					if (bullet->_id >= 90 && bullet->_id < 109) {

						// 角度
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 3.0f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 10.f);

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 3.0f, deltaTime);
					}
				}

				//　第６波
				if (assistTimer_icicleFall_straightBlue_cirno > DELAY_OFFSET * 5.f) {

					// 100 ～ 119 の弾---------------------------------------------------------------------------------
					if (bullet->_id >= 100 && bullet->_id < 119) {

						// 角度
						float radius =
							(2.f * tnl::PI / (RADIUS_OFFSET * 3.5f)) *
							(bullet->_id % BULLET_NUM_EACHLINE) *
							(BULLET_SPEED - 12.5f);

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightBlue_Cirno(bullet, radius, newAngle, 3.5f, deltaTime);
					}
				}
			}

			// 	パターン2　１行５列の弾をプレイヤーへ照射（３セット）
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow) {

				const float BULLET_SPEED = 400.0f;   //　弾の速さ
				const float FIRE_INTERVAL = 5.0f;    //　発射間隔
				const float DELAY_OFFSET = 10.f;     //　遅延処理用係数


				//　波を管理するタイマー
				assistTimer_icicleFall_straightYellow_cirno += deltaTime;

				//　第１波---------------------------------------------------------
				if (assistTimer_icicleFall_straightYellow_cirno > 0.f) {

					// id 0～5の弾---------------------------------------------------------------------
					if (bullet->_id < 5) {

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightYellow_Cirno(bullet);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
					}
				}

				//　第２波---------------------------------------------------------
				if (assistTimer_icicleFall_straightYellow_cirno > DELAY_OFFSET) {

					// id 5～10の弾---------------------------------------------------------------------
					if (bullet->_id >= 5 && bullet->_id < 10) {

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightYellow_Cirno(bullet);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
					}
				}

				//　第３波---------------------------------------------------------
				if (assistTimer_icicleFall_straightYellow_cirno > DELAY_OFFSET * 2.f) {

					// id 10～15の弾---------------------------------------------------------------------
					if (bullet->_id >= 10 && bullet->_id < 15) {

						// 初期化
						if (bullet->_timer == 0.f) {
							InitAssist_IcicleFall_StraightYellow_Cirno(bullet);
						}

						//　更新
						bullet->_timer += deltaTime;
						UpdateAssist_IcicleFall_StraightYellow_Cirno(bullet, deltaTime, BULLET_SPEED);
					}
				}
			}
		}
	}


	//　特殊攻撃１アシスト（初期化、青い弾）------------------------------------------------------------------------
	void BulletHell::InitAssist_IcicleFall_StraightBlue_Cirno(
		Shared<EnemyBullet>& bullet,
		const float radius,
		const float newAngle)
	{
		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// ボスの正面座標取得　xz平面
		tnl::Vector3 bossForward = { cos(_bossMesh_ref->rot_.y), 0, sin(_bossMesh_ref->rot_.y) };

		tnl::Vector3 moveDir, spawnPos;

		spawnPos = bossPosition + bossForward * radius * cos(newAngle);
		spawnPos.x += radius * cos(newAngle);
		spawnPos.z += radius * sin(newAngle);

		moveDir += tnl::Vector3{ cos(newAngle), 0 , sin(newAngle) };
		moveDir.normalize();

		bullet->_mesh->pos_ = spawnPos;    // 位置

		bullet->_moveDirection = moveDir;  // 方向

		bullet->_isActive = true;          // アクティブ状態
	}


	//　特殊攻撃１アシスト（更新、青い弾）------------------------------------------------------------------------
	void BulletHell::UpdateAssist_IcicleFall_StraightBlue_Cirno(
		Shared<EnemyBullet>& bullet,
		const float radius,
		const float newAngle,
		const float startChangeDirTime,
		const float deltaTime)
	{
		const float BULLET_SPEED = 200.0f;   //　弾の速さ
		const float BULLET_LIFETIME = 3.0f;  //　寿命

		if (bullet->_timer > startChangeDirTime &&
			bullet->_timer < startChangeDirTime + 0.1f)
		{
			//　プレイヤー方向へ軌道修正
			tnl::Vector3 changeDir = _player_ref->GetPos() - bullet->_mesh->pos_;
			changeDir += tnl::Vector3{ radius * cos(newAngle), 0 , radius * sin(newAngle) };

			changeDir.normalize();

			bullet->_moveDirection = changeDir;
			bullet->_moveDirection.normalize();
		}

		//　リセット
		if (bullet->_timer > startChangeDirTime + BULLET_LIFETIME)
		{
			bullet->_isActive = false;
			bullet->_timer = 0;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}

		//　更新
		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
	}


	//　特殊攻撃１アシスト（初期化、黄色い弾）------------------------------------------------------------------------
	void BulletHell::InitAssist_IcicleFall_StraightYellow_Cirno(Shared<EnemyBullet>& bullet)
	{
		const int BULLETS_PER_ROW = 5;
		const float BULLET_SPACING = 50.0f;  //　弾どうしのの間隔

		// プレイヤー方向取得
		tnl::Vector3 direction = _player_ref->GetPos() - _bossMesh_ref->pos_;
		direction.normalize();
		bullet->_moveDirection = direction;

		// 中央以外の弾が左右にずれるように調整
		int column = bullet->_id % BULLETS_PER_ROW;
		float offset = (column - (BULLETS_PER_ROW / 2)) * BULLET_SPACING;

		// ボスの向いている方向に合わせる
		tnl::Vector3 sideOffset = tnl::Vector3::TransformCoord({ offset,0,0 }, _bossMesh_ref->rot_);

		bullet->_mesh->pos_ = _bossMesh_ref->pos_ + sideOffset;
		bullet->_isActive = true;
	}


	//　特殊攻撃１アシスト（更新、黄色い弾）------------------------------------------------------------------------
	void BulletHell::UpdateAssist_IcicleFall_StraightYellow_Cirno(
		Shared<EnemyBullet>& bullet,
		const float deltaTime,
		const float bulletSpeed)
	{
		const float BULLET_LIFETIME = 4.0f;

		//　更新
		if (bullet->_isActive) {

			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * bulletSpeed;
		}

		//　リセット
		if (bullet->_timer > BULLET_LIFETIME) {

			bullet->_isActive = false;
			bullet->_timer = 0.f;
			bullet->_mesh->pos_ = _bossMesh_ref->pos_;
		}
	}


	//　特殊攻撃２--------------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_PerfectFreeze_Cirno(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_Cirno::_bullet_perfectFreeze_cirno.size(); i++) {

			auto bullet = EnemyBoss_Cirno::_bullet_perfectFreeze_cirno[i];
			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			//　パターン１　　スフィア、円周状、白　　ボスの周囲にランダムに弾を生成
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_White) {

				const float RADIUS = 150.0f;                      //　半径
				const float BULLET_SPEED = 200.0f;                //　速さ
				const float BULLET_LIFETIME = 8.0f;               //　寿命

				float theta = (2.0f * tnl::PI) * (bullet->_id);   // 方位角
				float phi = tnl::PI * bullet->_id;                // 天頂角

				//　初期化
				if (bullet->_timer == 0.f) {

					tnl::Vector3 spawnPos = bossPosition;

					//　生成位置
					spawnPos.x += sin(phi) * cos(theta);
					spawnPos.y += sin(phi) * sin(theta);
					spawnPos.z += sin(theta);
					spawnPos += RandomValueGenerator::Vector(-250, 50, -250, 50, -150, 150);

					tnl::Vector3 moveDirection = _player_ref->GetPos() - bullet->_mesh->pos_;

					//　方向
					moveDirection.x = cos(theta) * RADIUS;
					moveDirection.y = sin(theta) * RADIUS;
					moveDirection.z = sin(theta) * RADIUS;
					moveDirection += RandomValueGenerator::Vector(-250, 50, -150, 150, -250, 50);
					moveDirection.normalize();

					bullet->_moveDirection = moveDirection;
					bullet->_mesh->pos_ = spawnPos;
					bullet->_isActive = true;
				}

				//　停止時間
				const float STOPPING_DURATION = 2.f;

				//　タイマー起動
				bullet->_timer += deltaTime;

				if (bullet->_timer < STOPPING_DURATION) {

					//　更新
					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * 0.1f;
				}
				else if (bullet->_timer > STOPPING_DURATION && bullet->_timer < STOPPING_DURATION * 2.f) {

					// 弾を一時停止
					bullet->_moveDirection = tnl::Vector3(0, 0, 0);
				}
				//　移動再開
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

				// 更新
				bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;

				// リセット
				if (bullet->_timer > BULLET_LIFETIME) {
					bullet->_isActive = false;

					bullet->_timer = 0.f;
				}
			}

			//　パターン２　　スフィア、直行弾、青　　プレイヤーへ１行８列の弾を連射
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue) {

				tnl::Vector3 spawnPos = bossPosition;
				const int BULLET_NUM_EACH_LINE = 8;

				// 初期化
				if (bullet->_timer == 0.f) {

					//　角度
					float theta = (tnl::PI / BULLET_NUM_EACH_LINE) * (bullet->_id % BULLET_NUM_EACH_LINE);

					//　方向
					tnl::Vector3 moveDir = _player_ref->GetPos() - spawnPos;
					moveDir.x += cos(theta) * 70;
					moveDir.normalize();

					bullet->_moveDirection = moveDir;

					bullet->_mesh->pos_ = spawnPos;
				}

				//　タイマー起動
				bullet->_timer += deltaTime;

				const float LIFE_TIME = 4.f;
				const float START_OFFSET = 0.1f;
				const float BULLET_SPEED = 500.f;

				float startTiming = START_OFFSET * (bullet->_id / BULLET_NUM_EACH_LINE);

				// 更新開始
				if (bullet->_timer >= startTiming) {

					bullet->_isActive = true;
					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
				}

				//　リセット
				if (bullet->_timer > LIFE_TIME) {

					bullet->_isActive = false;
					bullet->_timer = 0.f;
					bullet->_mesh->pos_ = spawnPos;
				}
			}
		}
	}
}