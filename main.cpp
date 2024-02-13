#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "SpriteCommon.h"
#include "Sprite.h"

#include "ImGuiManager.h"

#include <vector>

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

    ImGuiManager* imgui = ImGuiManager::Create();
	ImGuiManager::Initialize(winApp_->GetHwnd(), dxCommon_);

    SpriteCommon* Common = new SpriteCommon;
	Common->Initialize(dxCommon_);

    std::vector<Sprite*> sprite;
	for (int i = 0; i < 5; i++) {
		Sprite* temp = new Sprite();
		temp->Initialize(dxCommon_, Common);
		temp->SetPosition({(float)i * 1, 0});
		sprite.push_back(temp);
	}
    // ゲームループ
    while (true) {

        if (winApp_->update() == true) {
            break;
        }
       
        ImGuiManager::NewFrame();
		imgui->ShowDemo();

        input_->Update();

        //移動
  //      DirectX::XMFLOAT2 pos = sprite->GetPosition();
		//pos.x += 0.03f;
		//sprite->SetPosition(pos);

  //      //回転
		//float rot = sprite->GetRotation();
		//rot += 0.01f;
		//sprite->SetRotation(rot);

  //      //色
		//DirectX::XMFLOAT4 color = sprite->GetColor();
		//color.x -= 0.01f;
		//if (color.x < 0) {
		//	color.x = 1.0f;
		//}
		//sprite->SetColor(color);

  //      //サイズ
		//DirectX::XMFLOAT2 size = sprite->GetSize();
		//size.y += 0.01f;
		//sprite->SetSize(size);

        for (int i = 0; i < 5; i++) {
			sprite[i]->Update();
		}

        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();

        for (int i = 0; i < 5; i++) {
			sprite[i]->Draw();
		}

        ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();

    }

    for (int i = 0; i < 5; i++) {
		delete sprite[i];
	}

    delete Common;

    delete imgui;

    delete input_;

    delete dxCommon_;

    winApp_->Finalize();
    delete winApp_;

    return 0;
}
