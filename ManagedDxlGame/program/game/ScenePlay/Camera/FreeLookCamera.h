#pragma once


class FreeLookCamera : public dxe::Camera
{
public:

	FreeLookCamera() {}
	FreeLookCamera(const int screen_w, const int screen_h) : dxe::Camera(screen_w, screen_h) {}

	void Update(const float deltaTime) override;

private:

	inline tnl::Vector3 up() {
		up_ = tnl::Vector3::TransformCoord({ 0,1,0 }, _rot);
		return up_;
	}

	inline tnl::Vector3 down() { return -up_; }

	inline::tnl::Vector3 forward() override {
		target_ = pos_ + tnl::Vector3::TransformCoord({ 0,0,1 }, _rot);
		return tnl::Vector3::Normalize(target_ - pos_);
	}

	inline tnl::Vector3 back() override { return -forward(); }
	inline tnl::Vector3 left() override { return tnl::Vector3::Cross(forward(), up()); }
	inline tnl::Vector3 right() override { return tnl::Vector3::Cross(up(), forward()); }

	// ÉJÉÅÉâêßå‰
	void ControlFreeLookCamera(
		const Shared<FreeLookCamera> camera, const Shared<dxe::Mesh> playerMesh, const tnl::Vector3& eye);

private:

	tnl::Quaternion _rot;
};