#pragma once
#include "../../../Loader/CsvLoader.h"

class BulletHell;
class EnemyManager;

// �p�`�����[�i�m�[�}���j
constexpr int ROUND_BULLETCOUNT_T_NORMAL_PATCHOULI = 8;
constexpr int EVERYDIRECTION_BULLETCOUNT_T_NORMAL_PATCHOULI = 192;
// �p�`�����[�i���^���E�t�@�e�B�[�O�j
constexpr int EVERYDIRECTION_BULLETCOUNT_T_METALFATIGUE_PATCHOULI = 8;
constexpr int SPLITONE_INTOEIGHT_BULLETCOUNT_T_METALFATIGUE_PATCHOULI = 64;
// �p�`�����[�i�T�C�����g�E�Z���i�j
constexpr int CIRCLECONTIGUOUS_BULLETCOUNT_T_SILENTSERENA_PATCHOULI = 320;
constexpr int SLOWLYCOMING_BULLETCOUNT_T_SILENTSERENA_PATCHOULI = 180;
// �`���m�i�m�[�}���j
constexpr int STRAIGHTAPPROACH_BULLETCOUNT_T_NORMAL_CIRNO = 81;
constexpr int EVERYDIRECTION_BULLETCOUNT_T_NORMAL_CIRNO = 48;
// �`���m�i�A�C�V�N���E�t�H�[���j
constexpr int SHOTOUTERMOVEINNER_BULLETCOUNT_T_ICICLEFALL_CIRNO = 120;
constexpr int LINEUPSTRAIGHTSHOT_BULLETCOUNT_T_ICICLEFALL_CIRNO = 15;
// �`���m�i�p�[�t�F�N�g�E�t���[�Y�j
constexpr int CIRCLECONTIGUOUS_BULLETCOUNT_T_PERFECTFREEZE_CIRNO = 300;
constexpr int EXPANDSTRAIGHTSHOT_BULLETCOUNT_T_PERFECTFREEZE_CIRNO = 120;
// �z�K�q�i�m�[�}���j
constexpr int CIRCLECONTIGUOUS_BULLETCOUNT_T_NORMAL_SUWAKO = 320;
// �z�K�q�i�k��̓S�̗ցj
constexpr int SLOWLYCOMING_BULLETCOUNT_T_IRONRINGOFMORIYA_SUWAKO = 900;
// �z�K�q�i�P������񕗉J�ɕ������j
constexpr int STRAIGHTAPPROACH_BULLETCOUNT_T_KCSTANDSFIRMAGAINSTSTORM_SUWAKO = 200;
constexpr int FALLINGAPPROACH_BULLETCOUNT_T_KCSTANDSFIRMAGAINSTSTORM_SUWAKO = 300;



// ����̒e���p�^�[���𐶐�
class BulletHellFactory
{
public:

	BulletHellFactory();

	void InitBulletHellInfo();

	std::vector<Shared<EnemyBullet>> CreateBulletHell(const BulletHell::TYPE type);

private:

	Shared<CsvLoader>  _csvLoader = nullptr;

private:

	BulletHellType_Info _sBltHell_patchouli_info{};
	BulletHellType_Info _sBltHell_cirno_info{};
	BulletHellType_Info _sBltHell_suwako_info{};

	std::unordered_map<int, BulletHellType_Info> _bulletHellData_map;

private:

	// �p�`�����[�E�m�[���b�W------------------------------------------------------------------------------------------------------------
	//// <summary>
	//  �� (�p�`�����[��p)
	// 1.�{�X�̎��͂�4�̋���Ȓe�𐶐�
	// 2.���̒e���璼���Ɍ��������
	// 3.����������Ȃ��獶��]
	// 4.�{�X�̎��͂ɐV����4�̋���Ȓe�𐶐�
	// 5.�����悤�ɒe���璼���Ɍ���������A���x�͉E��]
	// 6.����]�����Ă���e�ƌ���������
	// 7.�E��]�����Ă���e�ƌ���������
	// 8.�l�������ɂS�A���Œe�𔭎�
	// 9.����]�A�E��]�̋Z�ɍ��킹�A�l�������ɂS�A���Œe�𔭎�
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Patchouli();

	//// <summary>
	//  �� (�p�`�����[��p)�@�u���^���t�@�e�B�[�O�v
	// �~���8way�e���W�J����A�r���ł��ꂼ�ꂪ8way�e�ɑ�����B
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_MetalFatigue_Patchouli();

	//// <summary>
	/// ���@CreateBulletHell_MetalFatigue_Patchouli�֐��Ŏg�p����e���������W�b�N�̊֐���
	/// </summary>
	/// <param name="split_one_into_eight_WAVE1"></param>
	/// <param name="split_one_into_eight_bullet"></param>
	//// <param name="enemyBullet"></param>
	void InitSplitOneIntoEightBullet(
		int split_one_into_eight_WAVE1, int id, Shared<EnemyBullet>& split_one_into_eight_bullet, std::vector<Shared<EnemyBullet>>& enemyBullet, EnemyBullet::SPECIFICTYPE wave);

	//// <summary>
    //  �� (�p�`�����[��p)�@�u�T�C�����g�E�Z���i�v
    // �~�`�A�����˂ƁA�����_���ɂ������߂Â��Ă���e��g�ݍ��킹���e��
    //// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_SilentSerena_Patchouli();

	// �`���m-------------------------------------------------------------------------------------------------------------------------
	//// <summary>
	//  �� (�`���m��p)
	// 1.�V���b�g�K���i2 + N)�s (N��0�`5�܂�)�A�U��̒e���v���C���[�֔���
	// 2.�Q����
	// 3.�R����
	// 4.�S�����΂�T���V���b�g
	// 5.�Q���ځi���̂Q���ڂ͂����ɏc���Ɏ������A�v���C���[�ւƕ������j
	// 6.�R����
	// 7.1�ɖ߂�
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Cirno();

	//// <summary>
	//  �� (�`���m��p)�@�u�A�C�V�N���t�H�[���v
	// 1.���E�ɂP�O�s�P��̒��s�e�𔭎ˁA�^�����璆���ւƔ��˕������������C���@(0.2�b�Ԋu��11�A��)
	// 2.���̊ԃv���C���[�ڊ|���ĂP�s�T��̒e�𔭎ˁi0.5�b�Ԋu�łR�A���j
	// 3.���E�Ɍ��������s�e���~�����A�����ւƋO���C��
	//   �Ȃ��A��~�ʒu�͑S�Ă̒e���e����̋�Ԃ��m�ۂ���悤���Ԋu�ɂ���
	// 4.
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_IcicleFall_Cirno();

	//// <summary>
	//  �� (�`���m��p)�@�u�p�[�t�F�N�g�t���[�Y�v
	// 1.�S�����΂�T���V���b�g������
	// 2.�t���[�Y�A�e���~�߂Ĕ�������
	// 3.���@�ˑ����ꂽ�W�����V���b�g��A���ł���
	// 4.�~�߂Ă����e��S�����œ������n�߂�
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_PerfectFreeze();

	// �z�K�q-------------------------------------------------------------------------------------------------------------------------
	//// <summary>
	//  �� (�z�K�q��p) 
	// �S���ʂɂP�s�W��̒e��₦�ԂȂ����ˁi�E���ƍ����j
	// �@�Ȃ��A�e�͒����ł͂Ȃ��΂߂ɔ��˂���
	// �i�E���̏ꍇ�A�����̒e�قǃv���C���[���ɋ߂��A�E���̒e�قǃ{�X���ɋ߂��j
	// �i�����̏ꍇ�A��L�̋t�ł���j
	// �@1�x�̔��˂łU�S���B�W�����֔�΂�
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_Normal_Suwako();

	//// <summary>
	//  �� (�z�K�q��p)�@�u�k��̓S�̗ցv
	//  1.�{�X�̒��S������͂ɉ~��ɒe���o���B�i�P��̔��˂�2�̉~�����A2��1�̉�Ƃ���j
	//  2.�������O�D�T�b�Ԋu�ő�����
	//  3.���������e���珇�Ƀv���C���[�����ւƂ������ړ�������B�i���̂Ƃ��~�̌`�͕����Ȃ��j
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_IronRingOfMoriya_Suwako();

	//// <summary>
	//�@�� (�z�K�q��p)�@�u�P������񕗉J�ɕ������v
	//�@1.��Ɍ������Ă��˂点�Ȃ���8way�e������
	//�@2.�e���������ɉ������S�̂ɍL����
	//  3.�����Ƀv���C���[�����֍L����悤�ɒ��s�e������
	//// </summary>
	std::vector<Shared<EnemyBullet>> CreateBulletHell_KeroChanStandsFirm_AgainstTheStorm();
};