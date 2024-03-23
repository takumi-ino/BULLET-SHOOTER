#include "AllBulletHellFactory_PatchouliKnowledge.h"
#include "../../BulletHell.h"
#include "../../EnemyBullet.h"
#include "../../BulletHellFactory.h"



/*　　　　　　　　　　　
		 　　  関数内の主な処理

		   １.　インスタンス生成
		   ２.　角度設定
		   ３.　弾ID設定
		   ４.　速度設定
		   ５.　アクティブ状態設定
		   ６.　弾のタイプ設定（形、動き方、色）
		   ７.　vectorコンテナに追加
*/


namespace inl {


	//　通常攻撃--------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_Normal_Patchouli() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int id_roundBullet = 0;
		int id_everyDirBullet = 0;

		// 回転用の弾生成 (左回転と右回転の２種類があるが、ここでは全てを同時に生成する）
		for (int i = 0; i < ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI; i++) {

			// 球体-------------------------------------------------------------------------------
			Shared<EnemyBullet> round_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere, 
					EnemyBullet::COLOR::Blue, 
					20.0f
				);

			round_bullet->_angle = (2.0f * tnl::PI / ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI) * i;

			round_bullet->_id = id_roundBullet++;
			round_bullet->_speed = 150.0f;
			round_bullet->_isActive = true;
			round_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue;

			enemyBullet.push_back(round_bullet);
		}


		for (int i = 0; i < ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI; i++) {

			// 球体と一緒に動く光線---------------------------------------------------------------
			Shared<EnemyBullet> round_bullet_beam =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Cylinder, 
					EnemyBullet::COLOR::Blue,
					500.f
				);

			round_bullet_beam->_angle = (2.0f * tnl::PI / ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI) * i;

			float pitch = tnl::PI / 2.0f;

			float angle = (2.0f * tnl::PI / ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI) * i;
			float yaw = angle;
			float roll = 0.f;

			tnl::Quaternion rotation =
				tnl::Quaternion::RotationRollPitchYawFromVector({ pitch, yaw, roll });

			round_bullet_beam->_mesh->rot_ = rotation;

			round_bullet_beam->_id = id_roundBullet++;
			round_bullet_beam->_speed = 150.0f;
			round_bullet_beam->_isActive = true;
			round_bullet_beam->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue;

			enemyBullet.push_back(round_bullet_beam);
		}


		// 全方向連射弾-----------------------------------------------------------------------
		for (int i = 0; i < EVERYDIRECTION_BULLETCOUNT_T_NORMAL_PATCHOULI; i++) {

			Shared<EnemyBullet> every_direction_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere, 
					EnemyBullet::COLOR::Red, 
					12.0f
				);

			every_direction_bullet->_id = id_everyDirBullet++;
			every_direction_bullet->_isActive = false;
			every_direction_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Red;

			enemyBullet.push_back(every_direction_bullet);
		}

		return enemyBullet;
	}


	//　特殊攻撃１--------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_MetalFatigue_Patchouli() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int id = 0;

		// 全方向連射弾----------------------------------------------------------------------------
		for (int i = 0; i < EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI; i++) {

			Shared<EnemyBullet> every_direction_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere, 
					EnemyBullet::COLOR::Yellow,
					15.0f
				);

			every_direction_bullet->_id = id++;
			every_direction_bullet->_isActive = false;
			every_direction_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow;

			enemyBullet.push_back(every_direction_bullet);
		}

		Shared<EnemyBullet> split_one_into_eight_bullet_wave1;
		Shared<EnemyBullet> split_one_into_eight_bullet_wave2;
		Shared<EnemyBullet> split_one_into_eight_bullet_wave3;

		InitAssist_MetalFatigue_Patchouli(SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI,
			id,
			split_one_into_eight_bullet_wave1,
			enemyBullet,
			EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow
		);

		InitAssist_MetalFatigue_Patchouli(SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI,
			id + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI,
			split_one_into_eight_bullet_wave2,
			enemyBullet,
			EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow
		);

		InitAssist_MetalFatigue_Patchouli(SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI,
			id + SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI * 2,
			split_one_into_eight_bullet_wave3,
			enemyBullet,
			EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow
		);

		return enemyBullet;
	}


	void BulletHellFactory::InitAssist_MetalFatigue_Patchouli(
		const int splitOneIntoEight_wave,
		int id,
		Shared<EnemyBullet>& splitOneIntoEight_bullet,
		std::vector<Shared<EnemyBullet>>& enemyBullet,
		const EnemyBullet::SPECIFICTYPE wave)
	{
		for (int i = 0; i < splitOneIntoEight_wave; i++) {

			splitOneIntoEight_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::Yellow, 
					12.0f
				);

			splitOneIntoEight_bullet->_radius = 65.0f;
			splitOneIntoEight_bullet->_angle = (2.0f * tnl::PI / 8) * i;

			splitOneIntoEight_bullet->_mesh->pos_.x =
				splitOneIntoEight_bullet->_radius * cos(splitOneIntoEight_bullet->_angle);

			splitOneIntoEight_bullet->_mesh->pos_.y = splitOneIntoEight_bullet->_mesh->pos_.y;

			splitOneIntoEight_bullet->_mesh->pos_.z =
				splitOneIntoEight_bullet->_radius * sin(splitOneIntoEight_bullet->_angle);

			splitOneIntoEight_bullet->_id = id++;
			splitOneIntoEight_bullet->_isActive = true;
			splitOneIntoEight_bullet->specificType = wave;

			enemyBullet.push_back(splitOneIntoEight_bullet);
		}
	}


	//　特殊攻撃２--------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_SilentSerena_Patchouli() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int id = 0;

		// 全方向連射弾---------------------------------------------------------------------------
		for (int i = 0; i < CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI; i++) {

			Shared<EnemyBullet> circle_contiguous_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::Sky, 
					15.0f
				);

			circle_contiguous_bullet->_id = id++;
			circle_contiguous_bullet->_isActive = false;
			circle_contiguous_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Sky;

			enemyBullet.push_back(circle_contiguous_bullet);
		}

		// スローで動くランダム弾-----------------------------------------------------------------
		for (int i = 0; i < SLOWLYCOMING_BULLETCOUNT_T_SILENTSERENA_PATCHOULI; i++) {

			Shared<EnemyBullet> slowly_coming_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::Blue,
					11.0f
				);

			slowly_coming_bullet->_id = id++;
			slowly_coming_bullet->_isActive = false;
			slowly_coming_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue;

			enemyBullet.push_back(slowly_coming_bullet);
		}

		return enemyBullet;
	}

}