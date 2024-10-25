#pragma once
#include "EnemyBullet.h"


class ScenePlay;
class Player;

/*
 　　 　弾の生成、更新、削除などを行うクラス (EnemyBoss限定)
　　　ボスは通常攻撃、特殊攻撃１、特殊攻撃２、の計３種類の弾幕を放つ

    ※　関数の定義は1箇所にまとめると長くなってしまうため、各ボスごとの cpp に分けています。

		　1. AllBulletHell_PatchouliKnowledge.cpp　 （ステージ１ボス）
		　2. AllBulletHell_Cirno.cpp				（ステージ２ボス）
		　3. AllBulletHell_MoriyaSuwako.cpp		  　（ステージ３ボス）


　　※    弾幕の名称・実装は
        「東方プロジェクト」という弾幕シューティングゲームを中心としたコンテンツから直接引用しています。
	    固有名詞が非常に多く分かりづらくなっているかと存じますが、何卒ご理解いただけますと幸いです。
*/


namespace inl {

	class BulletHellFactory;
	class RandomValueGenerator;

	class BulletHell : public EnemyBullet
	{
	public:

		enum class TYPE {

			// パチュリー（ステージ１ボス）
			Normal_Patchouli,
			MetalFatigue_Patchouli,
			SilentSerena_Patchouli,

			// チルノ（ステージ２ボス）
			Normal_Cirno,
			IcicleFall_Cirno,
			Perfect_Freeze_Cirno,

			// 諏訪子（ステージ３ボス）
			Normal_Suwako,
			IronRingOfMoriya_Suwako,
			KeroChan_StandsFirm_AgainstTheStorm_Suwako,
		};

	public:

		// コンストラクタ--------------------------------------------------------------------------------

		BulletHell() {}
		BulletHell(const Shared<dxe::Mesh>& bossMesh) : _bossMesh_ref(bossMesh) {}

		~BulletHell() override {}

		// ----------------------------------------------------------------------------------------------

		/*
			 以下関数は

			 1. 機能（主に発射・初期化）
			 2. スペル（弾幕名）
			 3. 使用者（キャラクター名）

			 という規則に則って命名しています。
		*/

		// ステージ1ボス（パチュリー）--------------------------------------------------
		void ShotBulletHell_Normal_Patchouli(const float deltaTime);
		void ShotBulletHell_MetalFatigue_Patchouli(const float deltaTime);
		void ShotBulletHell_SilentSerena_Patchouli(const float deltaTime);

		// ステージ2ボス（チルノ）----------------------------------------------------------
		void ShotBulletHell_Normal_Cirno(const float deltaTime);
		void ShotBulletHell_IcicleFall_Cirno(const float deltaTime);
		void ShotBulletHell_PerfectFreeze_Cirno(const float deltaTime);

		// ステージ3ボス（諏訪子）----------------------------------------------------------
		void ShotBulletHell_Normal_Suwako(const float deltaTime);
		void ShotBulletHell_IronRingOfMoriya_Suwako(const float deltaTime);
		void ShotBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako(const float deltaTime);

	private:

		// 似た処理が多い場合、効率化のため補助関数を使用（ InitAssist, WaveAssist, UpdateAssist）など

		// ステージ1ボス（パチュリー）-----------------------------------------------------------
		void WaveAssist_MetalFatigue_Patchouli(
			Shared<EnemyBullet>& bullet,
			const float deltaTime,
			const float angleOrigin,
			const float radiusOrigin,
			const float startMoveTime
		);

		// ステージ2ボス（チルノ）---------------------------------------------------------------
		void WaveAssist_Normal_Cirno(
			Shared<EnemyBullet>& bullet,
			const float timing,
			const float delayOffset,
			const float deltaTime
		);

		void InitAssist_IcicleFall_StraightBlue_Cirno(
			Shared<EnemyBullet>& bullet,
			const float radius,
			const float angle
		);

		void UpdateAssist_IcicleFall_StraightBlue_Cirno(
			Shared<EnemyBullet>& bullet,
			const float radius,
			const float newAngle,
			const float startChangeDirTime,
			const float deltaTime
		);

		void InitAssist_IcicleFall_StraightYellow_Cirno(
			Shared<EnemyBullet>& bullet
		);

		void UpdateAssist_IcicleFall_StraightYellow_Cirno(
			Shared<EnemyBullet>& bullet,
			const float deltaTime,
			const float bulletSpeed
		);

		// ステージ3ボス（諏訪子）-----------------------------------------------------------------------
		void ResetAssist_Normal_Suwako(
			Shared<EnemyBullet>& bullet,
			const float timeLimit
		);

		void WaveAssist_IronRingOfMoriya_Suwako(
			Shared<EnemyBullet>& bullet,
			const float circleRadius,
			const float angle,
			const float deltaTime,
			const float bulletSpeed,
			const int startMoveTime
		);

		void InitAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
			const float deltaTime,
			const float upwardPower,
			Shared<EnemyBullet>& bullet,
			const bool isOscillate
		);

		void UpdateAssist_KeroChanStandsFirmAgainstTheStorm_Suwako(
			Shared<EnemyBullet>& bullet,
			const float deltaTime
		);

	private:

		const Shared<dxe::Mesh> _bossMesh_ref = nullptr;
	};
}