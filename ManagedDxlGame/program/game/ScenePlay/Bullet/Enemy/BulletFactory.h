#pragma once

class StraightBullet;
class HomingBullet;


class BulletFactory
{
public:

	BulletFactory(){}
	explicit BulletFactory(const Shared<dxe::Mesh>& mesh) { _enemyMesh_ref = mesh; }

	// �e������------------------------------------------------------------------------------------
	std::list<Shared<StraightBullet>> CreateStraightBullet(
		const StraightBullet::USER user, const int maxBulletSpawnCount);

	std::list<Shared<HomingBullet>> CreateHomingBullet(
		const HomingBullet::USER user, const int maxBulletSpawnCount);

private:

	/*
		���󉺋L�̊֐��͏������S�������ł����A
	�@�@����͓G���ƂɈقȂ�v�f��ǉ��������Ǝv���Ă��邽�߁A�֐��͓G���Ƃɕ����Ă��܂�
	*/

	//�@���s�e--------------------------------------------------------------------------------------------------
	std::list<Shared<StraightBullet>> CreateStraightBullet_ZakoBox(const int maxBulletSpawnCount);
	std::list<Shared<StraightBullet>> CreateStraightBullet_ZakoDome(const int maxBulletSpawnCount);
	std::list<Shared<StraightBullet>> CreateStraightBullet_ZakoCylinder(const int maxBulletSpawnCount);

	//�@�ǔ��e--------------------------------------------------------------------------------------------------
	std::list<Shared<HomingBullet>> CreateHomingBullet_ZakoBox(const int maxBulletSpawnCount);
	std::list<Shared<HomingBullet>> CreateHomingBullet_ZakoDome(const int maxBulletSpawnCount);
	std::list<Shared<HomingBullet>> CreateHomingBullet_ZakoCylinder(const int maxBulletSpawnCount);

private:

	Shared<dxe::Mesh> _enemyMesh_ref = nullptr;
};