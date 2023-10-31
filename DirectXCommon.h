#pragma once
#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <chrono>

#include <vector>

#include "WinApp.h"

class DirectXCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Initialize(WinApp* winApp);

	void PreDraw();

	void PostDraw();

	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

private:
	void DeviceInitialize();

	void CommandInitialize();

	void SwapChainInitialize();

	void RenderTargetInitialize();

	void DepthBufferInitialize();

	void FenceInitialize();

private:
	WinApp* winApp = nullptr;

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;

	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ComPtr<IDXGISwapChain4> swapChain;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12Resource> depthBuff;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};
};

