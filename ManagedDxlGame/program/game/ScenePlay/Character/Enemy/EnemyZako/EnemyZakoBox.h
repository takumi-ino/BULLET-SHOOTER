#pragma once
#include "../EnemyBase.h"
#include "../../../Bullet/Enemy/EnemyBullet.h"
#include "../../../../Loader/CsvLoader.h"
#include "../EnemyZakoBase.h"

class EnemyZakoBase;
class StraightBullet;
class HomingBullet;
class BulletFactory;

class EnemyZakoBox : public EnemyZakoBase
{
public:

	EnemyZakoBox() {}

	EnemyZakoBox(const EnemyZakoInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera, const Shared<Collision>& collision);

	void InitBulletFactoryInstance() override;
	void SetMeshInfo() override;
};