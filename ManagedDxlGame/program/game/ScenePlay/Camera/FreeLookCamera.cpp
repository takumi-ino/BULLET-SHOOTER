#include "../../DxLibEngine.h"
#include "FreeLookCamera.h"
#include "../Character/Player/Player.h"



void FreeLookCamera::Update(const float delta_time) {

	dxe::Camera::Update(delta_time);
}


void FreeLookCamera::ControlFreeLookCamera(const Shared<FreeLookCamera> camera, const Shared<dxe::Mesh> playerMesh, const tnl::Vector3& eye) {

	tnl::Input::RunIndexKeyDown(
		[&](uint32_t index) {
			tnl::Vector3 v[4] = {
				camera->left(),
				camera->right(),
				tnl::Vector3::up,
				tnl::Vector3::down
			};
			camera->pos_ += v[index] * 3.0f;
			camera->target_ = eye;

		}, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);


	camera->pos_ += camera->forward().xz() * tnl::Input::GetMouseWheel() * 0.3f;
	playerMesh->rot_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1));



	//for (int i = 0; i < 4; ++i) {
	//	tnl::Vector3 v = camera->getFlustumNormal(static_cast<dxe::Camera::eFlustum>(i));
	//	tnl::Vector3 np = tnl::GetNearestPointPlane(playerMesh->pos_, v, camera->pos_);
	//	if ((np - playerMesh->pos_).length() < 100) {
	//		playerMesh->pos_ = np + (v * 100);
	//	}
	//}
}