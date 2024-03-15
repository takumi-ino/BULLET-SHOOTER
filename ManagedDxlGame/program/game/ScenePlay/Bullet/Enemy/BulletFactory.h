#pragma once

class StraightBullet;
class HomingBullet;


class BulletFactory
{
public:

	BulletFactory(){}
	explicit BulletFactory(const Shared<dxe::Mesh>& mesh) { _enemyMesh_ref = mesh; }

	// 弾初期化------------------------------------------------------------------------------------
	std::list<Shared<StraightBullet>> CreateStraightBullet(
		const StraightBullet::USER user, const int maxBulletSpawnCount);

	std::list<Shared<HomingBullet>> CreateHomingBullet(
		const HomingBullet::USER user, const int maxBulletSpawnCount);

private:

	/*
		現状下記の関数は処理が全く同じですが、
	　　今後は敵ごとに異なる要素を追加したいと思っているため、関数は敵ごとに分けています
	*/

	//　直行弾--------------------------------------------------------------------------------------------------
	std::list<Shared<StraightBullet>> CreateStraightBullet_ZakoBox(const int maxBulletSpawnCount);
	std::list<Shared<StraightBullet>> CreateStraightBullet_ZakoDome(const int maxBulletSpawnCount);
	std::list<Shared<StraightBullet>> CreateStraightBullet_ZakoCylinder(const int maxBulletSpawnCount);

	//　追尾弾--------------------------------------------------------------------------------------------------
	std::list<Shared<HomingBullet>> CreateHomingBullet_ZakoBox(const int maxBulletSpawnCount);
	std::list<Shared<HomingBullet>> CreateHomingBullet_ZakoDome(const int maxBulletSpawnCount);
	std::list<Shared<HomingBullet>> CreateHomingBullet_ZakoCylinder(const int maxBulletSpawnCount);

private:

	Shared<dxe::Mesh> _enemyMesh_ref = nullptr;
};