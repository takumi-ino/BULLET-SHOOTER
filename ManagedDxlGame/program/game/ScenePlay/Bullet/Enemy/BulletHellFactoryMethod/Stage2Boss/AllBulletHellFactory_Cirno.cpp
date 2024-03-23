#include "AllBulletHellFactory_Cirno.h"
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
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_Normal_Cirno() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int straight_id = 0;
		int every_direction_id = 0;


		// 直行弾--------------------------------------------------------------------------------------
		for (int i = 0; i < STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO; i++) {

			Shared<EnemyBullet> straight_approach_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Cylinder,
					EnemyBullet::COLOR::Blue, 
					10.0f
				);

			straight_approach_bullet->_id = straight_id++;
			straight_approach_bullet->_isActive = true;
			straight_approach_bullet->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Straight_Blue;

			enemyBullet.push_back(straight_approach_bullet);
		}

		// 全方向連射弾---------------------------------------------------------------------------------
		for (int i = 0; i < EVERYDIRECTION_BULLETCOUNT_T_NORMAL_CIRNO; i++) {

			Shared<EnemyBullet> every_direction_bullet =
				std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue, 15.0f);

			every_direction_bullet->_id = every_direction_id++;
			every_direction_bullet->_isActive = true;
			every_direction_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue;

			enemyBullet.push_back(every_direction_bullet);
		}

		return enemyBullet;
	}


	//　特殊攻撃１---------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_IcicleFall_Cirno() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int shotOuter_moveInner_id = 0;
		int lineUp_straight_id = 0;

		// 外側へ撃ち、内側へ移動させる弾----------------------------------------------------------------------
		for (int i = 0; i < SHOTOUTERMOVEINNER_BULLETCOUNT_T_ICICLEFALL_CIRNO; i++) {

			Shared<EnemyBullet> shotOuter_moveInner_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere, 
					EnemyBullet::COLOR::Blue,
					15.0f
				);

			shotOuter_moveInner_bullet->_id = shotOuter_moveInner_id++;
			shotOuter_moveInner_bullet->_isActive = true;
			shotOuter_moveInner_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue;

			enemyBullet.push_back(shotOuter_moveInner_bullet);
		}

		// １行５列の弾（３セット）-----------------------------------------------------------------------------
		for (int i = 0; i < LINEUPSTRAIGHTSHOT_BULLETCOUNT_T_ICICLEFALL_CIRNO; i++) {

			Shared<EnemyBullet> lineUp_straight_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::Yellow, 
					15.0f
				);

			lineUp_straight_bullet->_id = lineUp_straight_id++;
			lineUp_straight_bullet->_isActive = false;
			lineUp_straight_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow;

			enemyBullet.push_back(lineUp_straight_bullet);
		}

		return enemyBullet;
	}


	//　特殊攻撃２--------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_PerfectFreeze_Cirno() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int circle_contiguous_bullet_id = 0;
		int expand_straight_bullet_id = 0;

		// 全方向連射弾--------------------------------------------------------------------------------
		for (int i = 0; i < CIRCLECONTIGUOUS_BULLETCOUNT_T_PERFECTFREEZE_CIRNO; i++) {

			Shared<EnemyBullet> circle_contiguous_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::White, 
					15.0f
				);

			circle_contiguous_bullet->_id = circle_contiguous_bullet_id++;
			circle_contiguous_bullet->_isActive = true;
			circle_contiguous_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_White;

			enemyBullet.push_back(circle_contiguous_bullet);
		}


		// 3行１列の弾を５連続、これを８方向へ撃つ(間隔を空けずに全て放射）-----------------------------
		for (int i = 0; i < EXPANDSTRAIGHTSHOT_BULLETCOUNT_T_PERFECTFREEZE_CIRNO; i++) {

			Shared<EnemyBullet> expand_straight_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::Blue, 
					10.0f
				);

			expand_straight_bullet->_id = expand_straight_bullet_id++;
			expand_straight_bullet->_isActive = true;
			expand_straight_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue;

			enemyBullet.push_back(expand_straight_bullet);
		}

		return enemyBullet;
	}
}