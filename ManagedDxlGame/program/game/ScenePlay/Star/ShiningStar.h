#pragma once

namespace inl {

	//　ステージ３で使用する自前のパーティクルクラス
	class ShiningStar
	{
	private:

		struct S_ShiningStar
		{
			float x, y, z;
		};

	public:

		S_ShiningStar star[1000];

		void Render();
	};
}