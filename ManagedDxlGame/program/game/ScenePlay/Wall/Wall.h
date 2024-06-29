#pragma once
#include "../../DxLibEngine.h"

// �������ȕǂ��t�B�[���h�ɐݒ�

namespace inl {

	class Wall
	{
	public:

		Wall();

		void Render(const Shared<dxe::Camera> mainCamera);

	private:

		// �������ɂȂ郁�b�V��
		Shared<dxe::Mesh> origine_mesh = nullptr;

		// �O���[�v���b�V��
		Shared<dxe::Mesh> group_mesh = nullptr;
	};
}