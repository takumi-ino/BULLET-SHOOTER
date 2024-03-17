#pragma once
#include "../DxLibEngine.h"
#include <iostream>
#include <exception>


namespace inl {

    /*
          �J�X�^����O�N���X

          ���݂����O�����͎�Ƀt�@�C���p�X�`�F�b�N�p�̂��݂̂̂���
          �K�v�ɉ����ėl�X�ȗ�O������ǉ��\
    */

    class CustomException : public std::exception
    {
    public:

        CustomException(){}

        // �R���X�g���N�^����֐������擾
        CustomException(const char* functionName) : _functionName(functionName) {}

        const char* what() const noexcept override {
            return "��O�����m���܂���";
        }

        const char* getFunctionName() const noexcept {         // ��O�����̊Y���֐���

            std::string str = "�Y���̊֐��F" + _functionName;
            return _functionName.c_str();
        }

        const char* pleaseCheckFilePath() const noexcept {     // �t�@�C���p�X�擾���s
            return "�t�@�C���p�X���m�F���Ă�������";
        }

        //�@�t�@�C���p�X�`�F�b�N---------------------------------------------------------------------------

        /*
        �@�@�@�@�@��P�����Ƀt�@�C���p�X�A��Q�����ɏ������Ă񂾊֐��̖��O���w��
        */

        // ��m
        int TryLoadGraph(const std::string filePath, const std::string relevantFunction);

        // �T�E���h
        int TryLoadSound(const std::string filePath, const std::string relevantFunction);

        // �e�N�X�`��
        Shared<dxe::Texture> TryLoadTexture(const std::string filePath, const std::string relevantFunction);

        // �p�[�e�B�N��
        Shared<dxe::Particle> TryLoadParticleBinaryFile(const std::string filePath, const std::string relevantFunction);

        // �T�E���h�t�@�C���p�X
        std::string TryLoadSoundPath(const std::string filePath, const std::string relevantFunction);

        // �o�C�i���[�t�@�C���p�X
        std::string TryLoadBinaryPath(const std::string filePath, const std::string relevantFunction);

        // CSV�t�@�C��
        std::vector<std::vector<tnl::CsvCell>> TryLoadCsvFile(const std::string filePath, const std::string relevantFunction);

    private:

        std::string _functionName;

        std::unordered_map<std::string, int>                                    _graphicHandle;
        std::unordered_map<std::string, int>                                    _soundHandle;
        std::unordered_map<std::string, Shared<dxe::Texture>>                   _textureFile;
        std::unordered_map<std::string, Shared<dxe::Particle>>                  _binaryFile;
        std::unordered_map<std::string, std::vector<std::vector<tnl::CsvCell>>> _csvFile;
    };
}