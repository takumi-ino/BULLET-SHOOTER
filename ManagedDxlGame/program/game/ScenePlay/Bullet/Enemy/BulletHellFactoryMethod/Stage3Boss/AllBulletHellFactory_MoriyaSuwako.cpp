#include "AllBulletHellFactory_MoriyaSuwako.h"
#include "../../EnemyBullet.h"
#include "../../BulletHell.h"
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


	//　通常攻撃-------------------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_Normal_Suwako() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int rightCircle_contiguous_bullet_id = 0;
		int leftCircle_contiguous_bullet_id = 0;

		// 全方向連射弾（右回り）-----------------------------------------------------------------------
		for (int i = 0; i < CIRCLECONTIGUOUS_BULLETCOUNT_T_NORMAL_SUWAKO; i++) {

			Shared<EnemyBullet> rightCircle_contiguous_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Cylinder,
					EnemyBullet::COLOR::Red, 
					15.0f
				);

			rightCircle_contiguous_bullet->_id = rightCircle_contiguous_bullet_id++;
			rightCircle_contiguous_bullet->_isActive = false;
			rightCircle_contiguous_bullet->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red;

			enemyBullet.push_back(rightCircle_contiguous_bullet);
		}

		// 全方向連射弾（左回り）------------------------------------------------------------------------
		for (int i = 0; i < CIRCLECONTIGUOUS_BULLETCOUNT_T_NORMAL_SUWAKO; i++) {

			Shared<EnemyBullet> leftCircle_contiguous_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Cylinder,
					EnemyBullet::COLOR::Blue,
					10.0f
				);

			leftCircle_contiguous_bullet->_id = leftCircle_contiguous_bullet_id++;
			leftCircle_contiguous_bullet->_isActive = false;
			leftCircle_contiguous_bullet->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Round_Blue;

			enemyBullet.push_back(leftCircle_contiguous_bullet);
		}

		return enemyBullet;
	}


	//　特殊攻撃１-------------------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_IronRingOfMoriya_Suwako() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int slow_approach_bullet_id = 0;

		for (int i = 0; i < SLOWLYCOMING_BULLETCOUNT_T_IRONRINGOFMORIYA_SUWAKO; i++) {

			Shared<EnemyBullet> slow_approach_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Cylinder, 
					EnemyBullet::COLOR::Red, 
					12.0f
				);

			slow_approach_bullet->_id = slow_approach_bullet_id++;
			slow_approach_bullet->_isActive = false;
			slow_approach_bullet->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Round_Red;

			enemyBullet.push_back(slow_approach_bullet);
		}

		return enemyBullet;
	}


	//　特殊攻撃３-------------------------------------------------------------------------------------------------------------------
	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako() {

		std::vector<Shared<EnemyBullet>> enemyBullet;
		int fall_approach_bullet_id = 0;
		int straight_approach_bullet_id = 0;

		// スフィア　青-------------------------------------------------------------------------------
		for (int i = 0; i < FALLINGAPPROACH_BULLETCOUNT_T_KCSTANDSFIRMAGAINSTSTORM_SUWAKO; i++) {

			Shared<EnemyBullet> fall_approach_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Sphere,
					EnemyBullet::COLOR::Blue, 12.0f
				);

			fall_approach_bullet->_id = fall_approach_bullet_id++;
			fall_approach_bullet->_isActive = false;
			fall_approach_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue;

			enemyBullet.push_back(fall_approach_bullet);
		}

		// シリンダー　エメラルドグリーン-------------------------------------------------------------
		for (int i = 0; i < STRAIGHTAPPROACH_BULLETCOUNT_T_KCSTANDSFIRMAGAINSTSTORM_SUWAKO; i++) {

			Shared<EnemyBullet> straight_approach_bullet =
				std::make_shared<EnemyBullet>(
					EnemyBullet::SHAPE::Cylinder, 
					EnemyBullet::COLOR::EmeraldGreen, 
					12.0f
				);

			straight_approach_bullet->_id = straight_approach_bullet_id++;
			straight_approach_bullet->_isActive = false;
			straight_approach_bullet->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Straight_EmeraldGreen;

			enemyBullet.push_back(straight_approach_bullet);
		}

		return enemyBullet;
	}
}