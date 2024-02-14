#pragma once
#include "DirectXCommon.h"
#include <string>
#include <DirectXTex.h>

class TextureManager {
private:
	struct TextureData {
		std::wstring filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

public:

	static TextureManager* GetInstance();

	void Finalize();

	void Initialize(DirectXCommon* dxCommon);

	void LoadTexture(const std::wstring& filePath);

	uint32_t GetTextureIndexByFilePath(const std::wstring& filePath);

	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

private:
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:
	static TextureManager* instance;

	static uint32_t kSRVIndexTop;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

private:

	DirectXCommon* dxCommon_ = nullptr;

	std::vector<TextureData> textureDatas;
};
