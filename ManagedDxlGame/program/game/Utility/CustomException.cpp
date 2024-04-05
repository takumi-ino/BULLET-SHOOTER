#include "../game/Utility/CustomException.h"
#include "../game/Utility/FilePathChecker.h"


namespace inl {

	//　画像----------------------------------------------------------------------------------
	int CustomException::TryLoadGraph(
		const std::string filePath, 
		const std::string relevantFunction)
	{

		try {
			bool graph = FilePathChecker::GRAPH(filePath);        // 最初にファイルパスが正しいか確認

			if (!graph) {
				throw CustomException(relevantFunction.c_str());  // 正しくなければ例外を投げる
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		auto it = _graphicHandle.find(filePath);				  // 1. _graphicHandleの中を走査

		if (it != _graphicHandle.end())
			return _graphicHandle[filePath];	                  // 2. filePath名が既にある場合、値をリターンし終了

		int handle = LoadGraph(filePath.c_str());	              // 3. filePath名がまだない場合、LoadGraphを実行

		_graphicHandle.insert(std::make_pair(filePath, handle));  // 4. LoadGraph後、そのハンドルを_graphicHandleに登録
		return handle;                                            //（次からは２の処理でリターン）
	}


	//　サウンド----------------------------------------------------------------------------------
	int CustomException::TryLoadSound(
		const std::string filePath, 
		const std::string relevantFunction) 
	{

		try {
			bool sound = FilePathChecker::SOUND(filePath);

			if (!sound) {
				throw CustomException(relevantFunction.c_str());
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		auto it = _soundHandle.find(filePath);

		if (it != _soundHandle.end())
			return _soundHandle[filePath];

		auto file = LoadSoundMem(filePath.c_str());

		_soundHandle.insert(std::make_pair(filePath, file));
		return file;
	}


	//　テクスチャ----------------------------------------------------------------------------------
	Shared<dxe::Texture> CustomException::TryLoadTexture(
		const std::string filePath,
		const std::string relevantFunction)
	{

		try {
			bool graph = FilePathChecker::GRAPH(filePath);

			if (!graph) {
				throw CustomException(relevantFunction.c_str());
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		auto it = _textureFile.find(filePath);

		if (it != _textureFile.end())
			return _textureFile[filePath];

		auto file = dxe::Texture::CreateFromFile(filePath);

		_textureFile.insert(std::make_pair(filePath, file));
		return file;
	}


	//　サウンドパス----------------------------------------------------------------------------------
	std::string CustomException::TryLoadSoundPath(
		const std::string filePath,
		const std::string relevantFunction)
	{

		try {
			bool graph = FilePathChecker::SOUND(filePath);

			if (!graph) {
				throw CustomException(relevantFunction.c_str());
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		return filePath;
	}


	//　バイナリファイルパス--------------------------------------------------------------------------
	std::string CustomException::TryLoadBinaryPath(
		const std::string filePath, 
		const std::string relevantFunction)
	{

		try {
			bool graph = FilePathChecker::BINARY(filePath);

			if (!graph) {
				throw CustomException(relevantFunction.c_str());
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		return filePath;
	}


	//　パーティクルのバイナリファイル-----------------------------------------------------------------
	Shared<dxe::Particle> CustomException::TryLoadParticleBinaryFile(
		const std::string filePath, 
		const std::string relevantFunction) 
	{

		try {
			bool graph = FilePathChecker::BINARY(filePath);

			if (!graph) {
				throw CustomException(relevantFunction.c_str());
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		auto it = _binaryFile.find(filePath);

		if (it != _binaryFile.end())
			return _binaryFile[filePath];

		Shared<dxe::Particle> particle = std::make_shared<dxe::Particle>(filePath);


		_binaryFile.insert(std::make_pair(filePath, particle));

		return particle;
	}


	//　CSVファイル----------------------------------------------------------------------------------
	std::vector<std::vector<tnl::CsvCell>> CustomException::TryLoadCsvFile(
		const std::string filePath,
		const std::string relevantFunction) 
	{

		try {
			bool graph = FilePathChecker::CSV(filePath);

			if (!graph) {
				throw CustomException(relevantFunction.c_str());
			}
		}
		catch (const CustomException& e) {

			std::cerr << e.pleaseCheckFilePath() << std::endl << e.getFunctionName();
		}

		auto it = _csvFile.find(filePath);

		if (it != _csvFile.end())
			return _csvFile[filePath];

		auto csv = tnl::LoadCsv(filePath);

		_csvFile.insert(std::make_pair(filePath, csv));

		return csv;
	}
}