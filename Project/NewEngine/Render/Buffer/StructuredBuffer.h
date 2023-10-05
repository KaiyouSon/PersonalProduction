#pragma once
#include "BufferResource.h"
#include "RenderBase.h"
#include <cassert>
#pragma warning(push)
#pragma warning(disable: 4061)
#include <d3dx12.h>
#pragma warning(pop)

class StructuredBuffer
{
private:
	std::unique_ptr<BufferResource> mBufferResource;

public:
	StructuredBuffer();

	template<typename T>
	void Create(T data)
	{
		mBufferResource = std::make_unique<BufferResource>();

		HRESULT result;

		// ヒープの設定
		D3D12_HEAP_PROPERTIES heapProp =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		// リソース設定
		D3D12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(T) + 0xff) & ~0xff,	// 256バイトアラインメント
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);			// UAV使用を許可

		// バッファの生成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp,	// ヒープの設定
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc, // リソースの設定
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mBufferResource->buffer));
		assert(SUCCEEDED(result));

		mBufferResource->buffer->SetName(L"StructuredBuffer");
	}

	BufferResource* GetBufferResource();
};

