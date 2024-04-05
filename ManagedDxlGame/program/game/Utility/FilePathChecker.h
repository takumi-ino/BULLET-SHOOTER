#pragma once
#include "../DxLibEngine.h"

namespace inl {

	//　ファイルパスが正しいかを調べ、結果のみを true or falseで返す
	class FilePathChecker
	{
	public:

		static const bool CSV(const std::string path) {

			auto csv = tnl::LoadCsv(path);

			if (csv.empty())
				return false;

			return true;
		}


		static const bool SOUND(const std::string path) {

			auto sound = LoadSoundMem(path.c_str());

			if (sound == -1) 
				return false;			

			return true;
		}


		static const bool GRAPH(const std::string path) {

			auto graph = LoadGraph(path.c_str());

			if (graph == -1)
				return false;
			
			return true;
		}


		static const bool BINARY(const std::string path) {

			FILE* fp = nullptr;
			fopen_s(&fp, path.c_str(), "rb");

			if (!fp)
				return false;
			else {
				fclose(fp);
			}

			return true;			
		}				
	};
}