#include "EnemyBoss_MoriyaSuwako.h"
#include "../../../Bullet/Enemy/BulletHell.h"

namespace inl {

	std::vector<Shared<EnemyBullet>> EnemyBoss_MoriyaSuwako::_bullet_normal_suwako;
	std::vector<Shared<EnemyBullet>> EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako;
	std::vector<Shared<EnemyBullet>> EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako;
	bool EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako;
	bool EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako;
	bool EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako;


	EnemyBoss_MoriyaSuwako::EnemyBoss_MoriyaSuwako(const EnemyBossInfo& info) : EnemyBossBase(info) {

		_collideSize = { 50,50,50 };
		_at = 6;

		_WARPING_DURATION = 8.0f;
	}


	void EnemyBoss_MoriyaSuwako::SetMeshInfo() {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		auto texture =
			cus->TryLoadTexture("graphics/bossTexture/Suwako.jpg", "inl::EnemyBoss_Suwako::SetMeshInfo()");

		_mesh = dxe::Mesh::CreateCubeMV(100);
		_mesh->setTexture(texture);

		_mesh->pos_ = { 0, 0, 0 };
		_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
	}


	void EnemyBoss_MoriyaSuwako::InitBulletHellInstance() {

		_bulletHell = std::make_shared<BulletHell>(_mesh);
	}



	void EnemyBoss_MoriyaSuwako::AttackPlayer(const float deltaTime) {

		if (!_bulletHell) return;

		if (4 == EnemyBossBase::_bossHp.size() || 2 == EnemyBossBase::_bossHp.size()) {

			EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako = true;
			_bulletHell->ShotBulletHell_Normal_Suwako(deltaTime);

			CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_normal_suwako);
		}
		else {
			EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako = false;
		}

		if (3 == EnemyBossBase::_bossHp.size()) {
			EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako = true;
			_bulletHell->ShotBulletHell_IronRingOfMoriya_Suwako(deltaTime);

			CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_ironRingOfMoriya_suwako);
		}
		else {
			EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako = false;
		}

		if (1 == EnemyBossBase::_bossHp.size()) {
			EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = true;
			_bulletHell->ShotBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako(deltaTime);

			CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_keroChanStandsFirmAgainstTheStorm_suwako);
		}
		else {
			EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = false;
		}
	}


	void EnemyBoss_MoriyaSuwako::DoRoutineMoves(const float deltaTime) {

		LookAtPlayer();

		ActKeepDistanceToPlayer(deltaTime);

		WarpToRandomPos(deltaTime);

		AttackPlayer(deltaTime);
	}


	void EnemyBoss_MoriyaSuwako::RenderBossSpellCardName() {

		int x = 650;
		int y = 700;

		std::string spell = "Spell:";

		SetFontSize(17);
		switch (EnemyBossBase::_bossHp.size())
		{
		case 4:
			DrawFormatString(x, y, -1, "%sNormal", spell.c_str());                             break;
		case 3:
			DrawFormatString(x, y, -1, "%sIronRingOfMoriya", spell.c_str());                   break;
		case 2:
			DrawFormatString(x, y, -1, "%sNormal", spell.c_str());                             break;
		case 1:
			DrawFormatString(x, y, -1, "%s_keroChanStandsFirmAgainstTheStorm", spell.c_str()); break;
		}
		SetFontSize(22);
	}


	void EnemyBoss_MoriyaSuwako::Render() {

		if (_isDead) return;

		_mesh->render(ScenePlay::GetInstance()->_mainCamera);

		ShowHpGage_EnemyBoss();
		RenderBossSpellCardName();
	}


	bool EnemyBoss_MoriyaSuwako::Update(const float deltaTime) {

		if (_isDead) return false;

		DoRoutineMoves(deltaTime);
		return true;
	}
}