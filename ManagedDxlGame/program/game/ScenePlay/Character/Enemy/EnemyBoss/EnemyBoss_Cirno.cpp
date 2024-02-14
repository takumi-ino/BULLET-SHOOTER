#include "../../../Bullet/Enemy/BulletHell.h"
#include "../../../ScenePlay.h"
#include "../EnemyBossBase.h"
#include "EnemyBoss_Cirno.h"

int EnemyBoss_Cirno::_at;

EnemyBoss_Cirno::EnemyBoss_Cirno(const EnemyBossInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera)
	: EnemyBossBase(info, player, camera) {

	_collide_size = { 50,50,50 };
	_at = 4;

}


void EnemyBoss_Cirno::SetMeshInfo() {

	_mesh = dxe::Mesh::CreateSphereMV(50);
	_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/bossTexture/Cirno.png"));

	_mesh->pos_ = { 0, 0, 0 };
	_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
}


void EnemyBoss_Cirno::InitBulletHellInstance() {

	_bulletHell = std::make_shared<BulletHell>(_mesh, _player_ref);
}




void EnemyBoss_Cirno::AttackPlayer(const float& delta_time) {

	if (EnemyBossBase::_bossHp.empty()) {
		ScenePlay::TurnOff_SecondStageBulletHellLists();
		return;
	}

	if (!_bulletHell) return;

	if (4 == EnemyBossBase::_bossHp.size() || 2 == EnemyBossBase::_bossHp.size()) {

		ScenePlay::_isUsingBullet_normal_cirno = true;
		_bulletHell->ShotBulletHell_Normal_Cirno(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_normal_cirno = false;
	}

	if (3 == EnemyBossBase::_bossHp.size()) {
		ScenePlay::_isUsingBullet_icicleFall_cirno = true;
		_bulletHell->ShotBulletHell_IcicleFall_Cirno(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_icicleFall_cirno = false;
	}

	if (1 == EnemyBossBase::_bossHp.size()) {
		ScenePlay::_isUsingBullet_perfectFreeze_cirno = true;
		_bulletHell->ShotBulletHell_PerfectFreeze_Cirno(delta_time);
	}
	else {
		ScenePlay::_isUsingBullet_perfectFreeze_cirno = false;
	}
}



void EnemyBoss_Cirno::DoRoutineMoves(const float& delta_time) {
	
	LookAtPlayer();

	ActKeepDistanceToPlayer(delta_time);

	AttackPlayer(delta_time);
}


void EnemyBoss_Cirno::RenderBossSpellCardName() {
	int x = 650;
	int y = 700;

	std::string spell = "Spell:";

	SetFontSize(17);
	switch (EnemyBossBase::_bossHp.size())
	{
	case 4:
		DrawFormatString(x, y, -1, "%sNormal", spell.c_str()); break;
	case 3:
		DrawFormatString(x, y, -1, "%sIcicleFall", spell.c_str()); break;
	case 2:
		DrawFormatString(x, y, -1, "%sNormal", spell.c_str()); break;
	case 1:
		DrawFormatString(x, y, -1, "%sPerfectFreeze", spell.c_str()); break;
	}
	SetFontSize(22);
}


void EnemyBoss_Cirno::Render(Shared<dxe::Camera> camera) {

	if (_isDead) return;

	_mesh->render(camera);


	ShowHpGage_EnemyBoss();
	RenderBossSpellCardName();
}


bool EnemyBoss_Cirno::Update(float delta_time) {

	if (_isDead) return false;

	DoRoutineMoves(delta_time);
	return true;
}