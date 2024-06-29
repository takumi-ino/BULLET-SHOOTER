#include "../../BulletHell.h"
#include "../../BulletHellsBulletNumInfo.h"
#include "../../EnemyBullet.h"
#include "../../../../Character/Enemy/EnemyBoss/EnemyBoss_PatchouliKnowledge.h"
#include "../../../../RandomValue/RandomValueGenerator.h"


/*
　　　　・関数の宣言は　BulletHell.h　にあります。
  　　  ・ここではステージ1ボス、「パチュリー・ノーレッジ」の弾幕関数定義をしています。
*/


namespace inl {

	//　通常攻撃-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_Normal_Patchouli(const float deltaTime) {

		tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

		// それぞれのタイプの弾幕を別々に処理するため、同じベクターから要素のグループ分けをする
		static std::vector<Shared<EnemyBullet>> sphereRoundBlue, cylinderRoundBlue, sphereRoundRed;

		if (sphereRoundBlue.empty() && cylinderRoundBlue.empty() && sphereRoundRed.empty()) {

			for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli.size(); i++) {
				auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli[i];

				// スフィア、回転、青
				if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue) {
					sphereRoundBlue.push_back(bullet);
				}
				// シリンダー、回転、青
				else if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue) {
					cylinderRoundBlue.push_back(bullet);
				}
				// スフィア、回転、赤
				else {
					sphereRoundRed.push_back(bullet);
				}
			}
		}

		const float ROUNDBULLET_RADIUS = 90.0f;
		const float ROUNDBULLET_MOVESPEED = 100.0f;
		tnl::Vector3 roundBullet_moveDirection;

		// パターン１　ボスの周囲を位置回転する青い弾
		for (auto& bullet : sphereRoundBlue) {

			// 左回転
			if (bullet->_id % 2 == 0) {
				bullet->_angle += deltaTime;
			}

			// 右回転
			if (bullet->_id % 2 != 0) {
				bullet->_angle -= deltaTime;
			}

			// X
			bullet->_mesh->pos_.x = bossPosition.x + ROUNDBULLET_RADIUS * cos(bullet->_angle);
			// Y
			bullet->_mesh->pos_.y = bossPosition.y;
			// Z
			bullet->_mesh->pos_.z = bossPosition.z + ROUNDBULLET_RADIUS * sin(bullet->_angle);

			// 動く方向
			roundBullet_moveDirection.x =
				bossPosition.x + cos(bullet->_angle) * ROUNDBULLET_RADIUS;

			roundBullet_moveDirection.z =
				bossPosition.z + sin(bullet->_angle) * ROUNDBULLET_RADIUS;

			//　正規化
			roundBullet_moveDirection.normalize();

			//　更新
			bullet->_mesh->pos_.x += roundBullet_moveDirection.x * deltaTime * ROUNDBULLET_MOVESPEED;
			bullet->_mesh->pos_.z += roundBullet_moveDirection.z * deltaTime * ROUNDBULLET_MOVESPEED;
		}


		// パターン２　初弾に追従する白い光線
		for (auto& bullet : cylinderRoundBlue) {

			// 左回転
			if (bullet->_id % 2 == 0) {
				bullet->_angle += deltaTime;
			}
			// 右回転
			else {
				bullet->_angle -= deltaTime;
			}

			//　青い弾との間隔
			const float SPACE_OFFSET = 150.f;

			// X
			bullet->_mesh->pos_.x = bossPosition.x + (ROUNDBULLET_RADIUS + SPACE_OFFSET) * cos(bullet->_angle);
			// Y
			bullet->_mesh->pos_.y = bossPosition.y;
			// Z
			bullet->_mesh->pos_.z = bossPosition.z + (ROUNDBULLET_RADIUS + SPACE_OFFSET) * sin(bullet->_angle);

			//　更新
			bullet->_mesh->pos_.x += roundBullet_moveDirection.x * deltaTime * ROUNDBULLET_MOVESPEED;
			bullet->_mesh->pos_.z += roundBullet_moveDirection.z * deltaTime * ROUNDBULLET_MOVESPEED;
		}

		// パターン３　全方位弾 １回につき16発 * 3連続周囲に発射。それを４セット
		for (auto& bullet : sphereRoundRed) {

			const float EVERYDIRECTION_BULLETRADIUS = 100.0f;
			const float BULLET_SPEED = 200.0f;
			const float LIFETIME_LIMIT = 3.0f;
			const float STARTTIME_OFFSET = 0.5f;
			const int BULLET_NUM_PER_GROUP = 16;

			// 角度
			float angle = (2.0f * tnl::PI / BULLET_NUM_PER_GROUP) * (bullet->_id % BULLET_NUM_PER_GROUP);

			//　初期化
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

			//　タイマー起動
			bullet->_timer += deltaTime;

			//　グループごとに分けた弾の各更新開始時間
			float startTiming = STARTTIME_OFFSET * (bullet->_id / BULLET_NUM_PER_GROUP);

			if (bullet->_isActive && bullet->_timer >= startTiming) {

				//　リセット
				if (bullet->_timer > LIFETIME_LIMIT) {

					bullet->_isActive = false;
					bullet->_timer = 0;

					bullet->_mesh->pos_.x = bossPosition.x + EVERYDIRECTION_BULLETRADIUS * cos(angle);
					bullet->_mesh->pos_.y = bossPosition.y;
					bullet->_mesh->pos_.z = bossPosition.z + EVERYDIRECTION_BULLETRADIUS * sin(angle);
				}
			}

			//　更新
			bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_SPEED;
		}
	}


	//　特殊攻撃１-----------------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_MetalFatigue_Patchouli(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli.size(); i++) {

			auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli[i];

			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow) {

				//　弾ごとの角度決定
				float angle_origin =
					(2.0f * tnl::PI / EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) *
					(i % EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI);

				const float RADIUS_ORIGIN = 150.0f;
				const float BULLET_MOVESPEED = 300.0f;
				const float BULLET_LIFETIME = 0.5f;
				const int BULLET_OFFSET = 8;

				tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

				// 初弾 ８個
				if (bullet->_id < BULLET_OFFSET) {

					// タイマー起動
					bullet->_timer += deltaTime;
					bullet->_isActive = true;

					//　弾リセット
					if (bullet->_timer > BULLET_LIFETIME) {

						bullet->_isActive = false;
						bullet->_mesh->pos_ = bossPosition;
						bullet->_timer = 0.f;
					}

					bullet->_moveDirection.x = bossPosition.x + cos(angle_origin) * RADIUS_ORIGIN;
					bullet->_moveDirection.y = bossPosition.y + sin(angle_origin) * RADIUS_ORIGIN;

					bullet->_moveDirection.normalize();

					//　更新
					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_MOVESPEED;
				}

				//　1波 64個
				if (bullet->_id >= BULLET_OFFSET &&
					bullet->_id < BULLET_OFFSET + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) {

					WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, RADIUS_ORIGIN, 0.f);
				}

				//　2波 64個
				if (bullet->_id >= BULLET_OFFSET + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI &&
					bullet->_id < BULLET_OFFSET + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2)) {

					WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, RADIUS_ORIGIN, 0.1f);
				}

				//　3波 64個
				if (bullet->_id >= BULLET_OFFSET + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI) * 2 &&
					bullet->_id < BULLET_OFFSET + (SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 3)) {

					WaveAssist_MetalFatigue_Patchouli(bullet, deltaTime, angle_origin, RADIUS_ORIGIN, 0.2f);
				}
			}
		}
	}

	//　特殊攻撃１ 処理アシスト関数------------------------------------------------------
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

		// タイマー起動
		bullet->_timer += deltaTime;

		// 1　初弾と同じように飛ぶ　ライフタイム0.5秒
		if (bullet->_timer > startMoveTime && bullet->_timer < startMoveTime + OFFSET) {

			bullet->_isActive = true;

			bulletMoveOrbit.x = bossPosition.x + cos(angleOrigin) * radiusOrigin;
			bulletMoveOrbit.y = bossPosition.y + sin(angleOrigin) * radiusOrigin;

			bullet->_moveDirection = bulletMoveOrbit;
		}
		// 2　停止位置からさらに全８方向に分裂させて飛ばす　ライフタイム 1.0秒
		if (bullet->_timer > startMoveTime + OFFSET && bullet->_timer < startMoveTime + (OFFSET * 2.f)) {

			float angle = (2.0f * tnl::PI / 8) * (bullet->_id / 8);

			bulletMoveOrbit.x = cos(angle) * radiusOrigin;
			bulletMoveOrbit.y = sin(angle) * radiusOrigin;
			bulletMoveOrbit.z = sin(angle) * radiusOrigin;

			bullet->_moveDirection = bulletMoveOrbit;
		}
		// 3　発射から２秒後に弾をリセット
		if (bullet->_timer > startMoveTime + (OFFSET * 4.f)) {

			bullet->_isActive = false;

			bullet->_mesh->pos_ = bossPosition;
			bullet->_timer = 0.f;
		}

		//　更新
		bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime;
	}


	//　特殊攻撃２ ----------------------------------------------------------------------------------------------------------
	void BulletHell::ShotBulletHell_SilentSerena_Patchouli(const float deltaTime) {

		for (int i = 0; i < EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli.size(); i++) {

			auto bullet = EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli[i];
			tnl::Vector3 bossPosition = _bossMesh_ref->pos_;

			//　パターン１　円周上放射。球面座標を用いてあらゆる方向へ撃つ
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_Round_Sky) {

				const float RADIUS = 20.0f;
				const float LIFETIME_LIMIT = 5.0f;
				const float BULLET_MOVESPEED = 200.f;
				const int SHOT_DIRECTION_COUNT = 16;

				// 方位角
				float theta =
					(2.0f * tnl::PI / CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI) *
					(bullet->_id % CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI);

				// 天頂角
				float phi = (tnl::PI / SHOT_DIRECTION_COUNT) * (bullet->_id % SHOT_DIRECTION_COUNT);

				// 初期化
				if (bullet->_timer == 0.f) {

					/*
					　 ※　Direct Xの座標系は　yが上下、zが奥行を指す（　このプロジェクトも該当する　）
					   　　フレームワークやゲームエンジンなどによっては　zが上下、yが奥行を表す場合があるので注意


						   3次元極座標　（ 動径、シータ（角度）、パイ（角度））

						　 弾を円周上に等間隔で配置
						　 この場合、パイ（天頂角） と　シータ（偏角または方位角という）の２つの角度を使用
						　（ 天頂角は　 sin(phi)　でのみ表される　(cos は不可）
						　
						　 天頂角       == 北を基準とした点までの角度
						　 方位角(偏角) == 原点から点までの角度
						　
						　 おまけ　→　仰角 == 基準平面から垂直軸までの角度(通常、水平面)
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

				//　タイマー起動
				bullet->_timer += deltaTime;

				//　更新
				if (bullet->_timer > 0.f && bullet->_timer < LIFETIME_LIMIT) {

					bullet->_isActive = true;

					bullet->_mesh->pos_ += bullet->_moveDirection * deltaTime * BULLET_MOVESPEED;
				}

				//　リセット
				if (bullet->_timer > LIFETIME_LIMIT) {

					bullet->_isActive = false;

					bullet->_timer = 0.f;
					bullet->_mesh->pos_ = bossPosition;
				}
			}

			//　パターン２　プレイヤーへゆっくり近づく弾
			if (bullet->specificType == EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue) {

				tnl::Vector3 moveDir = bullet->_moveDirection;

				const float LIFETIME_LIMIT = 7.0f;
				const float BULLET_MOVESPEED = 50.f;

				//　初期化
				if (bullet->_timer == 0.f) {

					// ランダムベクター
					tnl::Vector3 randomVector = RandomValueGenerator::Vector(-400, 400, -50, 150, -250, 200);
					tnl::Vector3 spawnPos;

					spawnPos.x = bossPosition.x + randomVector.x;
					spawnPos.y = bossPosition.y + randomVector.y;
					spawnPos.z = bossPosition.z + randomVector.z;

					//　プレイヤー方向取得
					moveDir = _player_ref->GetPos() - spawnPos;

					moveDir.normalize();

					bullet->_mesh->pos_ = spawnPos;
					bullet->_moveDirection = moveDir;

					bullet->_isActive = true;
				}

				//　タイマー起動
				bullet->_timer += deltaTime;

				//　更新
				bullet->_mesh->pos_ += moveDir * deltaTime * BULLET_MOVESPEED;

				//　リセット
				if (bullet->_timer > LIFETIME_LIMIT) {

					bullet->_mesh->pos_ = bossPosition;

					bullet->_timer = 0.f;
					bullet->_isActive = false;
				}
			}
		}
	}
}