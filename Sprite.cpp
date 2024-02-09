#include "Sprite.h"

#include <DirectXMath.h>

#include "BufferResource.h"


using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common) {

	dxCommon_ = dxCommon;
	common_ = common;

	CreateVertex();
	CreateMaterial();
	CreateWVP();
}

void Sprite::Draw() {

	transform.rotate.y += 0.03f;
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
	    XMConvertToRadians(45.0f), (float)WinApp::window_height, 
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

	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Sprite::CreateVertex() {

	vertexResource = createBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4) * 3);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(DirectX::XMFLOAT4) * 3;
	vertexBufferView.StrideInBytes = sizeof(DirectX::XMFLOAT4);

	XMFLOAT4* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
	vertexData[1] = {+0.0f, +0.5f, 0.0f, 1.0f};
	vertexData[2] = {+0.5f, -0.5f, 0.0f, 1.0f};

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
