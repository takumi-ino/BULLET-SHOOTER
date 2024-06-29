#pragma once
#include "../../DxLibEngine.h"

// 半透明な壁をフィールドに設定

namespace inl {

	class Wall
	{
	public:

		Wall();

		void Render(const Shared<dxe::Camera> mainCamera);

	private:

		// 複製元になるメッシュ
		Shared<dxe::Mesh> origine_mesh = nullptr;

		// グループメッシュ
		Shared<dxe::Mesh> group_mesh = nullptr;
	};
}