#pragma once

namespace inl {

	//�@�X�e�[�W�R�Ŏg�p���鎩�O�̃p�[�e�B�N���N���X
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