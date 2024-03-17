#pragma once
#include "../DxLibEngine.h"
#include <iostream>
#include <exception>


namespace inl {

    /*
          カスタム例外クラス

          現在ある例外処理は主にファイルパスチェック用のもののみだが
          必要に応じて様々な例外処理を追加可能
    */

    class CustomException : public std::exception
    {
    public:

        CustomException(){}

        // コンストラクタから関数名を取得
        CustomException(const char* functionName) : _functionName(functionName) {}

        const char* what() const noexcept override {
            return "例外を検知しました";
        }

        const char* getFunctionName() const noexcept {         // 例外発生の該当関数名

            std::string str = "該当の関数：" + _functionName;
            return _functionName.c_str();
        }

        const char* pleaseCheckFilePath() const noexcept {     // ファイルパス取得失敗
            return "ファイルパスを確認してください";
        }

        //　ファイルパスチェック---------------------------------------------------------------------------

        /*
        　　　　　第１引数にファイルパス、第２引数に処理を呼んだ関数の名前を指定
        */

        // 画僧
        int TryLoadGraph(const std::string filePath, const std::string relevantFunction);

        // サウンド
        int TryLoadSound(const std::string filePath, const std::string relevantFunction);

        // テクスチャ
        Shared<dxe::Texture> TryLoadTexture(const std::string filePath, const std::string relevantFunction);

        // パーティクル
        Shared<dxe::Particle> TryLoadParticleBinaryFile(const std::string filePath, const std::string relevantFunction);

        // サウンドファイルパス
        std::string TryLoadSoundPath(const std::string filePath, const std::string relevantFunction);

        // バイナリーファイルパス
        std::string TryLoadBinaryPath(const std::string filePath, const std::string relevantFunction);

        // CSVファイル
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