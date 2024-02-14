#include "../../../Bullet/Enemy/BulletHell.h"
#include "../../../ScenePlay.h"
#include "EnemyBoss_MoriyaSuwako.h"

int EnemyBoss_MoriyaSuwako::_at;


EnemyBoss_MoriyaSuwako::EnemyBoss_MoriyaSuwako(const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera)
	: EnemyBossBase(info, player, camera)  {

	_collide_size = { 50,50,50 };
	_at = 3;
}


void EnemyBoss_MoriyaSuwako::SetMeshInfo() {

	_mesh = dxe::Mesh::CreateSphereMV(50);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/bossTexture/Suwako.jpg"));

	_mesh->pos_ = { 0, 0, 0 };
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}


void EnemyBoss_MoriyaSuwako::InitBulletHellInstance() {

	_bulletHell = std::make_shared<BulletHell>(_mesh, _player_ref);
}



void EnemyBoss_MoriyaSuwako::AttackPlayer(const float& delta_time) {

	if (EnemyBossBase::_bossHp.empty()) {
		ScenePlay::TurnOff_ThirdStageBulletHellLists();	return;
	}

	if (!_bulletHell) return;

	if (4 == EnemyBossBase::_bossHp.size() || 2 == EnemyBossBase::_bossHp.size()) {

		ScenePlay::_isUsingBullet_normal_suwako = true;
		_bulletHell->ShotBulletHell_Normal_Suwako(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_normal_suwako = false;
	}

	if (3 == EnemyBossBase::_bossHp.size()) {
		ScenePlay::_isUsingBullet_ironRingOfMoriya_suwako = true;
		_bulletHell->ShotBulletHell_IronRingOfMoriya_Suwako(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_ironRingOfMoriya_suwako = false;
	}

	if (1 == EnemyBossBase::_bossHp.size()) {
		ScenePlay::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = true;
		_bulletHell->ShotBulletHell_KeroChanStandsFirm_AgainstTheStorm_Suwako(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako = false;
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


bool EnemyBoss_MoriyaSuwako::Update(float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);
	return true;
}