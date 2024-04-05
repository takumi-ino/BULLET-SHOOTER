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

		// �L�����N�^�[���m----------------------------------------------------------------------------------
		bool CheckCollision_PlayerAndEnemyZako(              // �v���C���[�ƒʏ�G�l�~�[
			Shared<Player>& player,
			Shared<EnemyZakoBase>& enemy,
			const tnl::Vector3 prevPos_player,
			const tnl::Vector3 prevPos_enemy
		);

		bool CheckCollision_PlayerAndEnemyBoss(              // �v���C���[�ƃ{�X�G�l�~�[
			Shared<Player>& player,
			Shared<EnemyBossBase>& enemy,
			const tnl::Vector3 prevPos_player,
			const tnl::Vector3 prevPos_enemy
		);

		void CheckCollision_EnemyAndEnemy(                   // �ʏ�G�l�~�[�ƒʏ�G�l�~�[
			Shared<EnemyZakoBase>& enemy_type1,
			Shared<EnemyZakoBase>& enemy_type2,
			const tnl::Vector3 prevPos_enemy1,
			const tnl::Vector3 prevPos_enemy2
		);


		// �L�����N�^�[�ƒe----------------------------------------------------------------------------------
		bool CheckCollision_PlayerBulletAndEnemyZako(        // �v���C���[�̒e�ƒʏ�G�l�~�[
			const Shared<inl::PlayerBullet>& meshA, 
			const Shared<EnemyZakoBase>& meshB
		);

		bool CheckCollision_PlayerBulletAndEnemyBoss(        // �v���C���[�̒e�ƃ{�X�G�l�~�[
			const Shared<inl::PlayerBullet>& meshA,
			const Shared<EnemyBossBase>& meshB
		);

		bool CheckCollision_EnemyStraightBulletAndPlayer(    // �ʏ�G�l�~�[�̒��s�e�ƃv���C���[
			const Shared<StraightBullet>& bullet, 
			const Shared<Player>& player
		);

		bool CheckCollision_EnemyHomingBulletAndPlayer(      // �ʏ�G�l�~�[�̒ǔ��e�ƃv���C���[
			
			const Shared<HomingBullet>& bullet,
			const Shared<Player>& player
		);

		bool CheckCollision_BulletHellBulletsAndPlayer(      // �e���ƃv���C���[
			const Shared<EnemyBullet>& bullet,
			const Shared<Player>& player
		);


		// �v���C���[�Ɠ��_�A�C�e��----------------------------------------------------------------------------------
		bool CheckCollision_PlayerAndScoreItem(              // �v���C���[�ƃX�R�A�A�C�e��
			const Shared<inl::ScoreItem>& scoreItem,
			const Shared<Player>& player
		);

		// ���_�A�C�e���Ɠ��_�A�C�e��--------------------------------------------------------------------------------
		void CheckCollision_ScoreItemAndScoreItem(           // �X�R�A�A�C�e���ƃX�R�A�A�C�e��
			Shared<inl::ScoreItem>& scoreItem_1,
			Shared<inl::ScoreItem>& scoreItem_2,
			const tnl::Vector3 prevPos_scoreItem1,
			const tnl::Vector3 prevPos_scoreItem2
		);

		// �v���C���[�ƃv���C���[�����A�C�e��------------------------------------------------------------------------
		bool CheckCollision_PlayerAndPowerUpItem(            // �v���C���[�ƃp���[�A�b�v�A�C�e��
			const Shared<inl::PowerUpItem>& powerUpItem,
			const Shared<Player>& player
		);

		// �v���C���[�����A�C�e���ƃv���C���[�����A�C�e��------------------------------------------------------------
		void CheckCollision_PowerUpItemAndPowerUpItem(       // �p���[�A�b�v�A�C�e���ƃp���[�A�b�v�A�C�e��
			Shared<inl::PowerUpItem>& powerUpItem_1,
			Shared<inl::PowerUpItem>& powerUpItem_2,
			const tnl::Vector3 prevPos_powerUpItem1,
			const tnl::Vector3 prevPos_powerUpItem2
		);

		// ���_�A�C�e���ƃv���C���[�����A�C�e��----------------------------------------------------------------------
		void CheckCollision_ScoreItemAndPowerUpItem(         // �X�R�A�A�C�e���ƃp���[�A�b�v�A�C�e��
			Shared<inl::ScoreItem>& scoreItem,
			Shared<inl::PowerUpItem>& powerUpItem,
			const tnl::Vector3 prevPos_scoreItem,
			const tnl::Vector3 prevPos_powerUpItem
		);
	};
}