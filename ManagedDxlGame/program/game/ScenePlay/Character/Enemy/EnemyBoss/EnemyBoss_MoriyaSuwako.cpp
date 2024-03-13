#include "EnemyBoss_MoriyaSuwako.h"
#include "../../../Bullet/Enemy/BulletHell.h"


std::vector<Shared<EnemyBullet>> EnemyBoss_MoriyaSuwako::_bullet_normal_suwako;
std::vector<Shared<EnemyBullet>> EnemyBoss_MoriyaSuwako::_bullet_ironRingOfMoriya_suwako;
std::vector<Shared<EnemyBullet>> EnemyBoss_MoriyaSuwako::_bullet_keroChanStandsFirmAgainstTheStorm_suwako;
bool EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako;
bool EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako;
bool EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako;


EnemyBoss_MoriyaSuwako::EnemyBoss_MoriyaSuwako(
	const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera, const Shared<Collision>& collision)
	: EnemyBossBase(info, player, camera, collision)  {

	_collideSize = { 50,50,50 };
	_at = 6;

	_WARPING_DURATION = 8.0f;
}


void EnemyBoss_MoriyaSuwako::SetMeshInfo() {

	_mesh = dxe::Mesh::CreateCubeMV(100);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/bossTexture/Suwako.jpg"));

	_mesh->pos_ = { 0, 0, 0 };
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}


void EnemyBoss_MoriyaSuwako::InitBulletHellInstance() {

	_bulletHell = std::make_shared<BulletHell>(_mesh, _player_ref);
}



void EnemyBoss_MoriyaSuwako::AttackPlayer(const float& delta_time) {

	if (!_bulletHell) return;

	if (4 == EnemyBossBase::_bossHp.size() || 2 == EnemyBossBase::_bossHp.size()) {

		EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako = true;
		_bulletHell->ShotBulletHell_Normal_Suwako(delta_time);

		CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_normal_suwako);
	}
	else {
		EnemyBoss_MoriyaSuwako::_isUsingBullet_normal_suwako = false;
	}

	if (3 == EnemyBossBase::_bossHp.size()) {
		EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako = true;
		_bulletHell->ShotBulletHell_IronRingOfMoriya_Suwako(delta_time);

		CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_ironRingOfMoriya_suwako);
	}
	else {
		EnemyBoss_MoriyaSuwako::_isUsingBullet_ironRingOfMoriya_suwako = false;
	}

	if (1 == EnemyBossBase::_bossHp.size()) {
		EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = true;
		_bulletHell->ShotBulletHell_KeroChanStandsFirm_AgainstTheStorm_Suwako(delta_time);

		CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_keroChanStandsFirmAgainstTheStorm_suwako);
	}
	else {
		EnemyBoss_MoriyaSuwako::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = false;
	}
}


void EnemyBoss_MoriyaSuwako::DoRoutineMoves(const float& delta_time) {

	LookAtPlayer();

	ActKeepDistanceToPlayer(delta_time);

	AttackPlayer(delta_time);
}


void EnemyBoss_MoriyaSuwako::RenderBossSpellCardName() {

	int x = 650;
	int y = 700;

	std::string spell = "Spell:";

	SetFontSize(17);
	switch (EnemyBossBase::_bossHp.size())
	{
	case 4:
		DrawFormatString(x, y, -1, "%sNormal", spell.c_str()); break;
	case 3:
		DrawFormatString(x, y, -1, "%sIronRingOfMoriya", spell.c_str()); break;
	case 2:
		DrawFormatString(x, y, -1, "%sNormal", spell.c_str()); break;
	case 1:
		DrawFormatString(x, y, -1, "%s_keroChanStandsFirmAgainstTheStorm", spell.c_str()); break;
	}
	SetFontSize(22);
}


void EnemyBoss_MoriyaSuwako::Render(Shared<dxe::Camera> camera) {

	if (_isDead) return;

	_mesh->render(camera);

	ShowHpGage_EnemyBoss();
	RenderBossSpellCardName();
}


bool EnemyBoss_MoriyaSuwako::Update(const float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);
	return true;
}