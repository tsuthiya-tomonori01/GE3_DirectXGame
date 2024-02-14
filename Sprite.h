#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>

#include <DirectXMath.h>

#include "SpriteCommon.h"

class Sprite {

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	struct  Transform {
	
	   DirectX::XMFLOAT3 scale;
	   DirectX::XMFLOAT3 rotate;
	   DirectX::XMFLOAT3 translate;
	};

	struct VertexData {
	   DirectX::XMFLOAT4 position;
	   DirectX::XMFLOAT2 texcoord;
	};

	struct MaterialData {
	   DirectX::XMFLOAT4 color;
	   DirectX::XMMATRIX uvTransform;
	};

public:
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* common, std::wstring textureFilePath);
	void Update();
	void Draw();

	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation()             { return rotation; }
	DirectX::XMFLOAT4 GetColor()    { return color_; }
	DirectX::XMFLOAT2 GetSize()     { return size; }
	//アンカーポイント
	DirectX::XMFLOAT2 GetAnchorPoint() { return anchorPoint; }
	bool GetFlipX() { return isFlipX_; }
	bool GetFlipY() { return isFlipY_; }

	DirectX::XMFLOAT2 GetTextureLeftTop() { return textureLeftTop; }
	DirectX::XMFLOAT2 GetTextureSize() { return textureSize; }

	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot)             { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color)  { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size)    { this->size = size; }
	void SetanchorPoint(DirectX::XMFLOAT2 anchor) { anchorPoint = anchor; }
	void SetFlipX(bool isFlag) { isFlipX_ = isFlag; }
	void SetFlipY(bool isFlag) { isFlipY_ = isFlag; }

	void SetTextureLeftTop(DirectX::XMFLOAT2 value) { textureLeftTop = value; }
	void SetTextureSize(DirectX::XMFLOAT2 size) { textureSize = size; }

	void SetTexture(std::wstring textureFilePath);

private:

	void CreateVertex();

	void CreateIndex();

	void CreateMaterial();

	void CreateWVP();

	void AdjustTextureSize();

private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;

	ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	ComPtr<ID3D12Resource> materialResource;
	MaterialData* materialData = nullptr;

	ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	DirectX::XMFLOAT4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	Transform uvTransform = { {1, 1, 1}, {0, 0, 0}, {0, 0, 0} };
	
	Transform transform = {{1, 1, 1}, {0, 0, 0}, {0, 0, 0}};
	DirectX::XMFLOAT2 position = {0, 0};
	float rotation = 0;

	DirectX::XMFLOAT2 size = {512, 512};

	DirectX::XMFLOAT2 anchorPoint = {0, 0};
	bool isFlipX_ = true;
	bool isFlipY_ = false;

	DirectX::XMFLOAT2 textureLeftTop = {0, 0};
	DirectX::XMFLOAT2 textureSize = {0, 0};

	uint32_t textureIndex = 0;

	Transform cameraTransform = {{1,1,1}, {0,0,0}, {0,0,-5}};
};
