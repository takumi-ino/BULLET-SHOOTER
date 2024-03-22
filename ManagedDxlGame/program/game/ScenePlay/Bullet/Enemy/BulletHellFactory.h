#pragma once
#include "../../../Loader/CsvLoader.h"
#include "BulletHellsBulletNumInfo.h"

class EnemyManager;


/*
 　　 　 各弾幕パターンを初期化 (EnemyBoss限定)


　　※    弾幕の名称・実装は
		「東方プロジェクト」という弾幕シューティングゲームを中心としたコンテンツから直接引用しています。
		固有名詞が非常に多く分かりづらくなっているかと存じますが、何卒ご理解いただけますと幸いです。　　
*/

namespace inl {

	class BulletHell;

	class BulletHellFactory
	{
	public:

		BulletHellFactory();

		// 初期化-------------------------------------------------------------------
		void InitBulletHellInfo();
		std::vector<Shared<EnemyBullet>> CreateBulletHell(const BulletHell::TYPE type);

	private:

		Shared<CsvLoader>  _csvLoader = nullptr;

	private:

		//   各キャラクターの各弾幕情報を構造体で取得。難易度ごとに弾幕を調整するため

		BulletHellType_Info _sBltHell_patchouli_info{};
		BulletHellType_Info _sBltHell_cirno_info{};
		BulletHellType_Info _sBltHell_suwako_info{};

		std::unordered_map<int, BulletHellType_Info> _bulletHellData_map;

	private:

		/*

			以下関数は

			  1. 機能（主に発射・初期化）
			  2. スペル（弾幕名）
			  3. 使用者（キャラクター名）

			という規則に則って命名しています。

	　　*/


	  // パチュリー・ノーレッジ(ステージ１ボス）------------------------------------------------------------------------------------------------------------
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
		//  ※ (パチュリー専用)　「メタルファティーグ」
		// 円状に8way弾が展開され、途中でそれぞれが8way弾に増える。
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_MetalFatigue_Patchouli();

		//// <summary>
		/// ※　CreateBulletHell_MetalFatigue_Patchouli関数で使用する弾幕生成ロジックの関数化
		/// </summary>
		/// <param name="splitOneIntoEight_wave"></param>
		/// <param name="splitOneIntoEight_bullet"></param>
		//// <param name="enemyBullet"></param>
		void InitAssist_MetalFatigue_Patchouli(
			const int splitOneIntoEight_wave,
			int id,
			Shared<EnemyBullet>& splitOneIntoEight_bullet,
			std::vector<Shared<EnemyBullet>>& enemyBullet,
			const EnemyBullet::SPECIFICTYPE wave
		);

		//// <summary>
		//  ※ (パチュリー専用)　「サイレント・セレナ」
		// 　円周上放射と、ランダムにゆっくり近づいてくる弾を組み合わせた弾幕
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_SilentSerena_Patchouli();



		// チルノ(ステージ２ボス）-------------------------------------------------------------------------------------------------------------------------
		//// <summary>
		//  ※ (チルノ専用)
		// 1.ショットガン（2 + N)行 (Nは0～5まで)、６列の弾をプレイヤーへ発射
		// 2.２発目
		// 3.３発目
		// 4.全方向ばら撒きショット
		// 5.２発目（この２発目はすぐに縦長に収束し、プレイヤーへと放たれる）
		// 6.３発目
		// 7.1に戻る
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Cirno();

		//// <summary>
		//  ※ (チルノ専用)　「アイシクルフォール」
		// 1.左右に１０行１列の直行弾を発射、真横から中央へと発射方向を少しずつ修正　(0.2秒間隔で11連続)
		// 2.その間プレイヤー目掛けて１行５列の弾を発射（0.5秒間隔で３連続）
		// 3.左右に撃った直行弾を停止させ、内側へと軌道修正
		//   なお、停止位置は全ての弾が弾一つ分の空間を確保するよう等間隔にする
		// 4.
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_IcicleFall_Cirno();

		//// <summary>
		//  ※ (チルノ専用)　「パーフェクトフリーズ」
		// 1.全方向ばら撒きショットをうつ
		// 2.フリーズ、弾を止めて白くする
		// 3.自機依存された８方向ショットを連続でうつ
		// 4.止めていた弾を全方向で動かし始める
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_PerfectFreeze_Cirno();



		// 諏訪子(ステージ３ボス）-------------------------------------------------------------------------------------------------------------------------
		//// <summary>
		//  ※ (諏訪子専用) 
		// 全方位に１行８列の弾を絶え間なく発射（右回りと左回り）
		// 　なお、弾は直線ではなく斜めに発射する
		// （右回りの場合、左側の弾ほどプレイヤー側に近く、右側の弾ほどボス側に近い）
		// （左回りの場合、上記の逆である）
		// 　1度の発射で６４発。８方向へ飛ばす
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Suwako();

		//// <summary>
		//  ※ (諏訪子専用)　「洩矢の鉄の輪」
		//  1.ボスの中心から周囲に円状に弾を出す。（１回の発射で2つの円を作り、2つで1つの塊とする）
		//  2.生成を０．５秒間隔で続ける
		//  3.生成した弾から順にプレイヤー方向へとゆっくり移動させる。（このとき円の形は崩さない）
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_IronRingOfMoriya_Suwako();

		//// <summary>
		//　※ (諏訪子専用)　「ケロちゃん風雨に負けず」
		//　1.上に向かってうねらせながら8way弾を撃つ
		//　2.弾を下向きに加速し全体に広げる
		//  3.同時にプレイヤー方向へ広がるように直行弾も放つ
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako();
	};
}