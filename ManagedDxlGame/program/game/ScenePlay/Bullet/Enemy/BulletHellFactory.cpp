#include "../../../Manager/Enemy/EnemyManager.h"
#include "../../ScenePlay.h"
#include "../../../DxLibEngine.h"
#include "../../../Loader/CsvLoader.h"
#include "EnemyBullet.h"
#include "BulletHell.h"
#include "BulletHellFactory.h"


BulletHellFactory::BulletHellFactory() {

	_bulletHellData_map = _csvLoader->LoadBulletHellTypeInfos("BulletHellType.csv");
	InitBulletHellInfo();
}


void BulletHellFactory::InitBulletHellInfo() {

	for (auto it_bltData = _bulletHellData_map.begin(); it_bltData != _bulletHellData_map.end(); it_bltData++) {

		switch ((*it_bltData).first) // id
		{
		case 1:
			_sBltHell_patchouli_info._typeName = (*it_bltData).second._typeName;
			_sBltHell_patchouli_info._maxBulletSpawnCount = (*it_bltData).second._maxBulletSpawnCount;
			_sBltHell_patchouli_info._bulletSpawnRate = (*it_bltData).second._bulletSpawnRate;
			break;
		case 2:
			_sBltHell_cirno_info._typeName = (*it_bltData).second._typeName;
			_sBltHell_cirno_info._maxBulletSpawnCount = (*it_bltData).second._maxBulletSpawnCount;
			_sBltHell_cirno_info._bulletSpawnRate = (*it_bltData).second._bulletSpawnRate;
			break;
		case 3:
			_sBltHell_suwako_info._typeName = (*it_bltData).second._typeName;
			_sBltHell_suwako_info._maxBulletSpawnCount = (*it_bltData).second._maxBulletSpawnCount;
			_sBltHell_suwako_info._bulletSpawnRate = (*it_bltData).second._bulletSpawnRate;
			break;
		}
	}
}


std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_Normal_Patchouli() {

	std::vector<Shared<EnemyBullet>> enemyBullet;
	int id_roundBullet = 0;
	int id_everyDirBullet = 0;

	// 回転用の弾生成 (左回転と右回転の２種類があるが、ここでは全てを同時に生成する）
	for (int i = 0; i < _ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI; i++) {

		// 球体--------------------------------------------------------------------------------------------------------------------------
		Shared<EnemyBullet> round_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue, 20.0f);

		round_bullet->_id = id_roundBullet++;
		round_bullet->_speed = 150.0f;
		round_bullet->_isActive = true;
		round_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue;

		enemyBullet.push_back(round_bullet);
	}


	for (int i = 0; i < _ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI; i++) {

		// 球体と一緒に動く光線------------------------------------------------------------------------------------------------------------
		Shared<EnemyBullet> round_bullet_beam = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Cylinder, EnemyBullet::COLOR::White, 500.f);
		
		round_bullet_beam->_angle = (2.0f * tnl::PI / _ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI) * i;

		float pitch = tnl::PI / 2.0f;

		float angle = (2.0f * tnl::PI / _ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI) * i;
		float yaw = angle;
		float roll = 0.f;

		tnl::Quaternion rotation = tnl::Quaternion::RotationRollPitchYawFromVector({ pitch, yaw, roll });

		round_bullet_beam->_mesh->rot_ = rotation;

		round_bullet_beam->_id = id_roundBullet++;
		round_bullet_beam->_speed = 150.0f;
		round_bullet_beam->_isActive = true;
		round_bullet_beam->specificType = EnemyBullet::SPECIFICTYPE::Cylinder_Round_White;


		enemyBullet.push_back(round_bullet_beam);
	}


	// 全方向連射弾--------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < _EVERYDIRECTION_BULLETCOUNT_T_NORMAL_PATCHOULI; i++) {

		Shared<EnemyBullet> every_direction_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::EmeraldGreen, 12.0f);

		
		every_direction_bullet->_id = id_everyDirBullet++;
		every_direction_bullet->_isActive = false;
		every_direction_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_EmeraldGreen;

		enemyBullet.push_back(every_direction_bullet);
	}


	return enemyBullet;
}


std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_MetalFatigue_Patchouli() {

	std::vector<Shared<EnemyBullet>> enemyBullet;
	int id = 0;

	for (int i = 0; i < _EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI; i++) {

		Shared<EnemyBullet> every_direction_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Yellow, 15.0f);


		every_direction_bullet->_id = id++;
		every_direction_bullet->_isActive = false;
		every_direction_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow;

		enemyBullet.push_back(every_direction_bullet);
	}


	Shared<EnemyBullet> split_one_into_eight_bullet_wave1;
	Shared<EnemyBullet> split_one_into_eight_bullet_wave2;
	Shared<EnemyBullet> split_one_into_eight_bullet_wave3;

	InitSplitOneIntoEightBullet(_SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI, id, split_one_into_eight_bullet_wave1, enemyBullet, EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow);
	//InitSplitOneIntoEightBullet(_SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI, id, split_one_into_eight_bullet_wave2, enemyBullet, EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow);
	//InitSplitOneIntoEightBullet(_SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI, id, split_one_into_eight_bullet_wave3, enemyBullet, EnemyBullet::SPECIFICTYPE::Sphere_Round_Yellow);


	return enemyBullet;
}


void BulletHellFactory::InitSplitOneIntoEightBullet(
	int split_one_into_eight_wave, int id, Shared<EnemyBullet>& split_one_into_eight_bullet, std::vector<Shared<EnemyBullet>>& enemyBullet, EnemyBullet::SPECIFICTYPE wave)
{
	for (int i = 0; i < split_one_into_eight_wave; i++) {

		split_one_into_eight_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Yellow, 12.0f);

		split_one_into_eight_bullet->_radius = 65.0f;
		split_one_into_eight_bullet->_angle = (2.0f * tnl::PI / 8) * i;
		
		split_one_into_eight_bullet->_mesh->pos_.x = split_one_into_eight_bullet->_radius * cos(split_one_into_eight_bullet->_angle);
		split_one_into_eight_bullet->_mesh->pos_.y = split_one_into_eight_bullet->_mesh->pos_.y;;
		split_one_into_eight_bullet->_mesh->pos_.z = split_one_into_eight_bullet->_radius * sin(split_one_into_eight_bullet->_angle);
		
		split_one_into_eight_bullet->_id = id++;
		split_one_into_eight_bullet->_isActive = true;
		split_one_into_eight_bullet->specificType = wave;

		enemyBullet.push_back(split_one_into_eight_bullet);
	}
}




std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_SilentSerena_Patchouli() {

	std::vector<Shared<EnemyBullet>> enemyBullet;
	int id = 0;

	// 全方向連射弾--------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < _CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI; i++) {

		Shared<EnemyBullet> circle_contiguous_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::EmeraldGreen, 15.0f);
		
		circle_contiguous_bullet->_id = id++;
		circle_contiguous_bullet->_isActive = false;
		circle_contiguous_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_EmeraldGreen;

		enemyBullet.push_back(circle_contiguous_bullet);
	}


	// スローで動くランダム弾--------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < _SLOWLYCOMING_BULLETCOUNT_T_SILENTSERENA_PATCHOULI; i++) {

		Shared<EnemyBullet> slowly_coming_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue,10.0f);


		slowly_coming_bullet->_id = id++;
		slowly_coming_bullet->_isActive = false;
		slowly_coming_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue;

		enemyBullet.push_back(slowly_coming_bullet);
	}

	return enemyBullet;
}



std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_Normal_Cirno() {

	std::vector<Shared<EnemyBullet>> enemyBullet;
	int id = 0;

	for (int i = 0; i < _STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO; i++) {

		Shared<EnemyBullet> straight_approach_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue, 15.0f);		
		
		straight_approach_bullet->_id = id++;
		straight_approach_bullet->_isActive = false;
		straight_approach_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue;

		enemyBullet.push_back(straight_approach_bullet);
	}

	for (int i = 0; i < _EVERYDIRECTION_BULLETCOUNT_T_NORMAL_CIRNO; i++) {

		Shared<EnemyBullet> every_direction_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue, 15.0f);

		every_direction_bullet->_id = id++;
		every_direction_bullet->_isActive = false;
		every_direction_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_Blue;

		enemyBullet.push_back(every_direction_bullet);
	}

	return enemyBullet;
}


std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_IcicleFall_Cirno() {

	std::vector<Shared<EnemyBullet>> enemyBullet;
	int id = 0;

	for (int i = 0; i < _SHOTOUTERMOVEINNER_BULLETCOUNT_T_ICICLEFALL_CIRNO; i++) {

		Shared<EnemyBullet> shotOuter_moveInner_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue, 15.0f);

		shotOuter_moveInner_bullet->_id = id++;
		shotOuter_moveInner_bullet->_isActive = false;
		shotOuter_moveInner_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Blue;

		enemyBullet.push_back(shotOuter_moveInner_bullet);
	}

	for (int i = 0; i < _LINEUPSTRAIGHTSHOT_BULLETCOUNT_T_ICICLEFALL_CIRNO; i++) {

		Shared<EnemyBullet> lineUp_straight_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Yellow, 15.0f);

		lineUp_straight_bullet->_id = id++;
		lineUp_straight_bullet->_isActive = false;
		lineUp_straight_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Straight_Yellow;

		enemyBullet.push_back(lineUp_straight_bullet);
	}

	return enemyBullet;

}



std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_PerfectFreeze() {

	std::vector<Shared<EnemyBullet>> enemyBullet;
	int id = 0;

	// 全方向連射弾--------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < _CIRCLECONTIGUOUS_BULLETCOUNT_T_PERFECTFREEZE_CIRNO; i++) {

		Shared<EnemyBullet> circle_contiguous_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::EmeraldGreen, 15.0f);

		circle_contiguous_bullet->_id = id++;
		circle_contiguous_bullet->_isActive = false;
		circle_contiguous_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_Round_EmeraldGreen;

		enemyBullet.push_back(circle_contiguous_bullet);
	}


	// 5列の直行弾を左から右へ広がるように発射（1回で全て放射）--------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < _EXPANDSTRAIGHTSHOT_BULLETCOUNT_T_PERFECTFREEZE_CIRNO; i++) {

		Shared<EnemyBullet> expand_straight_bullet = std::make_shared<EnemyBullet>(EnemyBullet::SHAPE::Sphere, EnemyBullet::COLOR::Blue, 10.0f);

		expand_straight_bullet->_id = id++;
		expand_straight_bullet->_isActive = false;
		expand_straight_bullet->specificType = EnemyBullet::SPECIFICTYPE::Sphere_RandomStraight_Blue;

		enemyBullet.push_back(expand_straight_bullet);
	}

	return enemyBullet;
}



std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_Normal_Suwako() {

	std::vector<Shared<EnemyBullet>> enemyBullet;

	return enemyBullet;

}


std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_IronRingOfMoriya_Suwako() {

	std::vector<Shared<EnemyBullet>> enemyBullet;

	return enemyBullet;

}



std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell_KeroChanStandsFirm_AgainstTheStorm() {

	std::vector<Shared<EnemyBullet>> enemyBullet;

	return enemyBullet;

}



std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell(const BulletHell::TYPE type) {

	switch (type)
	{
	// パチュリー
	case BulletHell::TYPE::Normal_Patchouli:
		return CreateBulletHell_Normal_Patchouli();
		break;
	case BulletHell::TYPE::MetalFatigue_Patchouli:
		return CreateBulletHell_MetalFatigue_Patchouli();
		break;
	case BulletHell::TYPE::SilentSerena_Patchouli:
		return CreateBulletHell_SilentSerena_Patchouli();
		break;
	// チルノ
	case BulletHell::TYPE::Normal_Cirno:
		return CreateBulletHell_Normal_Cirno();
		break;
	case BulletHell::TYPE::IcicleFall_Cirno:
		return CreateBulletHell_IcicleFall_Cirno();
		break;
	case BulletHell::TYPE::Perfect_Freeze_Cirno:
		return CreateBulletHell_PerfectFreeze();
		break;
	// 諏訪子
	case BulletHell::TYPE::Normal_Suwako:
		return CreateBulletHell_Normal_Suwako();
		break;
	case BulletHell::TYPE::IronRingOfMoriya_Suwako:
		return CreateBulletHell_IronRingOfMoriya_Suwako();
		break;
	case BulletHell::TYPE::KeroChan_StandsFirm_AgainstTheStorm_Suwako:
		return CreateBulletHell_KeroChanStandsFirm_AgainstTheStorm();
		break;
	}
	return std::vector<Shared<EnemyBullet>>();
}