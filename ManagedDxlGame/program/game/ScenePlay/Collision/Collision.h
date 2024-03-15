#pragma once


namespace inl {

	class Player;
	class ScoreItem;
	class PowerUpItem;
	class PlayerBullet;
	class StraightBullet;
	class HomingBullet;
	class EnemyBullet;
	class EnemyZakoBase;
	class EnemyBossBase;


	class Collision
	{
	public:

		// キャラクター同士
		bool CheckCollision_PlayerAndEnemyZako(
			Shared<Player>& player,
			Shared<EnemyZakoBase>& enemy,
			const tnl::Vector3 prevPos_player,
			const tnl::Vector3 prevPos_enemy
		);
		bool CheckCollision_PlayerAndEnemyBoss(
			Shared<Player>& player,
			Shared<EnemyBossBase>& enemy,
			const tnl::Vector3 prevPos_player,
			const tnl::Vector3 prevPos_enemy
		);
		void CheckCollision_EnemyAndEnemy(
			Shared<EnemyZakoBase>& enemy_type1,
			Shared<EnemyZakoBase>& enemy_type2,
			const tnl::Vector3 prevPos_enemy1,
			const tnl::Vector3 prevPos_enemy2
		);


		// キャラクターと弾
		bool CheckCollision_PlayerBulletAndEnemyZako(
			const Shared<inl::PlayerBullet>& meshA, const Shared<EnemyZakoBase>& meshB
		);
		bool CheckCollision_PlayerBulletAndEnemyBoss(
			const Shared<inl::PlayerBullet>& meshA, const Shared<EnemyBossBase>& meshB
		);
		bool CheckCollision_EnemyStraightBulletAndPlayer(
			const Shared<StraightBullet>& bullet, const Shared<Player>& player
		);
		bool CheckCollision_EnemyHomingBulletAndPlayer(
			const Shared<HomingBullet>& bullet, const Shared<Player>& player
		);
		bool CheckCollision_BulletHellBulletsAndPlayer(
			const Shared<EnemyBullet>& bullet, const Shared<Player>& player
		);


		// プレイヤーと得点アイテム
		bool CheckCollision_PlayerAndScoreItem(
			const Shared<inl::ScoreItem>& scoreItem, const Shared<Player>& player
		);


		// 得点アイテムと得点アイテム
		void CheckCollision_ScoreItemAndScoreItem(
			Shared<inl::ScoreItem>& scoreItem_1,
			Shared<inl::ScoreItem>& scoreItem_2,
			const tnl::Vector3 prevPos_scoreItem1,
			const tnl::Vector3 prevPos_scoreItem2
		);


		// プレイヤーとプレイヤー強化アイテム
		bool CheckCollision_PlayerAndPowerUpItem(
			const Shared<inl::PowerUpItem>& powerUpItem, const Shared<Player>& player
		);


		// プレイヤー強化アイテムとプレイヤー強化アイテム
		void CheckCollision_PowerUpItemAndPowerUpItem(
			Shared<inl::PowerUpItem>& powerUpItem_1,
			Shared<inl::PowerUpItem>& powerUpItem_2,
			const tnl::Vector3 prevPos_powerUpItem1,
			const tnl::Vector3 prevPos_powerUpItem2
		);


		// 得点アイテムとプレイヤー強化アイテム
		void CheckCollision_ScoreItemAndPowerUpItem(
			Shared<inl::ScoreItem>& scoreItem,
			Shared<inl::PowerUpItem>& powerUpItem,
			const tnl::Vector3 prevPos_scoreItem,
			const tnl::Vector3 prevPos_powerUpItem
		);
	};
}