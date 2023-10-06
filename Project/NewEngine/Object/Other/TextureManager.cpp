#include "TextureManager.h"
#include "RenderBase.h"
#include <DirectXTex.h>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

using namespace DirectX;

TextureManager::TextureManager() : mMutex(std::mutex{})
{
}

void TextureManager::Init()
{
}

// 繝・け繧ｹ繝√Ε繝ｼ縺ｮ蜿門ｾ・
Texture* TextureManager::GetTexture(const std::string tag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// 繝ｭ繧ｰ
	std::string log = "Error";

	if (GetInstance()->mTextureMap[tag].get() == nullptr)
	{
		log = "[Texture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[Texture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// 濶ｲ繧呈欠螳壹＠縺ｦ繝・け繧ｹ繝√Ε繧堤函謌舌＠繝槭ャ繝励↓譬ｼ邏阪☆繧・
Texture* TextureManager::CreateTexture(const Color color, const std::string tag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// 繝・け繧ｹ繝√Ε繝ｼ菴懈・
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	HRESULT result;

	// 繝偵・繝励・險ｭ螳・
	CD3DX12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1, 1, 1, 1, 1);

	// 繝・け繧ｹ繝√Ε縺ｮ繧ｵ繧､繧ｺ繧偵そ繝・ヨ
	GetInstance()->mTextureMap[tag]->size =
		Vec2((float)textureResourceDesc.Width, (float)textureResourceDesc.Height);

	// 繝・け繧ｹ繝√Ε繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer));
	assert(SUCCEEDED(result));

	// SRV菴懈・
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(GetInstance()->mTextureMap[tag]->GetBufferResource());
	//GetInstance()->CreateSRV(*GetInstance()->mTextureMap[tag], GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get());

	// 濶ｲ
	Color col = color / 255.f;

	D3D12_SUBRESOURCE_DATA subResourcesData{};
	subResourcesData.pData = (void**)&col;
	subResourcesData.RowPitch =
		(LONG_PTR)(sizeof(Color) *
			GetInstance()->mTextureMap[tag]->size.x);
	subResourcesData.SlicePitch =
		(LONG_PTR)(sizeof(Color) *
			GetInstance()->mTextureMap[tag]->size.x *
			GetInstance()->mTextureMap[tag]->size.y);

	uint64_t uploadSize = GetRequiredIntermediateSize(
		GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get(), 0, 1);

	// 繝偵・繝励・險ｭ螳・
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// 繝・け繧ｹ繝√Ε繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get(),
		GetInstance()->mTextureMap[tag]->uploadBuffer.Get(),
		0,
		0,
		1,
		&subResourcesData);

	GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer->SetName(L"ResourceBuffer");
	GetInstance()->mTextureMap[tag]->uploadBuffer->SetName(L"UploadBuffer");

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	// 繝ｭ繧ｰ蜃ｺ蜉・
	std::string log = "[Texture Create] ColorTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// 繝輔ぃ繧､繝ｫ繝代せ繧呈欠螳壹＠縺ｦ繝・け繧ｹ繝√Ε繧堤函謌舌＠繝槭ャ繝励・譬ｼ邏阪☆繧・
Texture* TextureManager::LoadTexture(const std::string filePath, const std::string tag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// 繝・け繧ｹ繝√Ε繝ｼ菴懈・
	GetInstance()->mTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	std::string path = "Application/Resources/Texture/" + filePath;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(path.begin(), path.end());

	HRESULT result;

	// WIC繝・け繧ｹ繝√Ε縺ｮ繝ｭ繝ｼ繝・
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "繝・け繧ｹ繝√Ε繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励＠縺ｾ縺励◆");
	}

	// 繝溘ャ繝励・繝・・逕滓・
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 隱ｭ縺ｿ霎ｼ繧薙□繝・ぅ繝輔Η繝ｼ繧ｺ繝・け繧ｹ繝√Ε繧担RGB縺ｨ縺励※謇ｱ縺・
	metadata.format = MakeSRGB(metadata.format);

	// 繝偵・繝励・險ｭ螳・
	CD3DX12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			(UINT64)metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels,
			1);

	// 繝・け繧ｹ繝√Ε縺ｮ繧ｵ繧､繧ｺ繧偵そ繝・ヨ
	GetInstance()->mTextureMap[tag]->size =
		Vec2((float)textureResourceDesc.Width, (float)textureResourceDesc.Height);

	// 繝・け繧ｹ繝√Ε繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer));
	assert(SUCCEEDED(result));

	// SRV菴懈・
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(GetInstance()->mTextureMap[tag]->GetBufferResource());

	// SRV菴懈・
	//GetInstance()->CreateSRV(*GetInstance()->mTextureMap[tag], GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get());

	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas{};
	subResourcesDatas.resize(metadata.mipLevels);

	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// 蜈ｨ繝溘ャ繝励・繝・・繝ｬ繝吶Ν繧呈欠螳壹＠縺ｦ繧､繝｡繝ｼ繧ｸ繧貞叙蠕・
		const Image* img = scratchImg.GetImage(i, 0, 0);

		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;
	}

	uint64_t uploadSize =
		GetRequiredIntermediateSize(GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get(), 0, (UINT)metadata.mipLevels);

	// 繝偵・繝励・險ｭ螳・
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// 繝・け繧ｹ繝√Ε繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mTextureMap[tag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get(),
		GetInstance()->mTextureMap[tag]->uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	// 繝ｭ繧ｰ蜃ｺ蜉・
	std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// obj繝輔ぃ繧､繝ｫ縺九ｉ繝ｭ繝ｼ繝峨＠縺溘ユ繧ｯ繧ｹ繝√Ε繝ｼ繧偵Ο繝ｼ繝峨☆繧句ｰら畑髢｢謨ｰ
Texture* TextureManager::LoadMaterialTexture(const std::string filePath, const std::string tag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// 繝・け繧ｹ繝√Ε繝ｼ菴懈・
	GetInstance()->mMaterialTextureMap.
		insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::wstring wfilePath(filePath.begin(), filePath.end());

	HRESULT result;

	// WIC繝・け繧ｹ繝√Ε縺ｮ繝ｭ繝ｼ繝・
	result = LoadFromWICFile(
		wfilePath.c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (result != S_OK)
	{
		std::string log = "[Texture Load] FilePath : " + filePath + ", Tag : " + tag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "繝・け繧ｹ繝√Ε繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励＠縺ｾ縺励◆");
	}

	// 繝溘ャ繝励・繝・・逕滓・
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 隱ｭ縺ｿ霎ｼ繧薙□繝・ぅ繝輔Η繝ｼ繧ｺ繝・け繧ｹ繝√Ε繧担RGB縺ｨ縺励※謇ｱ縺・
	metadata.format = MakeSRGB(metadata.format);

	// 繝偵・繝励・險ｭ螳・
	CD3DX12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			(UINT64)metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels,
			1);

	// 繝・け繧ｹ繝√Ε縺ｮ繧ｵ繧､繧ｺ繧偵そ繝・ヨ
	GetInstance()->mMaterialTextureMap[tag]->size =
		Vec2((float)textureResourceDesc.Width, (float)textureResourceDesc.Height);

	// 繝・け繧ｹ繝√Ε繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mMaterialTextureMap[tag]->GetBufferResource()->buffer));
	assert(SUCCEEDED(result));

	// SRV菴懈・
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(GetInstance()->mMaterialTextureMap[tag]->GetBufferResource());

	std::vector<D3D12_SUBRESOURCE_DATA> subResourcesDatas{};
	subResourcesDatas.resize(metadata.mipLevels);

	for (size_t i = 0; i < subResourcesDatas.size(); i++)
	{
		// 蜈ｨ繝溘ャ繝励・繝・・繝ｬ繝吶Ν繧呈欠螳壹＠縺ｦ繧､繝｡繝ｼ繧ｸ繧貞叙蠕・
		const Image* img = scratchImg.GetImage(i, 0, 0);

		subResourcesDatas[i].pData = img->pixels;
		subResourcesDatas[i].RowPitch = img->rowPitch;
		subResourcesDatas[i].SlicePitch = img->slicePitch;
	}

	uint64_t uploadSize =
		GetRequiredIntermediateSize(GetInstance()->mMaterialTextureMap[tag]->GetBufferResource()->buffer.Get(), 0, (UINT)metadata.mipLevels);

	// 繝偵・繝励・險ｭ螳・
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	// 繝・け繧ｹ繝√Ε繝舌ャ繝輔ぃ縺ｮ逕滓・
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&GetInstance()->mMaterialTextureMap[tag]->uploadBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		RenderBase::GetInstance()->GetCommandList(),
		GetInstance()->mMaterialTextureMap[tag]->GetBufferResource()->buffer.Get(),
		GetInstance()->mMaterialTextureMap[tag]->uploadBuffer.Get(),
		0,
		0,
		(UINT)metadata.mipLevels,
		subResourcesDatas.data());

	D3D12_RESOURCE_BARRIER  barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = GetInstance()->mMaterialTextureMap[tag]->GetBufferResource()->buffer.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	RenderBase::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	std::string log = "[MaterialTexture Load] FilePath : " + filePath + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return GetInstance()->mMaterialTextureMap[tag].get();
}

// 繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧｢繝ｳ繝ｭ繝ｼ繝蛾未謨ｰ
void TextureManager::UnLoadTexture(const std::string tag)
{
	auto it = GetInstance()->mTextureMap.find(tag);
	if (it == GetInstance()->mTextureMap.end())
	{
		return;
	}

	// 繝薙Η繝ｼ蜑企勁
	DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
		GetInstance()->mTextureMap[tag]->GetBufferResource());

	// 繝槭ャ繝励°繧牙炎髯､
	GetInstance()->mTextureMap.erase(tag);
}

// 豺ｱ蠎ｦ繝・け繧ｹ繝√Ε繝ｼ繧堤函謌・
Texture* TextureManager::CreateDepthTexture()
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	std::string tag = "DepthTexture";
	GetInstance()->mTextureMap.insert(std::make_pair(tag, std::move(std::make_unique<Texture>())));

	// 豺ｱ蠎ｦ繝舌ャ繝輔ぃ縺ｮ繝ｪ繧ｽ繝ｼ繧ｹ
	GetInstance()->mTextureMap[tag]->GetBufferResource()->buffer = RenderBase::GetInstance()->GetDepthBuffer()->GetBufferResource()->buffer;

	// SRV菴懈・
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(GetInstance()->mTextureMap[tag]->GetBufferResource());

	std::string log = "[Texture Create] DepthTexture, Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mTextureMap[tag].get();
}

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ蜿門ｾ・
RenderTexture* TextureManager::GetRenderTexture(const std::string tag)
{
	std::string log;
	if (GetInstance()->mRenderTextureMap[tag].get() == nullptr)
	{
		log = "[RenderTexture Use] Tag : " + tag + ", does not exist";
	}
	else
	{
		log = "[RenderTexture Use] Tag : " + tag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return GetInstance()->mRenderTextureMap[tag].get();
}

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ繧堤函謌舌＠繝槭ャ繝励↓譬ｼ邏阪☆繧・
RenderTexture* TextureManager::CreateRenderTexture(const Vec2 size, const uint32_t num, const std::string tag)
{
	// 謗剃ｻ門宛蠕｡
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	std::unique_ptr<RenderTexture> renderTex = std::make_unique<RenderTexture>();
	renderTex->size = size;
	renderTex->GetBufferResources()->resize(num);

	HRESULT result;
	RenderBase* renderBase = RenderBase::GetInstance();

	// 繝偵・繝苓ｨｭ螳・
	CD3DX12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 繝ｪ繧ｽ繝ｼ繧ｹ險ｭ螳・
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			(UINT64)size.x, (UINT)size.y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_CLEAR_VALUE clearValue =
		CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			RenderTexture::sClearColor);

	for (uint32_t i = 0; i < num; i++)
	{
		result = renderBase->GetDevice()->
			CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(&renderTex->GetBufferResources()->at(i).buffer));
		assert(SUCCEEDED(result));
	}

	for (uint32_t i = 0; i < num; i++)
	{
		// SRV菴懈・
		DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(&renderTex->GetBufferResources()->at(i));

		// RTV菴懈・
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(&renderTex->GetBufferResources()->at(i));
	}

	// DSV菴懈・
	renderTex->depthBuffer.Create(size);
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(renderTex->depthBuffer.GetBufferResource());

	renderTex->depthTexture = std::make_unique<Texture>();
	renderTex->depthTexture->GetBufferResource()->buffer = renderTex->depthBuffer.GetBufferResource()->buffer;
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(renderTex->depthTexture->GetBufferResource());

	GetInstance()->mRenderTextureMap.insert(std::make_pair(tag, std::move(renderTex)));

	std::string log = "[RenderTexture Create] Tag : " + tag + ", created";
	OutputDebugLog(log.c_str());

	return GetInstance()->mRenderTextureMap[tag].get();
}

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧｢繝ｳ繝ｭ繝ｼ繝蛾未謨ｰ
void TextureManager::UnLoadRenderTexture(const std::string tag)
{
	auto it = GetInstance()->mRenderTextureMap.find(tag);
	if (it == GetInstance()->mRenderTextureMap.end())
	{
		return;
	}


	for (uint32_t i = 0; i < GetInstance()->mRenderTextureMap[tag]->GetBufferResources()->size(); i++)
	{
		// 繝薙Η繝ｼ蜑企勁
		DescriptorHeapManager::GetDescriptorHeap("SRV")->DestroyView(
			&GetInstance()->mRenderTextureMap[tag]->GetBufferResources()->at(i));

		// 繝槭ャ繝励°繧牙炎髯､
		GetInstance()->mTextureMap.erase(tag);
	}
}

// 繝・け繧ｹ繝√Ε繝槭ャ繝励・蜿門ｾ・
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetTextureMap()
{
	return &GetInstance()->mTextureMap;
}

// 繝・け繧ｹ繝√Ε繝槭ャ繝励・蜿門ｾ・
std::unordered_map<std::string, std::unique_ptr<Texture>>* TextureManager::GetMaterialTextureMap()
{
	return &GetInstance()->mMaterialTextureMap;
}

// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝槭ャ繝励・蜿門ｾ・
std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* TextureManager::GetRenderTextureMap()
{
	return &GetInstance()->mRenderTextureMap;
}

// 繝・け繧ｹ繝√Ε繝ｼ繝ｭ繝ｼ繝牙ｾ後・繧ｳ繝槭Φ繝峨Μ繧ｹ繝医・螳溯｡・
void TextureManager::ExcuteComandList()
{
	ID3D12GraphicsCommandList* iCommandList = RenderBase::GetInstance()->GetCommandList();

	iCommandList->Close();

	ID3D12CommandQueue* iCommandQueue = RenderBase::GetInstance()->GetCommandQueue();

	ID3D12CommandList* list[] = { iCommandList };
	iCommandQueue->ExecuteCommandLists(1, list);

	RenderBase::GetInstance()->PreIncrimentFenceValue();

	// 繧ｳ繝槭Φ繝峨・螳溯｡悟ｮ御ｺ・ｒ蠕・▽
	iCommandQueue->Signal(RenderBase::GetInstance()->GetFence(), RenderBase::GetInstance()->GetFenceValue());

	if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != RenderBase::GetInstance()->GetFenceValue())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(RenderBase::GetInstance()->GetFenceValue(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	HRESULT result;

	// 繧ｭ繝･繝ｼ繧偵け繝ｪ繧｢
	result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// 蜀阪・繧ｳ繝槭Φ繝峨Μ繧ｹ繝医ｒ雋ｯ繧√ｋ貅門ｙ
	result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

