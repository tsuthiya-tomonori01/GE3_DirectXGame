#include "ImGuiManager.h"

#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_dx12.h"
#include "External/imgui/imgui_impl_win32.h"

void ImGuiManager::Initialize(HWND hwnd, DirectXCommon* dxCommon) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
	    dxCommon->GetDevice(), dxCommon->GetSwapChainDesc().BufferCount,
	    dxCommon->GetRtvDesc().Format, dxCommon->GetSrvDescriptorHeap(),
	    dxCommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
	    dxCommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart()
	);
}

void ImGuiManager::NewFrame() {

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::CreateCommand() { ImGui::Render(); }

void ImGuiManager::CommandsExcute(ID3D12GraphicsCommandList* commandList) {

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}

ImGuiManager* ImGuiManager::Create() { 

	ImGuiManager* instance = new ImGuiManager();
	
	return instance; 
}

ImGuiManager::~ImGuiManager() {

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void ImGuiManager::ShowDemo() { ImGui::ShowDemoWindow(); }
