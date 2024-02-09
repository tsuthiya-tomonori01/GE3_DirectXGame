#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "SpriteCommon.h"
#include "Sprite.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;

    //WindowsAPI初期化処理
    
    winApp_ = new WinApp();
    winApp_->Initialize();

#pragma region DirectX初期化処理
    dxCommon_ = new DirectXCommon();
    dxCommon_->Initialize(winApp_);

    // DirectX初期化処理　ここまで
#pragma endregion

    input_ = new Input();
    input_->Initialize(winApp_);

    SpriteCommon* Common = new SpriteCommon;
	Common->Initialize(dxCommon_);

    Sprite* sprite = new Sprite;
	sprite->Initialize(dxCommon_, Common);

    // ゲームループ
    while (true) {

        if (winApp_->update() == true) {
            break;
        }
       
        input_->Update();

        dxCommon_->PreDraw();

        sprite->Draw();

        dxCommon_->PostDraw();

    }

	delete sprite;
	delete Common;

    delete input_;

    delete dxCommon_;

    winApp_->Finalize();
    delete winApp_;

    return 0;
}
