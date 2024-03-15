#pragma once

namespace inl {

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