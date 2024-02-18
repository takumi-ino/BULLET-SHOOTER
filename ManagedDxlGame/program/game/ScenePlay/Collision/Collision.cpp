#include "../Score/Score.h"
#include "../../ScenePlay/Character/Player/Player.h"
#include "../../ScenePlay/Character/Enemy/EnemyBase.h"
#include "../Character/Enemy/EnemyZako/EnemyZakoBox.h"
#include "../Bullet/Player/PlayerBullet.h"
#include "../Bullet/Enemy/StraightBullet.h"
#include "../Bullet/Enemy/HomingBullet.h"
#include "../Bullet/Enemy/EnemyBullet.h"
#include "../Character/Enemy/EnemyZakoBase.h"
#include "../Character/Enemy/EnemyBossBase.h"
#include "../Item/ScoreItem.h"
#include "../Item/PowerUpItem.h"
#include "Collision.h"


// キャラクターと弾----------------------------------------------------------------------------------------------------------------------------

bool Collision::CheckCollision_PlayerBulletAndEnemyZako(const Shared<PlayerBullet> bullet, const Shared<EnemyZakoBase> enemy) {

	if (!enemy->_mesh) return false;

	if (tnl::IsIntersectAABB(bullet->_mesh->pos_, bullet->_collisionSize, enemy->_mesh->pos_, enemy->_collide_size)) {
		return true;
	}
	return false;
}


bool Collision::CheckCollision_PlayerBulletAndEnemyBoss(const Shared<PlayerBullet> bullet, const Shared<EnemyBossBase> enemy) {

	if (!enemy->_mesh) return false;

	if (tnl::IsIntersectAABB(bullet->_mesh->pos_, bullet->_collisionSize, enemy->_mesh->pos_, enemy->_collide_size)) {
		return true;
	}
	return false;
}


bool Collision::CheckCollision_EnemyStraightBulletAndPlayer(const Shared<StraightBullet> bullet, const Shared<Player> player) {

	if (tnl::IsIntersectAABB(bullet->_mesh->pos_, bullet->_collisionSize, player->_mesh->pos_, player->_collide_size)) {
		return true;
	}
	return false;
}


bool Collision::CheckCollision_EnemyHomingBulletAndPlayer(const Shared<HomingBullet> bullet, const Shared<Player> player) {

	if (tnl::IsIntersectAABB(bullet->_mesh->pos_, bullet->_collisionSize, player->_mesh->pos_, player->_collide_size)) {
		return true;
	}
	return false;
}


bool Collision::CheckCollision_BulletHellBulletsAndPlayer(
	const Shared<EnemyBullet> bullet, const Shared<Player> player, const tnl::Vector3 collisionSize, const tnl::Vector3 prev_pos) {

	if (tnl::IsIntersectAABB(bullet->_mesh->pos_, collisionSize, player->_mesh->pos_, collisionSize)) {
		return true;
	}
	return false;
}

// キャラクター同士----------------------------------------------------------------------------------------------------------------------------

bool Collision::CheckCollision_PlayerAndEnemyZako(
	Shared<Player> player, Shared<EnemyZakoBase> enemy, tnl::Vector3 prevPos_player, tnl::Vector3 prevPos_enemy) {

	if (!enemy->_mesh) return false;

	if (tnl::IsIntersectAABB(player->_mesh->pos_, player->_mesh->scl_, enemy->_mesh->pos_, enemy->_mesh->scl_)) {

		tnl::CorrectPositionAABB(
			prevPos_player, prevPos_enemy,
			player->_mesh->scl_, enemy->_mesh->scl_,
			player->_mesh->pos_, enemy->_mesh->pos_,
			tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH,
			10.0f);

		return true;
	}
	return false;
}


bool Collision::CheckCollision_PlayerAndEnemyBoss(
	Shared<Player> player, Shared<EnemyBossBase> enemy, tnl::Vector3 prevPos_player, tnl::Vector3 prevPos_enemy) {

	if (!enemy->_mesh) return false;

	if (tnl::IsIntersectAABB(player->_mesh->pos_, player->_mesh->scl_, enemy->_mesh->pos_, enemy->_mesh->scl_)) {

		tnl::CorrectPositionAABB(
			prevPos_player, prevPos_enemy,
			player->_mesh->scl_, enemy->_mesh->scl_,
			player->_mesh->pos_, enemy->_mesh->pos_,
			tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH,
			20.0f);

		return true;
	}
	return false;
}


void Collision::CheckCollision_EnemyAndEnemy(
	Shared<EnemyZakoBase>& enemy_type1, Shared<EnemyZakoBase>& enemy_type2,const tnl::Vector3 prev_pos_e1,const tnl::Vector3 prev_pos_e2) {

	if (!enemy_type1->_mesh || !enemy_type2->_mesh) return;

	tnl::Vector3 prevChord_e1 = prev_pos_e1;
	tnl::Vector3 prevChord_e2 = prev_pos_e2;

	if (tnl::IsIntersectAABB(enemy_type1->_mesh->pos_, enemy_type1->_collide_size, enemy_type2->_mesh->pos_, enemy_type2->_collide_size)) {

		tnl::CorrectPositionAABB(
			prevChord_e1, prevChord_e2, 
			enemy_type1->_mesh->scl_, enemy_type2->_mesh->scl_, 
			enemy_type1->_mesh->pos_, enemy_type2->_mesh->pos_,
			tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH,
			30.0f);
	}
}

// プレイヤーと得点アイテム----------------------------------------------------------------------------------------------------------------------------
bool Collision::CheckCollision_PlayerAndScoreItem(const Shared<ScoreItem> scoreItem, const Shared<Player> player) {

	if (tnl::IsIntersectAABB(scoreItem->_mesh->pos_, scoreItem->_collisionSize, player->_mesh->pos_, player->_mesh->scl_)) {
		return true;
	}
	return false;
}

// 得点アイテムと得点アイテム----------------------------------------------------------------------------------------------------------------------------
void Collision::CheckCollision_ScoreItemAndScoreItem(
	Shared<ScoreItem>& scoreItem_1, Shared<ScoreItem>& scoreItem_2, const tnl::Vector3 prevPos_si1, const tnl::Vector3 prevPos_si2) {

	if (!scoreItem_1->_mesh || !scoreItem_2->_mesh) return;

	if (tnl::IsIntersectAABB(scoreItem_1->_mesh->pos_, scoreItem_1->_collisionSize, scoreItem_2->_mesh->pos_, scoreItem_2->_collisionSize)) {

		tnl::CorrectPositionAABB(
			prevPos_si1, prevPos_si2,
			scoreItem_1->_mesh->scl_, scoreItem_2->_mesh->scl_,
			scoreItem_1->_mesh->pos_, scoreItem_2->_mesh->pos_,
			tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH,
			50.0f);
	}
}

// プレイヤーとプレイヤー強化アイテム----------------------------------------------------------------------------------------------------------------------------
bool Collision::CheckCollision_PlayerAndPowerUpItem(const Shared<PowerUpItem> powerUpItem, const Shared<Player> player) {

	if (tnl::IsIntersectAABB(powerUpItem->_mesh->pos_, powerUpItem->_collisionSize, player->_mesh->pos_, player->_mesh->scl_)) {
		return true;
	}
	return false;
}
// プレイヤー強化アイテムとプレイヤー強化アイテム-------------------------------------------------------------------------------
void Collision::CheckCollision_PowerUpItemAndPowerUpItem(
	Shared<PowerUpItem>& powerUpItem_1, Shared<PowerUpItem>& powerUpItem_2, const tnl::Vector3 prevPos_pui1, const tnl::Vector3 prevPos_pui2) {

	if (!powerUpItem_1->_mesh || !powerUpItem_2->_mesh) return;

	if (tnl::IsIntersectAABB(powerUpItem_1->_mesh->pos_, powerUpItem_1->_collisionSize, powerUpItem_2->_mesh->pos_, powerUpItem_2->_collisionSize)) {

		tnl::CorrectPositionAABB(
			prevPos_pui1, prevPos_pui2,
			powerUpItem_1->_mesh->scl_, powerUpItem_2->_mesh->scl_,
			powerUpItem_1->_mesh->pos_, powerUpItem_2->_mesh->pos_,
			tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH,
			50.0f);
	}
}


// 得点アイテムとプレイヤー強化アイテム-------------------------------------------------------------------------------
void Collision::CheckCollision_ScoreItemAndPowerUpItem(
	Shared<ScoreItem>& scoreItem, Shared<PowerUpItem>& powerUpItem, const tnl::Vector3 prevPos_si, const tnl::Vector3 prevPos_pui) {

	if (!scoreItem->_mesh || !powerUpItem->_mesh) return;

	if (tnl::IsIntersectAABB(scoreItem->_mesh->pos_, scoreItem->_collisionSize, powerUpItem->_mesh->pos_, powerUpItem->_collisionSize)) {

		tnl::CorrectPositionAABB(
			prevPos_si, prevPos_pui,
			scoreItem->_mesh->scl_, powerUpItem->_mesh->scl_,
			scoreItem->_mesh->pos_, powerUpItem->_mesh->pos_,
			tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH, tnl::eCorrTypeAABB::BOTH,
			50.0f);
	}
}