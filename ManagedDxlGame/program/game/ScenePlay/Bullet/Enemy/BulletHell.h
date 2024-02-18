#pragma once
#include <random>
#include "EnemyBullet.h"

class BulletHellFactory;
class ScenePlay;
class Player;

// �e�̐����A�e���̃p�^�[���A�e�̍X�V�ƍ폜�Ȃǂ��s�� (EnemyBoss����)
// �{�X�͒ʏ�U���A�X�y���J�[�h1�A�X�y���J�[�h2�̌v�R��ނ̒e�������

class BulletHell : public EnemyBullet
{
public:

	enum class TYPE {
		// �p�`�����[
		Normal_Patchouli,
		MetalFatigue_Patchouli,
		SilentSerena_Patchouli,
		// �`���m
		Normal_Cirno,
		IcicleFall_Cirno,
		Perfect_Freeze_Cirno,
		// �z�K�q
		Normal_Suwako,
		IronRingOfMoriya_Suwako,
		KeroChan_StandsFirm_AgainstTheStorm_Suwako,
	};

public:

	BulletHell() {}
	BulletHell(const Shared<dxe::Mesh>& bossMesh, const Shared<Player>& player) : _bossMesh_ref(bossMesh) {
		_player_ref = player;
	}

	// 1. �@�\
	// 2. �X�y���i�e�����j
	// 3. �g�p�ҁi�L�������j

	// �X�e�[�W1�{�X�i�p�`�����[�j-------------------------------------
	void ShotBulletHell_Normal_Patchouli(const float& delta_time);
	void ShotBulletHell_MetalFatigue_Patchouli(const float& delta_time);
	void ShotBulletHell_SilentSerena_Patchouli(const float& delta_time);
	// �X�e�[�W2�{�X�i�`���m�j---------------------------------------------
	void ShotBulletHell_Normal_Cirno(const float& delta_time);
	void ShotBulletHell_IcicleFall_Cirno(const float& delta_time);
	void ShotBulletHell_PerfectFreeze_Cirno(const float& delta_time);
	// �X�e�[�W3�{�X�i�z�K�q�j---------------------------------------------
	void ShotBulletHell_Normal_Suwako(const float& delta_time);
	void ShotBulletHell_IronRingOfMoriya_Suwako(const float& delta_time);
	void ShotBulletHell_KeroChanStandsFirm_AgainstTheStorm_Suwako(const float& delta_time);
	// ---------------------------------------------

private:

	// �X�e�[�W1�{�X�i�p�`�����[�j-------------------------------------

	void Wave_MetalFatigue_Patchouli(
		Shared<EnemyBullet> bullet, 
		tnl::Vector3& bossPosition, 
		const float& delta_time, 
		float angle_origin, 
		float radius_origin, 
		float startMoveTime);

	// �X�e�[�W3�{�X�i�z�K�q�j---------------------------------------------
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