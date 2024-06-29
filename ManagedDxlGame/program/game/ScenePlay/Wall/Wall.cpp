#include "Wall.h"

namespace inl {

	Wall::Wall() {

		// ���������b�V��
		origine_mesh = dxe::Mesh::CreateFromFileMV("mesh/mv/test.mv");
		origine_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/Scene/stageWall.png"));

		std::vector<tnl::Matrix> mats;

		
		//mats.emplace_back(
		//	tnl::Matrix::AffineTransformation({ -1500, 0, 0 },	// �ʒu
		//		{ 5, 50, 150 },									// �T�C�Y�@( ���A���A�c )
		//		tnl::Quaternion::RotationAxis({ 1, 0, 0 },		// ��]
		//			tnl::ToRadian(90)))
		//);

		//mats.emplace_back(
		//	tnl::Matrix::AffineTransformation({ 1500, 0, 0 }, 
		//		{ 5, 50, 150 },
		//		tnl::Quaternion::RotationAxis({ 1, 0, 0 },
		//			tnl::ToRadian(90)))
		//);

		//�@��]�����̒������ł��Ă��Ȃ����߃R�����g�A�E�g
		
		/*mats.emplace_back(
			tnl::Matrix::AffineTransformation({ 0, 0, 1500 },
				{ 5, 50, 150 }, 
				tnl::Quaternion::RotationAxis({ 1, 0, 1 },
					tnl::ToRadian(340)))
		);*/

		//mats.emplace_back(
		//	tnl::Matrix::AffineTransformation({ 0, 0, -1500 },
		//		{ 5, 50, 150 },
		//		tnl::Quaternion::RotationAxis({ 0, 0, 1 },
		//			tnl::ToRadian(180)))
		//);

		group_mesh = dxe::Mesh::CreateStaticMeshGroupMV(origine_mesh, mats);
	}


	void Wall::Render(const Shared<dxe::Camera> mainCamera) {

		group_mesh->setBlendMode(DX_BLENDMODE_ADD);
		group_mesh->setAlpha(1.f);
		group_mesh->render(mainCamera);
	}
}