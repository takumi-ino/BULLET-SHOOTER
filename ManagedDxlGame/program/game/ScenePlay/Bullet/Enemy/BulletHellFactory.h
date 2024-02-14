#pragma once
#include "../../../Loader/CsvLoader.h"

class BulletHell;
class EnemyManager;

// パチュリー（ノーマル）
constexpr int _ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI = 8;
constexpr int _EVERYDIRECTION_BULLETCOUNT_T_NORMAL_PATCHOULI = 192;
// パチュリー（メタル・ファティーグ）
constexpr int _EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI = 8;
constexpr int _SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI = 64;
// パチュリー（サイレント・セレナ）
constexpr int _CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI = 320;
constexpr int _SLOWLYCOMING_BULLETCOUNT_T_SILENTSERENA_PATCHOULI = 50;
// チルノ（ノーマル）
constexpr int _STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO = 99;
constexpr int _EVERYDIRECTION_BULLETCOUNT_T_NORMAL_CIRNO = 48;
// チルノ（アイシクル・フォール）
constexpr int _SHOTOUTERMOVEINNER_BULLETCOUNT_T_ICICLEFALL_CIRNO = 120;
constexpr int _LINEUPSTRAIGHTSHOT_BULLETCOUNT_T_ICICLEFALL_CIRNO = 15;
// チルノ（パーフェクト・フリーズ）
constexpr int _CIRCLECONTIGUOUS_BULLETCOUNT_T_PERFECTFREEZE_CIRNO = 300;
constexpr int _EXPANDSTRAIGHTSHOT_BULLETCOUNT_T_PERFECTFREEZE_CIRNO = 100;



// 特定の弾幕パターンを生成

class BulletHellFactory
{
public:

	BulletHellFactory();

	void InitBulletHellInfo();

	std::vector<Shared<EnemyBullet>> CreateBulletHell(const BulletHell::TYPE type);

private:

	// パチュリー・ノーレッジ------------------------------------------------------------------------------------------------------------
	//// <summary>
	//  ※ (パチュリー専用)
	// 1.ボスの周囲に4つの巨大な弾を生成
	// 2.その弾から直線に光線を放つ
	// 3.光線を放ちながら左回転
	// 4.ボスの周囲に新たな4つの巨大な弾を生成
	// 5.同じように弾から直線に光線を放ち、今度は右回転
	// 6.左回転をしている弾と光線を消去
	// 7.右回転をしている弾と光線を消去
	// 8.四方八方に４連続で弾を発射
	// 9.左回転、右回転の技に合わせ、四方八方に４連続で弾を発射
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Patchouli();

	//// <summary>
	//  ※ (パチュリー専用)
	// 円形連続発射と、ランダムにゆっくり近づいてくる弾を組み合わせた弾幕
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_SilentSerena_Patchouli();

	//// <summary>
	//  ※ (パチュリー専用)
	// 円状に8way弾が展開され、途中でそれぞれが8way弾に増える。
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_MetalFatigue_Patchouli();

	//// <summary>
	/// ※　CreateBulletHell_MetalFatigue_Patchouli関数で使用する弾幕生成ロジックの関数化
	/// </summary>
	/// <param name="split_one_into_eight_WAVE1"></param>
	/// <param name="split_one_into_eight_bullet"></param>
	//// <param name="enemyBullet"></param>
	void InitSplitOneIntoEightBullet(
		int split_one_into_eight_WAVE1,int id ,Shared<EnemyBullet>& split_one_into_eight_bullet, std::vector<Shared<EnemyBullet>>& enemyBullet, EnemyBullet::SPECIFICTYPE wave);

	// チルノ-------------------------------------------------------------------------------------------------------------------------
	//// <summary>
    //  ※ (チルノ専用)
    // 1.ショットガン（3 + N)行 (Nは0～5まで)、６列の弾をプレイヤーへ発射
    // 2.２発目
    // 3.３発目
    // 4.全方向ばら撒きショット
	// 5.２発目（この２発目はすぐに縦長に収束し、プレイヤーへと放たれる）
	// 6.３発目
	// 7.1に戻る
    //// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Cirno();


	//// <summary>
    //  ※ (チルノ専用)
    // 1.
    // 2.
    // 3.
    // 4.
    //// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_IcicleFall_Cirno();


	//// <summary>
	//  ※ (チルノ専用)
	// 1.全方向ばら撒きショットをうつ
	// 2.フリーズ、弾を止めて白くする
	// 3.自機依存された８方向ショットを連続でうつ
	// 4.止めていた弾を全方向で動かし始める
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_PerfectFreeze();

	// 諏訪子-------------------------------------------------------------------------------------------------------------------------


	std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Suwako();


	std::vector<Shared<EnemyBullet>> CreateBulletHell_IronRingOfMoriya_Suwako();

	//// <summary>
	//　※ (諏訪子専用)
	//　1.上に向かって8way弾を撃つ
	//　2.徐々に下向きに加速していく
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_KeroChanStandsFirm_AgainstTheStorm();


private:

	Shared<CsvLoader> _csvLoader = nullptr;

	Shared<BulletHell> _bulletHell = nullptr;

private:

	BulletHellType_Info _sBltHell_patchouli_info{};
	BulletHellType_Info _sBltHell_cirno_info{};
	BulletHellType_Info _sBltHell_suwako_info{};

	std::unordered_map<int, BulletHellType_Info> _bulletHellData_map;
};