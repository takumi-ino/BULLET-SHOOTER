#include "DxLibEngine.h"
#include "gm_main.h"
#include "Manager/Scene/SceneManager.h"
#include "SceneTitle/SceneTitle.h"
#include "ScenePlay/ScenePlay.h"
#include "ScenePlay/Character/Enemy/EnemyZakoBase.h"
#include "ScenePlay/Character/Player/Player.h"


void LockCursorToWindow() {

	HWND hwnd = GetMainWindowHandle(); // DX���C�u�����ŃE�B���h�E�n���h�����擾
	RECT rect;
	GetClientRect(hwnd, &rect); // �N���C�A���g�̈�̃T�C�Y���擾

	POINT ul;
	ul.x = rect.left;
	ul.y = rect.top;

	POINT lr;
	lr.x = rect.right;
	lr.y = rect.bottom;

	MapWindowPoints(hwnd, nullptr, &ul, 1); // �N���C�A���g�̈�̍���̃X�N���[�����W�ւ̕ϊ�
	MapWindowPoints(hwnd, nullptr, &lr, 1); // �N���C�A���g�̈�̉E���̃X�N���[�����W�ւ̕ϊ�

	RECT clipRect;
	clipRect.left = ul.x;
	clipRect.top = ul.y;
	clipRect.right = lr.x;
	clipRect.bottom = lr.y;

	ClipCursor(&clipRect); // �J�[�\�������̗̈�ɐ���
}


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {

	srand(time(0));

	//�w�i�F
	SetBackgroundColor(64, 64, 64);

	SetDefaultFontState("�l�r �o����", 20, 2);

	//LockCursorToWindow();
	SetMouseDispFlag(true);

	SceneManager::GetInstance(new SceneTitle());
}

bool showCursor = true;

//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	SceneManager::GetInstance()->Update(delta_time);

	// �Q�[�������I���@
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE) || tnl::Input::IsPadDownTrigger(ePad::KEY_8)) exit(1);

	// �}�E�X�J�[�\���@ON OFF
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) showCursor = !showCursor;

	if (showCursor) SetMouseDispFlag(true);
	else			SetMouseDispFlag(false);

	//SetFontSize(18);
	//DrawStringEx(965, 700, -1, "Tab�L�[�ŃJ�[�\�� ON Off �؂�ւ�");
	//SetFontSize(20);

	//DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


// ���@[ �d�v ]�@std::shared_ptr �Ŋm�ۂ��� dxe::Particle �͕K�� gameEnd() ��
// �@�@�^�C�~���O�܂łɎQ�ƃJ�E���^�� 0 �ɂȂ�悤���Z�b�g���Ă������� ( gameEnd �֐����Q�� )//
// ���@[ �d�v ]�@���̋@�\�� DxLib �̋@�\�ł͂���܂���
// �@�@dxe::Particle �� dxe::InstMeshPool �Ȃ� DirectX �𒼐ڐ��䂷��N���X�� render �֐���
//�@�@ dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() �ň͂������ŃR�[�����Ă�������  
//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

	inl::EnemyZakoBase::_explode_particle.reset();
	ScenePlay::GetInstance()->_player->_bombParticle.reset();
	ScenePlay::GetInstance()->_player->_playerParticle.reset();
	ScenePlay::_weatherParticle.reset();
}