#pragma once
/*

//-----------------------------------------------------------------------------------------------------------
//
//
// �V���h�E�}�b�v�ƃu���[���G�t�F�N�g�K�p�T���v��
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> mesh = nullptr;
Shared<dxe::Mesh> ground = nullptr;
Shared<dxe::ScreenEffect> screen_efct = nullptr;
Shared<dxe::ShadowMap> shadow = nullptr;


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	mesh = dxe::Mesh::CreateSphereMV(100, 20, 20);
	mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	mesh->pos_ = { 0, 100, 0 };

	ground = dxe::Mesh::CreatePlaneMV({ 2000, 2000, 0 }, 20, 20);
	ground->setTexture(dxe::Texture::CreateFromFile("graphics/lawn.png"));
	ground->rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));

	// �V���h�E�}�b�v�쐬
	shadow = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);

	// �u���[���G�t�F�N�g�쐬
	screen_efct = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);


}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	camera->update();

	//-------------------------------------------------------------------------------------
	//
	// �V���h�E�}�b�v���p�̏���
	//

	// �V���h�E�}�b�v�ւ̕`��J�n
	shadow->reserveBegin();

	ground->reserveShadow();
	mesh->reserveShadow();

	// �I��
	shadow->reserveEnd();


	//-------------------------------------------------------------------------------------
	//
	// �V���h�E�}�b�v�ƃu���[����K�p�����`��
	//

	// �V���h�E�}�b�v�ƃu���[���K�p�J�n
	shadow->renderBegin();
	screen_efct->renderBegin();

	// ���b�V���`��
	mesh->render(camera);
	ground->render(camera);

	// �V���h�E�}�b�v�ƃu���[���K�p�I��
	screen_efct->renderEnd();
	shadow->renderEnd();


	// �f�o�b�O���`��
	DrawDefaultLightGuiController();
	screen_efct->drawGuiController({ 0, 0 });

	DrawFpsIndicator({ 0, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}

*/
