#include "../../../Bullet/Enemy/BulletHell.h"
#include "../../../ScenePlay.h"
#include "EnemyBoss_PatchouliKnowledge.h"

int EnemyBoss_PatchouliKnowledge::_at;


EnemyBoss_PatchouliKnowledge::EnemyBoss_PatchouliKnowledge(const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera)
	: EnemyBossBase(info, player, camera) {

	_collide_size = { 50,50,50 };
	_at = 5;
}


void EnemyBoss_PatchouliKnowledge::SetMeshInfo() {

	_mesh = dxe::Mesh::CreateSphereMV(50);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/bossTexture/PatchouliKnowledge.jpg"));

	_mesh->pos_ = { 0, 0, 0 };
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}


void EnemyBoss_PatchouliKnowledge::InitBulletHellInstance() {

	_bulletHell = std::make_shared<BulletHell>(_mesh, _player_ref);
}




void EnemyBoss_PatchouliKnowledge::AttackPlayer(const float& delta_time) {

	if (EnemyBossBase::_bossHp.empty()) {
		ScenePlay::TurnOff_FirstStageBulletHellLists();	return;
	}

	if (!_bulletHell) return;

	if (4 == EnemyBossBase::_bossHp.size() || 2 == EnemyBossBase::_bossHp.size()) {

		ScenePlay::_isUsingBullet_normal_patchouli = true;
		_bulletHell->ShotBulletHell_Normal_Patchouli(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_normal_patchouli = false;
	}

	if (3 == EnemyBossBase::_bossHp.size()) {
		ScenePlay::_isUsingBullet_metalFatigue_patchouli = true;
		_bulletHell->ShotBulletHell_MetalFatigue_Patchouli(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_metalFatigue_patchouli = false;
	}

	if (1 == EnemyBossBase::_bossHp.size()) {
		ScenePlay::_isUsingBullet_silentSerena_patchouli = true;
		_bulletHell->ShotBulletHell_SilentSerena_Patchouli(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_silentSerena_patchouli = false;
	}
}


void EnemyBoss_PatchouliKnowledge::DoRoutineMoves(const float& delta_time) {

	LookAtPlayer();

	ActKeepDistanceToPlayer(delta_time);

	AttackPlayer(delta_time);
}


void EnemyBoss_PatchouliKnowledge::RenderBossSpellCardName() {

	int x = 650;
	int y = 700;

	std::string spell = "Spell:";

	SetFontSize(17);
	switch (EnemyBossBase::_bossHp.size())
	{
	case 4:
		DrawFormatString(x, y, -1, "%sNormal", spell.c_str()); break;
	case 3:
		DrawFormatString(x, y, -1, "%sMetalFatigue", spell.c_str()); break;
	case 2:
		DrawFormatString(x, y, -1, "%sNormal", spell.c_str()); break;
	case 1:
		DrawFormatString(x, y, -1, "%sSilentSerena", spell.c_str()); break;
	}
	SetFontSize(22);
}


void EnemyBoss_PatchouliKnowledge::Render(Shared<dxe::Camera> camera) {

	if (_isDead) return;

	_mesh->render(camera);

	ShowHpGage_EnemyBoss();
	RenderBossSpellCardName();
}


bool EnemyBoss_PatchouliKnowledge::Update(const float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);
	return true;
}