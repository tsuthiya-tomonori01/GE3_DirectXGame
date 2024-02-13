#include "Sprite.h"

#include <DirectXMath.h>

#include "BufferResource.h"

#include "External/imgui/imgui.h"

using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common) {

	dxCommon_ = dxCommon;
	common_ = common;

	DirectX::ScratchImage mipImages = common->LoadTexture(L"Resources/mario.jpg");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	common_->UploadTextureData(textureResource, mipImages);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();

	textureSrvHandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);

	CreateVertex();
	CreateMaterial();
	CreateWVP();
}

void Sprite::Update() {

	ImGui::Begin("Texture");
	ImGui::DragFloat3("Pos", &transform.translate.x, 0.1f);
	ImGui::End();
}

void Sprite::Draw() {

	//transform.rotate.y += 0.03f;
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));

	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);

	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);


	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));

	XMMATRIX cameraRotateAndScameMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);

	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotateAndScameMatrix, cameraTranslationMatrix);

	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);

	XMMATRIX proj = XMMatrixPerspectiveFovLH(
	    XMConvertToRadians(45.0f), 
		(float)WinApp::window_width/(float)WinApp::window_height, 
		0.1f, 
		100.0f
	);

	XMMATRIX worldViewPrjectionMatrix = worldMatrix * (view * proj);

	*wvpData = worldViewPrjectionMatrix;

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());

	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

	dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}

void Sprite::CreateVertex() {

	vertexResource = createBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = {-0.5f, -0.5f, 0.0f, 1.0f};
	vertexData[0].texcoord = {0.0f, 1.0f};

	vertexData[1].position = {-0.5f, +0.5f, 0.0f, 1.0f};
	vertexData[1].texcoord = {0.0f, 0.0f};

	vertexData[2].position = {0.5f, -0.5f, 0.0f, 1.0f};
	vertexData[2].texcoord = {1.0f, 1.0f};

	vertexData[3].position = {-0.5f, +0.5f, 0.0f, 1.0f};
	vertexData[3].texcoord = {0.0f, 0.0f};

	vertexData[4].position = {+0.5f, +0.5f, 0.0f, 1.0f};
	vertexData[4].texcoord = {1.0f, 0.0f};

	vertexData[5].position = {+0.5f, -0.5f, 0.0f, 1.0f};
	vertexData[5].texcoord = {1.0f, 1.0f};
}

void Sprite::CreateMaterial() {

	materialResource = createBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

	XMFLOAT4* materialDate = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialDate));

	*materialDate = color_;
}

void Sprite::CreateWVP() {

	wvpResource = createBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));

	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*wvpData = XMMatrixIdentity();

	
}
