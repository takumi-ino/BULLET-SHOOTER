#pragma once
#include <random>
#include "../../DxLibEngine.h"

// BulletHellクラスのみで使用する機能クラス

namespace inl {

	class RandomValueGenerator 
	{
	public:

		// ランダムベクター生成
		static const tnl::Vector3 Vector(
			const int minX,
			const int maxX,
			const int minY,
			const int maxY,
			const int minZ,
			const int maxZ) noexcept
		{
			std::random_device rd;
			std::mt19937 mt(rd());

			std::uniform_int_distribution<int> rnd_valX(minX, maxX);
			std::uniform_int_distribution<int> rnd_valY(minY, maxY);
			std::uniform_int_distribution<int> rnd_valZ(minZ, maxZ);

			return tnl::Vector3{
				static_cast<float>(rnd_valX(mt)),
				static_cast<float>(rnd_valY(mt)),
				static_cast<float>(rnd_valZ(mt))
			};
		}

		// ランダム整数生成
		static const int Int(const int min, const int max) noexcept {

			std::random_device rd;
			std::mt19937 mt(rd());

			std::uniform_int_distribution<int> randomInt(min, max);

			return static_cast<int>(randomInt(mt));
		}

		// ランダム浮動小数生成
		static const float Float(const float min, const float max) noexcept {

			std::random_device rd;
			std::mt19937 mt(rd());

			std::uniform_real_distribution<float> randomFloat(min, max);

			return static_cast<float>(randomFloat(mt));
		}
	};
}