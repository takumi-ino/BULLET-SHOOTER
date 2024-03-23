#include "BulletHell.h"
#include "EnemyBullet.h"
#include "BulletHellFactory.h"
#include "../game/Utility/CustomException.h"
#include "BulletHellFactoryMethod/Stage1Boss/AllBulletHellFactory_PatchouliKnowledge.h"
#include "BulletHellFactoryMethod/Stage2Boss/AllBulletHellFactory_Cirno.h"
#include "BulletHellFactoryMethod/Stage3Boss/AllBulletHellFactory_MoriyaSuwako.h"


namespace inl {


	BulletHellFactory::BulletHellFactory() {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		auto csv = cus->TryLoadCsvFile("BulletHellType.csv", "inl::BulletHellFactory::BulletHellFactory()");

		_bulletHellData_map = _csvLoader->LoadBulletHellTypeInfos(csv);
		InitBulletHellInfo();
	}


	void BulletHellFactory::InitBulletHellInfo() {

		for (auto it_bltData = _bulletHellData_map.begin(); it_bltData != _bulletHellData_map.end(); it_bltData++) {

			switch ((*it_bltData).first) // id
			{
			case 1: 
			{
				// 名前
				_sBltHell_patchouli_info._typeName = (*it_bltData).second._typeName;
				// 弾の最大数
				_sBltHell_patchouli_info._maxBulletSpawnCount = (*it_bltData).second._maxBulletSpawnCount;
				// 弾の生成頻度
				_sBltHell_patchouli_info._bulletSpawnRate = (*it_bltData).second._bulletSpawnRate;
				break;
			}
			case 2:
			{
				_sBltHell_cirno_info._typeName = (*it_bltData).second._typeName;
				_sBltHell_cirno_info._maxBulletSpawnCount = (*it_bltData).second._maxBulletSpawnCount;
				_sBltHell_cirno_info._bulletSpawnRate = (*it_bltData).second._bulletSpawnRate;
				break;
			}
			case 3:
			{
				_sBltHell_suwako_info._typeName = (*it_bltData).second._typeName;
				_sBltHell_suwako_info._maxBulletSpawnCount = (*it_bltData).second._maxBulletSpawnCount;
				_sBltHell_suwako_info._bulletSpawnRate = (*it_bltData).second._bulletSpawnRate;
				break;
			}
			}
		}
	}



	std::vector<Shared<EnemyBullet>> BulletHellFactory::CreateBulletHell(const BulletHell::TYPE type) {

		switch (type)
		{
			// パチュリー（ステージ１ボス）
		case BulletHell::TYPE::Normal_Patchouli:
			return CreateBulletHell_Normal_Patchouli();
			break;
		case BulletHell::TYPE::MetalFatigue_Patchouli:
			return CreateBulletHell_MetalFatigue_Patchouli();
			break;
		case BulletHell::TYPE::SilentSerena_Patchouli:
			return CreateBulletHell_SilentSerena_Patchouli();
			break;

			// チルノ（ステージ２ボス）
		case BulletHell::TYPE::Normal_Cirno:
			return CreateBulletHell_Normal_Cirno();
			break;
		case BulletHell::TYPE::IcicleFall_Cirno:
			return CreateBulletHell_IcicleFall_Cirno();
			break;
		case BulletHell::TYPE::Perfect_Freeze_Cirno:
			return CreateBulletHell_PerfectFreeze_Cirno();
			break;

			// 諏訪子（ステージ３ボス）
		case BulletHell::TYPE::Normal_Suwako:
			return CreateBulletHell_Normal_Suwako();
			break;
		case BulletHell::TYPE::IronRingOfMoriya_Suwako:
			return CreateBulletHell_IronRingOfMoriya_Suwako();
			break;
		case BulletHell::TYPE::KeroChan_StandsFirm_AgainstTheStorm_Suwako:
			return CreateBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako();
			break;
		}
		return std::vector<Shared<EnemyBullet>>();
	}
}