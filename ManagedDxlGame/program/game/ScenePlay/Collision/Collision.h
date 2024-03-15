#pragma once

class Player;
class PlayerBullet;
class StraightBullet;
class HomingBullet;
class EnemyBullet;
class EnemyZakoBase;
class EnemyBossBase;
class EnemyZakoBox;
class Score;
class ScoreItem;
class PowerUpItem;

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
		const Shared<PlayerBullet>& meshA, const Shared<EnemyZakoBase>& meshB
	);
	bool CheckCollision_PlayerBulletAndEnemyBoss(
		const Shared<PlayerBullet>& meshA, const Shared<EnemyBossBase>& meshB
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
		const Shared<ScoreItem>& scoreItem, const Shared<Player>& player
	);


	// 得点アイテムと得点アイテム
	void CheckCollision_ScoreItemAndScoreItem(
		Shared<ScoreItem>& scoreItem_1,
		Shared<ScoreItem>& scoreItem_2,
		const tnl::Vector3 prevPos_scoreItem1,
		const tnl::Vector3 prevPos_scoreItem2
	);


	// プレイヤーとプレイヤー強化アイテム
	bool CheckCollision_PlayerAndPowerUpItem(
		const Shared<PowerUpItem>& powerUpItem, const Shared<Player>& player
	);


	// プレイヤー強化アイテムとプレイヤー強化アイテム
	void CheckCollision_PowerUpItemAndPowerUpItem(
		Shared<PowerUpItem>& powerUpItem_1,
		Shared<PowerUpItem>& powerUpItem_2,
		const tnl::Vector3 prevPos_powerUpItem1,
		const tnl::Vector3 prevPos_powerUpItem2
	);


	// 得点アイテムとプレイヤー強化アイテム
	void CheckCollision_ScoreItemAndPowerUpItem(
		Shared<ScoreItem>& scoreItem,
		Shared<PowerUpItem>& powerUpItem,
		const tnl::Vector3 prevPos_scoreItem,
		const tnl::Vector3 prevPos_powerUpItem
	);
};