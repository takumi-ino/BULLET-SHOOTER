#include "EnemyBoss_PatchouliKnowledge.h"
#include "../../../Bullet/Enemy/BulletHell.h"

namespace inl {

	std::vector<Shared<EnemyBullet>> EnemyBoss_PatchouliKnowledge::_bullet_normal_patchouli;
	std::vector<Shared<EnemyBullet>> EnemyBoss_PatchouliKnowledge::_bullet_silentSerena_patchouli;
	std::vector<Shared<EnemyBullet>> EnemyBoss_PatchouliKnowledge::_bullet_metalFatigue_patchouli;
	bool EnemyBoss_PatchouliKnowledge::_isUsingBullet_normal_patchouli;
	bool EnemyBoss_PatchouliKnowledge::_isUsingBullet_silentSerena_patchouli;
	bool EnemyBoss_PatchouliKnowledge::_isUsingBullet_metalFatigue_patchouli;


	EnemyBoss_PatchouliKnowledge::EnemyBoss_PatchouliKnowledge(
		const EnemyBossInfo& info, 
		const Shared<Player>& player, 
		const Shared<dxe::Camera>& camera,
		const Shared<Collision>& collision)
		: EnemyBossBase(info, player, camera, collision) {

		_collideSize = { 50,50,50 };
		_at = 6;

		_WARPING_DURATION = 10.0f;
	}


	void EnemyBoss_PatchouliKnowledge::SetMeshInfo() {

		Shared<CustomException> cus = std::make_shared<CustomException>();

		auto texture = 
			cus->TryLoadTexture("graphics/bossTexture/PatchouliKnowledge.jpg", "inl::EnemyBoss_PatchouliKnowledge::SetMeshInfo()");

		_mesh = dxe::Mesh::CreateCubeMV(100);
		_mesh->setTexture(texture);

		_mesh->pos_ = { 0, 0, 0 };
		_mesh->scl_ = { 1.0f, 1.0f, 1.0f };
	}


	void EnemyBoss_PatchouliKnowledge::InitBulletHellInstance() {

		_bulletHell = std::make_shared<BulletHell>(_mesh, _player_ref);
	}



	void EnemyBoss_PatchouliKnowledge::AttackPlayer(const float deltaTime) {

		if (!_bulletHell) return;


		if (4 == EnemyBossBase::_bossHp.size() || 2 == EnemyBossBase::_bossHp.size()) {

			_isUsingBullet_normal_patchouli = true;
			_bulletHell->ShotBulletHell_Normal_Patchouli(deltaTime);

			CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_normal_patchouli);
		}
		else {
			_isUsingBullet_normal_patchouli = false;
		}

		if (3 == EnemyBossBase::_bossHp.size()) {
			_isUsingBullet_metalFatigue_patchouli = true;
			_bulletHell->ShotBulletHell_MetalFatigue_Patchouli(deltaTime);

			CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_metalFatigue_patchouli);
		}
		else {
			_isUsingBullet_metalFatigue_patchouli = false;
		}

		if (1 == EnemyBossBase::_bossHp.size()) {
			_isUsingBullet_silentSerena_patchouli = true;
			_bulletHell->ShotBulletHell_SilentSerena_Patchouli(deltaTime);

			CheckCollision_BulletHellBulletsAndPlayer_DRY(_bullet_silentSerena_patchouli);
		}
		else {
			_isUsingBullet_silentSerena_patchouli = false;
		}
	}


	void EnemyBoss_PatchouliKnowledge::DoRoutineMoves(const float deltaTime) {

		LookAtPlayer();

		ActKeepDistanceToPlayer(deltaTime);

		WarpToRandomPos(deltaTime);

		AttackPlayer(deltaTime);
	}


	void EnemyBoss_PatchouliKnowledge::RenderBossSpellCardName() {

		int x = 650;
		int y = 700;

		std::string spell = "Spell:";

		SetFontSize(17);
		switch (EnemyBossBase::_bossHp.size())
		{
		case 4:
			DrawFormatString(x, y, -1, "%sNormal", spell.c_str());       break;
		case 3:
			DrawFormatString(x, y, -1, "%sMetalFatigue", spell.c_str()); break;
		case 2:
			DrawFormatString(x, y, -1, "%sNormal", spell.c_str());       break;
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


	bool EnemyBoss_PatchouliKnowledge::Update(const float deltaTime) {

		if (_isDead) return false;

		DoRoutineMoves(deltaTime);
		return true;
	}
}