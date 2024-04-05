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

		// キャラクター同士----------------------------------------------------------------------------------
		bool CheckCollision_PlayerAndEnemyZako(              // プレイヤーと通常エネミー
			Shared<Player>& player,
			Shared<EnemyZakoBase>& enemy,
			const tnl::Vector3 prevPos_player,
			const tnl::Vector3 prevPos_enemy
		);

		bool CheckCollision_PlayerAndEnemyBoss(              // プレイヤーとボスエネミー
			Shared<Player>& player,
			Shared<EnemyBossBase>& enemy,
			const tnl::Vector3 prevPos_player,
			const tnl::Vector3 prevPos_enemy
		);

		void CheckCollision_EnemyAndEnemy(                   // 通常エネミーと通常エネミー
			Shared<EnemyZakoBase>& enemy_type1,
			Shared<EnemyZakoBase>& enemy_type2,
			const tnl::Vector3 prevPos_enemy1,
			const tnl::Vector3 prevPos_enemy2
		);


		// キャラクターと弾----------------------------------------------------------------------------------
		bool CheckCollision_PlayerBulletAndEnemyZako(        // プレイヤーの弾と通常エネミー
			const Shared<inl::PlayerBullet>& meshA, 
			const Shared<EnemyZakoBase>& meshB
		);

		bool CheckCollision_PlayerBulletAndEnemyBoss(        // プレイヤーの弾とボスエネミー
			const Shared<inl::PlayerBullet>& meshA,
			const Shared<EnemyBossBase>& meshB
		);

		bool CheckCollision_EnemyStraightBulletAndPlayer(    // 通常エネミーの直行弾とプレイヤー
			const Shared<StraightBullet>& bullet, 
			const Shared<Player>& player
		);

		bool CheckCollision_EnemyHomingBulletAndPlayer(      // 通常エネミーの追尾弾とプレイヤー
			
			const Shared<HomingBullet>& bullet,
			const Shared<Player>& player
		);

		bool CheckCollision_BulletHellBulletsAndPlayer(      // 弾幕とプレイヤー
			const Shared<EnemyBullet>& bullet,
			const Shared<Player>& player
		);


		// プレイヤーと得点アイテム----------------------------------------------------------------------------------
		bool CheckCollision_PlayerAndScoreItem(              // プレイヤーとスコアアイテム
			const Shared<inl::ScoreItem>& scoreItem,
			const Shared<Player>& player
		);

		// 得点アイテムと得点アイテム--------------------------------------------------------------------------------
		void CheckCollision_ScoreItemAndScoreItem(           // スコアアイテムとスコアアイテム
			Shared<inl::ScoreItem>& scoreItem_1,
			Shared<inl::ScoreItem>& scoreItem_2,
			const tnl::Vector3 prevPos_scoreItem1,
			const tnl::Vector3 prevPos_scoreItem2
		);

		// プレイヤーとプレイヤー強化アイテム------------------------------------------------------------------------
		bool CheckCollision_PlayerAndPowerUpItem(            // プレイヤーとパワーアップアイテム
			const Shared<inl::PowerUpItem>& powerUpItem,
			const Shared<Player>& player
		);

		// プレイヤー強化アイテムとプレイヤー強化アイテム------------------------------------------------------------
		void CheckCollision_PowerUpItemAndPowerUpItem(       // パワーアップアイテムとパワーアップアイテム
			Shared<inl::PowerUpItem>& powerUpItem_1,
			Shared<inl::PowerUpItem>& powerUpItem_2,
			const tnl::Vector3 prevPos_powerUpItem1,
			const tnl::Vector3 prevPos_powerUpItem2
		);

		// 得点アイテムとプレイヤー強化アイテム----------------------------------------------------------------------
		void CheckCollision_ScoreItemAndPowerUpItem(         // スコアアイテムとパワーアップアイテム
			Shared<inl::ScoreItem>& scoreItem,
			Shared<inl::PowerUpItem>& powerUpItem,
			const tnl::Vector3 prevPos_scoreItem,
			const tnl::Vector3 prevPos_powerUpItem
		);
	};
}