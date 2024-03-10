#pragma once
#include <random>
#include "EnemyBullet.h"

class BulletHellFactory;
class ScenePlay;
class Player;

// 弾の生成、弾幕のパターン、弾の更新と削除などを行う (EnemyBoss限定)
// ボスは通常攻撃、スペルカード1、スペルカード2の計３種類の弾幕を放つ

class BulletHell : public EnemyBullet
{
public:

	enum class TYPE {
		// パチュリー
		Normal_Patchouli,
		MetalFatigue_Patchouli,
		SilentSerena_Patchouli,
		// チルノ
		Normal_Cirno,
		IcicleFall_Cirno,
		Perfect_Freeze_Cirno,
		// 諏訪子
		Normal_Suwako,
		IronRingOfMoriya_Suwako,
		KeroChan_StandsFirm_AgainstTheStorm_Suwako,
	};

public:

	BulletHell() {}
	BulletHell(const Shared<dxe::Mesh>& bossMesh, const Shared<Player>& player)
		: _bossMesh_ref(bossMesh) {	_player_ref = player; }


	// 1. 機能
	// 2. スペル（弾幕名）
	// 3. 使用者（キャラ名）

	// ステージ1ボス（パチュリー）-------------------------------------
	void ShotBulletHell_Normal_Patchouli(const float& delta_time);
	void ShotBulletHell_MetalFatigue_Patchouli(const float& delta_time);
	void ShotBulletHell_SilentSerena_Patchouli(const float& delta_time);

	// ステージ2ボス（チルノ）---------------------------------------------
	void ShotBulletHell_Normal_Cirno(const float& delta_time);
	void ShotBulletHell_IcicleFall_Cirno(const float& delta_time);
	void ShotBulletHell_PerfectFreeze_Cirno(const float& delta_time);

	// ステージ3ボス（諏訪子）---------------------------------------------
	void ShotBulletHell_Normal_Suwako(const float& delta_time);
	void ShotBulletHell_IronRingOfMoriya_Suwako(const float& delta_time);
	void ShotBulletHell_KeroChanStandsFirm_AgainstTheStorm_Suwako(const float& delta_time);

private:

	// 似た処理が多い場合に補助関数を使用（ WaveAssist ）など

	// ステージ1ボス（パチュリー）-------------------------------------
	void WaveAssist_MetalFatigue_Patchouli(
		Shared<EnemyBullet>& bullet,
		const float& delta_time,
		float angle_origin,
		float radius_origin,
		float startMoveTime
	);

	// ステージ2ボス（チルノ）-------------------------------------
	void WaveAssist_Normal_Cirno(
		Shared<EnemyBullet>& bullet,
		float timing, 
		float delayOffset,
		const float& delta_time
	);

	void InitAssist_IcicleFall_StraightBlue_Cirno(
		Shared<EnemyBullet>& bullet,
		const float radius, 
		const float angle
	);

	void UpdateAssist_IcicleFall_StraightBlue_Cirno(
		Shared<EnemyBullet>& bullet,
		float radius, 
		float newAngle,
		float startChangeDirTime,
		const float& delta_time
	);

	void InitAssist_IcicleFall_StraightYellow_Cirno(
		Shared<EnemyBullet>& bullet,
		const int BULLETS_PER_ROW, 
		const float BULLET_SPACING
	);

	void UpdateAssist_IcicleFall_StraightYellow_Cirno(
		Shared<EnemyBullet>& bullet,
		const float& delta_time,
		const float BULLET_SPEED
	);

	// ステージ3ボス（諏訪子）---------------------------------------------
	void UpdateAssist_Normal_Suwako(
		Shared<EnemyBullet>& bullet,
		const float timeLimit
	);

	void WaveAssist_IronRingOfMoriya_Suwako(
		Shared<EnemyBullet>& bullet,
		const float circle_radius,
		const float angle,
		const float delta_time,
		const float bullet_speed,
		const int startMove_time
	);

	void InitAssist_KeroChanStandsFirm_AgainstTheStorm_Suwako(
		tnl::Vector3& upward_velocity,
		Shared<EnemyBullet>& bullet
	);

	void UpdateAssist_KeroChanStandsFirm_AgainstTheStorm_Suwako(
		Shared<EnemyBullet>& bullet,
		const float& delta_time
	);

	// 共通機能-----------------------------------------------------------------------------
	tnl::Vector3 GenerateRandomVector(  // ランダムベクター生成
		std::mt19937& mt,
		const int minX,
		const int maxX,
		const int minY,
		const int maxY,
		const int minZ,
		const int maxZ)
	{
		std::uniform_int_distribution<int> rnd_valX(minX, maxX);
		std::uniform_int_distribution<int> rnd_valY(minY, maxY);
		std::uniform_int_distribution<int> rnd_valZ(minZ, maxZ);

		return tnl::Vector3{
			static_cast<float>(rnd_valX(mt)),
			static_cast<float>(rnd_valY(mt)),
			static_cast<float>(rnd_valZ(mt))
		};
	}

private:

	const Shared<dxe::Mesh> _bossMesh_ref = nullptr;

private:

	std::random_device rd;
};