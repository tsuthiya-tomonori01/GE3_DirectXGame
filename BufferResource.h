#pragma once
#include <d3d12.h>
#include <cassert>

ID3D12Resource* createBufferResource(ID3D12Device* device, size_t sizeInByte) {

	ID3D12Resource* resource = nullptr;

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = sizeInByte;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result = device->CreateCommittedResource(
	    &uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	return resource;
}
