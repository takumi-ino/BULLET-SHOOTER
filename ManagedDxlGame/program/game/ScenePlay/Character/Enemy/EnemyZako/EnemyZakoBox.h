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

	EnemyZakoBox(const EnemyZakoInfo& info, const Shared<Player>& player, const Shared<dxe::Camera>& camera);

	void InitBulletFactoryInstance() override;
	void SetMeshInfo() override;

	const std::list<Shared<StraightBullet>>& GetStraightBullets() override { return _straight_bullets; };

private:

	// ’eŒn----------------------------------------------------------------
	void ShotStraightBullet(const float& delta_time);
	void ReloadStraightBulletByTimer(float& reload_time_counter, const float& delta_time);
	void UpdateStraightBullet(const float delta_time) override;

	void ShotHomingBullet(const float& delta_time);
	void ReloadHomingBulletByTimer(float& reload_time_counter, const float& delta_time);
	void UpdateHomingBullet(const float delta_time) override;

	// ‚»‚êˆÈŠO----------------------------------------------------------------------

	void DoRoutineMoves(const float& delta_time) override;
	void ChasePlayer(const float delta_time) override;
	void AttackPlayer(const float& delta_time) override;

	void SetStraightBullets(
		const std::list<Shared<StraightBullet>>& bullets) { _straight_bullets = bullets; };


	bool Update(float delta_time) override;
	void Render(Shared<dxe::Camera> camera) override;

public:

	std::list<Shared<StraightBullet>>      _straight_bullets;
	std::list<Shared<HomingBullet>>        _homing_bullets;

private:

	Shared<BulletFactory> _bulletFactory = nullptr;

	std::deque<Shared<StraightBullet>>_straightBullet_queue;
	std::deque<Shared<HomingBullet>>_homingBullet_queue;

public:

	static int   _at;
	static int   _def;

private:

	int straight_bullet_count;
	int homing_bullet_count;

	bool _isShotStraightBullet = true;
	bool _isShotHomingBullet = false;

	const float _IDLE_DISTANCE = 500.0f;
	const float _ATTACK_DISTANCE = 450.0f;
};