#include "../../DxLibEngine.h"
#include "Collision.h"
#include "../../ScenePlay/Character/Player/Player.h"
// 敵---------------------------------------------------
#include "../../ScenePlay/Character/Enemy/EnemyBase.h"
#include "../Character/Enemy/EnemyZakoBase.h"
#include "../Character/Enemy/EnemyBossBase.h"
// 弾---------------------------------------------------
#include "../Bullet/Player/PlayerBullet.h"
#include "../Bullet/Enemy/StraightBullet.h"
#include "../Bullet/Enemy/HomingBullet.h"
#include "../Bullet/Enemy/EnemyBullet.h"
// アイテム---------------------------------------------
#include "../Item/ScoreItem.h"
#include "../Item/PowerUpItem.h"

namespace inl {

	// プレイヤーの弾とノーマルエネミー----------------------------------------------------------------------
	bool Collision::CheckCollision_PlayerBulletAndEnemyZako(
		const Shared<inl::PlayerBullet>& bullet,
		const Shared<EnemyZakoBase>& enemy)
	{
		if (!enemy->_mesh) return false;

		if (tnl::IsIntersectAABB(
			bullet->_mesh->pos_, 
			bullet->_collisionSize,
			enemy->_mesh->pos_, 
			enemy->_collideSize))
		{
			return true;
		}

		return false;
	}


	// プレイヤーの弾とボスエネミー------------------------------------------------------------------------
	bool Collision::CheckCollision_PlayerBulletAndEnemyBoss(
		const Shared<inl::PlayerBullet>& bullet, 
		const Shared<EnemyBossBase>& enemy) 
	{
		if (!enemy->_mesh) return false;

		if (tnl::IsIntersectAABB(
			bullet->_mesh->pos_, 
			bullet->_collisionSize, 
			enemy->_mesh->pos_, 
			enemy->_collideSize))
		{
			return true;
		}

		return false;
	}


	// 敵の直行弾とプレイヤー--------------------------------------------------------------------------------
	bool Collision::CheckCollision_EnemyStraightBulletAndPlayer(
		const Shared<StraightBullet>& bullet,
		const Shared<Player>& player)
	{
		if (tnl::IsIntersectAABB(
			bullet->_mesh->pos_, 
			bullet->_collisionSize, 
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_collideSize))
		{
			return true;
		}

		return false;
	}


	// ------------------------------------------------------------------------------------------------
	bool Collision::CheckCollision_EnemyHomingBulletAndPlayer(
		const Shared<HomingBullet>& bullet, 
		const Shared<Player>& player) 
	{
		if (tnl::IsIntersectAABB(
			bullet->_mesh->pos_, 
			bullet->_collisionSize, 
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_collideSize))
		{
			return true;
		}

		return false;
	}


	// 弾幕とプレイヤー----------------------------------------------------------------------------------
	bool Collision::CheckCollision_BulletHellBulletsAndPlayer(
		const Shared<EnemyBullet>& bullet,
		const Shared<Player>& player)
	{
		if (tnl::IsIntersectAABB(bullet->_mesh->pos_,
			bullet->_collisionSize,
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_collideSize))
		{
			return true;
		}

		return false;
	}

	// プレイヤーと通常エネミー----------------------------------------------------------------------------

	bool Collision::CheckCollision_PlayerAndEnemyZako(
		Shared<Player>& player, 
		Shared<EnemyZakoBase>& enemy,
		const tnl::Vector3 prevPos_player,
		const tnl::Vector3 prevPos_enemy)
	{
		if (!enemy->_mesh) return false;

		if (tnl::IsIntersectAABB(
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_mesh->scl_,
			enemy->_mesh->pos_, 
			enemy->_mesh->scl_)) 
		{

			tnl::CorrectPositionAABB(
				prevPos_player, prevPos_enemy,
				ScenePlay::GetInstance()->_player->_mesh->scl_,
				enemy->_mesh->scl_,
				ScenePlay::GetInstance()->_player->_mesh->pos_,
				enemy->_mesh->pos_,
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH,
				tnl::eCorrTypeAABB::BOTH,
				10.0f
			);

			return true;
		}

		return false;
	}


	// プレイヤーとボス----------------------------------------------------------------------------------------
	bool Collision::CheckCollision_PlayerAndEnemyBoss(
		Shared<Player>& player, 
		Shared<EnemyBossBase>& enemy, 
		const tnl::Vector3 prevPos_player,
		const tnl::Vector3 prevPos_enemy) 
	{
		if (!enemy->_mesh) return false;

		if (tnl::IsIntersectAABB(
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_mesh->scl_,
			enemy->_mesh->pos_, 
			enemy->_mesh->scl_))
		{

			tnl::CorrectPositionAABB(
				prevPos_player, prevPos_enemy,
				ScenePlay::GetInstance()->_player->_mesh->scl_,
				enemy->_mesh->scl_,
				ScenePlay::GetInstance()->_player->_mesh->pos_,
				enemy->_mesh->pos_,
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH,
				tnl::eCorrTypeAABB::BOTH,
				20.0f
			);

			return true;
		}

		return false;
	}


	// 敵と敵------------------------------------------------------------------------------------------------
	void Collision::CheckCollision_EnemyAndEnemy(
		Shared<EnemyZakoBase>& enemy_type1,
		Shared<EnemyZakoBase>& enemy_type2, 
		const tnl::Vector3 prevPos_enemy1,
		const tnl::Vector3 prevPos_enemy2) 
	{
		if (!enemy_type1->_mesh || !enemy_type2->_mesh) return;

		tnl::Vector3 prevChord_e1 = prevPos_enemy1;
		tnl::Vector3 prevChord_e2 = prevPos_enemy2;

		if (tnl::IsIntersectAABB(
			enemy_type1->_mesh->pos_,
			enemy_type1->_collideSize,
			enemy_type2->_mesh->pos_,
			enemy_type2->_collideSize))
		{

			tnl::CorrectPositionAABB(
				prevChord_e1, prevChord_e2,
				enemy_type1->_mesh->scl_,
				enemy_type2->_mesh->scl_,
				enemy_type1->_mesh->pos_,
				enemy_type2->_mesh->pos_,
				tnl::eCorrTypeAABB::BOTH,
				tnl::eCorrTypeAABB::BOTH,
				tnl::eCorrTypeAABB::BOTH,
				30.0f
			);
		}
	}

	// プレイヤーと得点アイテム---------------------------------------------------------------------------------
	bool Collision::CheckCollision_PlayerAndScoreItem(
		const Shared<inl::ScoreItem>& scoreItem,
		const Shared<Player>& player)
	{
		if (tnl::IsIntersectAABB(scoreItem->_mesh->pos_,
			scoreItem->_collisionSize,
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_mesh->scl_))
		{
			return true;
		}

		return false;
	}

	// 得点アイテムと得点アイテム-------------------------------------------------------------------------------
	void Collision::CheckCollision_ScoreItemAndScoreItem(
		Shared<inl::ScoreItem>& scoreItem_1,
		Shared<inl::ScoreItem>& scoreItem_2,
		const tnl::Vector3 prevPos_scoreItem1,
		const tnl::Vector3 prevPos_scoreItem2) 
	{
		if (!scoreItem_1->_mesh || !scoreItem_2->_mesh) return;

		if (tnl::IsIntersectAABB(scoreItem_1->_mesh->pos_,
			scoreItem_1->_collisionSize,
			scoreItem_2->_mesh->pos_, 
			scoreItem_2->_collisionSize))
		{

			tnl::CorrectPositionAABB(
				prevPos_scoreItem1, prevPos_scoreItem2,
				scoreItem_1->_mesh->scl_,
				scoreItem_2->_mesh->scl_,
				scoreItem_1->_mesh->pos_,
				scoreItem_2->_mesh->pos_,
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH,
				50.0f
			);
		}
	}

	// プレイヤーとプレイヤー強化アイテム----------------------------------------------------------------------------
	bool Collision::CheckCollision_PlayerAndPowerUpItem(
		const Shared<inl::PowerUpItem>& powerUpItem,
		const Shared<Player>& player) 
	{
		if (tnl::IsIntersectAABB(powerUpItem->_mesh->pos_,
			powerUpItem->_collisionSize,
			ScenePlay::GetInstance()->_player->_mesh->pos_,
			ScenePlay::GetInstance()->_player->_mesh->scl_))
		{
			return true;
		}

		return false;
	}
	// プレイヤー強化アイテムとプレイヤー強化アイテム----------------------------------------------------------------
	void Collision::CheckCollision_PowerUpItemAndPowerUpItem(
		Shared<inl::PowerUpItem>& powerUpItem_1,
		Shared<inl::PowerUpItem>& powerUpItem_2, 
		const tnl::Vector3 prevPos_powerUpItem1,
		const tnl::Vector3 prevPos_powerUpItem2) 
	{
		if (!powerUpItem_1->_mesh || !powerUpItem_2->_mesh) return;

		if (tnl::IsIntersectAABB(
			powerUpItem_1->_mesh->pos_,
			powerUpItem_1->_collisionSize,
			powerUpItem_2->_mesh->pos_,
			powerUpItem_2->_collisionSize)) 
		{

			tnl::CorrectPositionAABB(
				prevPos_powerUpItem1,
				prevPos_powerUpItem2,
				powerUpItem_1->_mesh->scl_,
				powerUpItem_2->_mesh->scl_,
				powerUpItem_1->_mesh->pos_,
				powerUpItem_2->_mesh->pos_,
				tnl::eCorrTypeAABB::BOTH,
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH,
				50.0f
			);
		}
	}


	// 得点アイテムとプレイヤー強化アイテム------------------------------------------------------------------------
	void Collision::CheckCollision_ScoreItemAndPowerUpItem(
		Shared<inl::ScoreItem>& scoreItem, 
		Shared<inl::PowerUpItem>& powerUpItem,
		const tnl::Vector3 prevPos_scoreItem, 
		const tnl::Vector3 prevPos_powerUpItem)
	{
		if (!scoreItem->_mesh || !powerUpItem->_mesh) return;

		if (tnl::IsIntersectAABB(
			scoreItem->_mesh->pos_,
			scoreItem->_collisionSize, 
			powerUpItem->_mesh->pos_,
			powerUpItem->_collisionSize)) 
		{

			tnl::CorrectPositionAABB(
				prevPos_scoreItem,
				prevPos_powerUpItem,
				scoreItem->_mesh->scl_,
				powerUpItem->_mesh->scl_,
				scoreItem->_mesh->pos_,
				powerUpItem->_mesh->pos_,
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH, 
				tnl::eCorrTypeAABB::BOTH,
				50.0f
			);
		}
	}
}