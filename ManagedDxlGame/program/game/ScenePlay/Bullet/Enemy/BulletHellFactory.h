#pragma once
#include "../../../Loader/CsvLoader.h"
#include "BulletHellsBulletNumInfo.h"

class EnemyManager;


/*
 �@�@ �@ �e�e���p�^�[���������� (EnemyBoss����)


�@�@��    �e���̖��́E������
		�u�����v���W�F�N�g�v�Ƃ����e���V���[�e�B���O�Q�[���𒆐S�Ƃ����R���e���c���璼�ڈ��p���Ă��܂��B
		�ŗL���������ɑ���������Â炭�Ȃ��Ă��邩�Ƒ����܂����A�������������������܂��ƍK���ł��B�@�@
*/

namespace inl {

	class BulletHell;

	class BulletHellFactory
	{
	public:

		BulletHellFactory();

		// ������-------------------------------------------------------------------
		void InitBulletHellInfo();
		std::vector<Shared<EnemyBullet>> CreateBulletHell(const BulletHell::TYPE type);

	private:

		Shared<CsvLoader>  _csvLoader = nullptr;

	private:

		//   �e�L�����N�^�[�̊e�e�������\���̂Ŏ擾�B��Փx���Ƃɒe���𒲐����邽��

		BulletHellType_Info _sBltHell_patchouli_info{};
		BulletHellType_Info _sBltHell_cirno_info{};
		BulletHellType_Info _sBltHell_suwako_info{};

		std::unordered_map<int, BulletHellType_Info> _bulletHellData_map;

	private:

		/*

			�ȉ��֐���

			  1. �@�\�i��ɔ��ˁE�������j
			  2. �X�y���i�e�����j
			  3. �g�p�ҁi�L�����N�^�[���j

			�Ƃ����K���ɑ����Ė������Ă��܂��B

	�@�@*/


	  // �p�`�����[�E�m�[���b�W(�X�e�[�W�P�{�X�j------------------------------------------------------------------------------------------------------------
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
		//  �� (�p�`�����[��p)�@�u�T�C�����g�E�Z���i�v
		// �@�~������˂ƁA�����_���ɂ������߂Â��Ă���e��g�ݍ��킹���e��
		//// </summary>
		std::vector<Shared<EnemyBullet>> CreateBulletHell_SilentSerena_Patchouli();



		// �`���m(�X�e�[�W�Q�{�X�j-------------------------------------------------------------------------------------------------------------------------
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
		std::vector<Shared<EnemyBullet>> CreateBulletHell_PerfectFreeze_Cirno();



		// �z�K�q(�X�e�[�W�R�{�X�j-------------------------------------------------------------------------------------------------------------------------
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
		std::vector<Shared<EnemyBullet>> CreateBulletHell_KeroChanStandsFirmAgainstTheStorm_Suwako();
	};
}