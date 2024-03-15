#pragma once

namespace inl {

	class FreeLookCamera : public dxe::Camera
	{
	public:

		FreeLookCamera() {}
		FreeLookCamera(const int screen_w, const int screen_h) : dxe::Camera(screen_w, screen_h) {}

		~FreeLookCamera() override {}
	};
};