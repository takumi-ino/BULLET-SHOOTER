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
	BulletHell(const Shared<dxe::Mesh>& bossMesh, const Shared<Player>& player) : _bossMesh_ref(bossMesh) {
		_player_ref = player;
	}

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
	// ---------------------------------------------

private:

	// ステージ1ボス（パチュリー）-------------------------------------

	void Wave_MetalFatigue_Patchouli(
		Shared<EnemyBullet> bullet, 
		tnl::Vector3& bossPosition, 
		const float& delta_time, 
		float angle_origin, 
		float radius_origin, 
		float startMoveTime);

	// ステージ3ボス（諏訪子）---------------------------------------------
	void Wave_IronRingOfMoriya_Suwako(
		Shared<EnemyBullet> bullet,
		float circle_radius,
		float angle,
		float delta_time, 
		float bullet_speed, 
		float startMove_time);

private:

	const Shared<dxe::Mesh> _bossMesh_ref = nullptr;

private:

	std::random_device rd;
};