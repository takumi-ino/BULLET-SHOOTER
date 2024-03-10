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
	BulletHell(const Shared<dxe::Mesh>& bossMesh, const Shared<Player>& player)
		: _bossMesh_ref(bossMesh) {	_player_ref = player; }


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

private:

	// ���������������ꍇ�ɕ⏕�֐����g�p�i WaveAssist �j�Ȃ�

	// �X�e�[�W1�{�X�i�p�`�����[�j-------------------------------------
	void WaveAssist_MetalFatigue_Patchouli(
		Shared<EnemyBullet>& bullet,
		const float& delta_time,
		float angle_origin,
		float radius_origin,
		float startMoveTime
	);

	// �X�e�[�W2�{�X�i�`���m�j-------------------------------------
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

	// �X�e�[�W3�{�X�i�z�K�q�j---------------------------------------------
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

	// ���ʋ@�\-----------------------------------------------------------------------------
	tnl::Vector3 GenerateRandomVector(  // �����_���x�N�^�[����
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