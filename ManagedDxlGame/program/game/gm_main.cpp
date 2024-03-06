#include "DxLibEngine.h"
#include "gm_main.h"
#include "Manager/Scene/SceneManager.h"
#include "SceneTitle/SceneTitle.h"
#include "SceneSelectDifficulty/SceneSelectDifficulty.h"
#include "ScenePlay/ScenePlay.h"
#include "ScenePlay/Character/Enemy/EnemyBase.h"
#include "ScenePlay/Character/Enemy/EnemyZakoBase.h"
#include "ScenePlay/Character/Player/Player.h"


void LockCursorToWindow() {

    HWND hwnd = GetMainWindowHandle(); // DXライブラリでウィンドウハンドルを取得
    RECT rect;
    GetClientRect(hwnd, &rect); // クライアント領域のサイズを取得

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(hwnd, nullptr, &ul, 1); // クライアント領域の左上のスクリーン座標への変換
    MapWindowPoints(hwnd, nullptr, &lr, 1); // クライアント領域の右下のスクリーン座標への変換

    RECT clipRect;
    clipRect.left = ul.x;
    clipRect.top = ul.y;
    clipRect.right = lr.x;
    clipRect.bottom = lr.y;

    ClipCursor(&clipRect); // カーソルをこの領域に制限
}


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {

    srand(time(0));

    //背景色
    SetBackgroundColor(64, 64, 64);

    SetDefaultFontState("ＭＳ Ｐ明朝", 20, 2);

    SetMouseDispFlag(false);
    LockCursorToWindow();

    SceneManager::GetInstance(new SceneTitle());
}


bool isFullScreen = true;
//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

    SceneManager::GetInstance()->Update(delta_time);

    //if (tnl::Input::IsKeyDown(eKeys::KB_ESCAPE)) isFullScreen = !isFullScreen;

    //if (isFullScreen) ChangeWindowMode(false);
    //else              ChangeWindowMode(true);

    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) exit(1);

    //DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


// ※　[ 重要 ]　std::shared_ptr で確保した dxe::Particle は必ず gameEnd() の
// 　　タイミングまでに参照カウンタが 0 になるようリセットしてください ( gameEnd 関数を参照 )
//
// ※　[ 重要 ]　この機能は DxLib の機能ではありません
// 　　dxe::Particle や dxe::InstMeshPool など DirectX を直接制御するクラスの render 関数は
//　　 dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() で囲った中でコールしてください  
//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
    EnemyZakoBase::_explode_particle.reset();
    ScenePlay::_weather_particle.reset();
    Player::_bombParticle.reset();
}