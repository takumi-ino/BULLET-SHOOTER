#pragma once
#include "../DxLibEngine.h"
#include "../../library/tnl_sequence.h"
#include "../Manager/Scene/SceneBase.h"

class Player;
class SkyBox;
class HeightMap;
class Collision;
class ItemManager;
class EnemyManager;
class Score;
class PauseMenu;
class FreeLookCamera;
class EnemyBullet;
class BulletHellFactory;
class StraightBullet;
class HomingBullet;

struct SpawnedBossBulletInfo;

class ScenePlay : public SceneBase
{
public:

	ScenePlay() {}
	ScenePlay(const std::string selected_difficulty, const int stage);

	void MoveToNextStage(const int stage, const std::string difficulty);

	// �v���C���[�̃{�����ʂɂ��e�̖������E�L����
	static void ReactivateEnemyBullets();
	static void DeactivateAllEnemyBullets();

	void CheckDoInit_FirstStageBulletHellLists();   // 1
	void CheckDoInit_SecondStageBulletHellLists();  // 2
	void CheckDoInit_ThirdStageBulletHellLists();   // 3

	void DestroyFirstStageBulletHellLists();
	void DestroySecondStageBulletHellLists();
	static void DestroyThirdStageBulletHellLists();

	static void TurnOff_FirstStageBulletHellLists();
	static void TurnOff_SecondStageBulletHellLists();
	static void TurnOff_ThirdStageBulletHellLists();


	static float GetDeltaTime() { return _deltaTime_ref; };

private:

	void SetDeltaTime(const float deltaTime) { _deltaTime_ref = deltaTime; };


	// 1
	void CheckDoRender_FirstStageBulletHellLists();
	void CheckDoUpdate_FirstStageBulletHellLists();
	// 2
	void CheckDoRender_SecondStageBulletHellLists();
	void CheckDoUpdate_SecondStageBulletHellLists();
	// 3
	void CheckDoRender_ThirdStageBulletHellLists();
	void CheckDoUpdate_ThirdStageBulletHellLists();

	void RenderBeginText();
	void UpdateShowBeginTextTimer(float deltaTime);
	void InitPlayersBombCount(const std::string& selected_difficulty);
	void RenderEnemyRadarOnMiniMap();
	void RenderPauseMenu();

	void Render() override;
	void Update(const float deltaTime) override;


public:

	static Shared<dxe::Particle> _weather_particle;

private:

	Shared<FreeLookCamera>    _mainCamera = nullptr;

	Shared<Player>            _player = nullptr;

	Shared<EnemyManager>      _enemyManager = nullptr;

	Shared<SkyBox>            _skyBox = nullptr;

	Shared<Collision>         _collision = nullptr;

	Shared<Score>             _score = nullptr;

	Shared<dxe::ScreenEffect> _screen_effect = nullptr;

	Shared<BulletHellFactory> _bltHellFactory = nullptr;

	Shared<PauseMenu>         _pauseMenu = nullptr;

public:

	// �X�e�[�W1
	static std::vector<Shared<EnemyBullet>> _bullet_normal_patchouli;
	static std::vector<Shared<EnemyBullet>> _bullet_metalFatigue_patchouli;
	static std::vector<Shared<EnemyBullet>> _bullet_silentSerena_patchouli;

	// �X�e�[�W2
	static std::vector<Shared<EnemyBullet>> _bullet_normal_cirno;
	static std::vector<Shared<EnemyBullet>> _bullet_icicleFall_cirno;
	static std::vector<Shared<EnemyBullet>> _bullet_perfectFreeze_cirno;

	// �X�e�[�W3
	static std::vector<Shared<EnemyBullet>> _bullet_normal_suwako;
	static std::vector<Shared<EnemyBullet>> _bullet_ironRingOfMoriya_suwako;
	static std::vector<Shared<EnemyBullet>> _bullet_keroChanStandsFirmAgainstTheStorm_suwako;

public:

	// �X�e�[�W1
	static bool _isUsingBullet_normal_patchouli;
	static bool _isUsingBullet_metalFatigue_patchouli;
	static bool _isUsingBullet_silentSerena_patchouli;
	// �X�e�[�W2
	static bool _isUsingBullet_normal_cirno;
	static bool _isUsingBullet_icicleFall_cirno;
	static bool _isUsingBullet_perfectFreeze_cirno;
	// �X�e�[�W3
	static bool _isUsingBullet_normal_suwako;
	static bool _isUsingBullet_ironRingOfMoriya_suwako;
	static bool _isUsingBullet_keroChanStandsFirmAgainstTheStorm_suwako;

	// �v���C���[�̃{���`��t���O
	static bool _isRenderPlayersBombEffect;

	static bool _isShowBeginGameText;


private:

	int _bgAlpha_when_call_pauseMenu = 255;

	static float _showBeginText_timer;

	static float _deltaTime_ref;

	int miniMap_hdl{};
	int _STAGE_ID;


	tnl::Vector2i miniMap_center_pos = { 120,600 };
};