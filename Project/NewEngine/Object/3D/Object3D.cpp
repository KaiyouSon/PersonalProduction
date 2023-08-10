#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "ShadowMap.h"

using namespace ConstantBufferData;

bool Object3D::isAllLighting = false;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), offset(0, 0), tiling(1, 1),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	mTexture(TextureManager::GetTexture("White")), mModel(nullptr), mParent(nullptr),
	mDissolveTex(TextureManager::GetTexture("DissolveTexture")),
	mDepthTex(TextureManager::GetRenderTexture("ShadowMap")->depthTexture.get()),
	isLighting(false), mIsWriteShadow(false), mIsWriteDepth(false),
	mCamera(&Camera::current),
	isUseDissolve(false), dissolve(0.f), colorPower(1), dissolveColor(Color::red)
{
	// マテリアルの初期化
	MaterialInit();

	mTexture->isMaterial = true;

	if (isAllLighting == true)
	{
		isLighting = true;
	}

	mWhiteTex = TextureManager::GetTexture("White");
}

void Object3D::Update(Transform* parent)
{
	// カメラが設定してない場合
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	mTransform.pos = pos;
	mTransform.scale = scale;
	mTransform.rot = rot;
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}
	else if (mParent != nullptr)
	{
		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	if (mIsWriteDepth == true)
	{
		ShadowMap::Bind(*this);
	}

	// マテリアルの転送
	MaterialTransfer();
}
void Object3D::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr || mModel == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mModel->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(mModel->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();
	LightManager::GetInstance()->DrawCommand(5);

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetDescriptorTableStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetGpuHandle());

	if (isUseDissolve == true)
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((uint32_t)startIndex + 1, mDissolveTex->GetGpuHandle());
	}
	else
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((uint32_t)startIndex + 1, mWhiteTex->GetGpuHandle());
	}

	if (mIsWriteShadow == true)
	{
		CD3DX12_RESOURCE_BARRIER barrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				mDepthTex->buffer.Get(),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
		renderBase->GetCommandList()->ResourceBarrier(1, &barrier);

		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((uint32_t)startIndex + 2, mDepthTex->GetGpuHandle());
	}
	else
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((uint32_t)startIndex + 2, mWhiteTex->GetGpuHandle());
	}

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mModel->mesh.indices.size(), 1, 0, 0, 0);
	if (mIsWriteShadow == true)
	{
		CD3DX12_RESOURCE_BARRIER barrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				mDepthTex->buffer.Get(),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
		renderBase->GetCommandList()->ResourceBarrier(1, &barrier);
	}
}

// --- マテリアル関連 --------------------------------------------------- //
void Object3D::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3DShadow>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// マテリアルカラー
	iConstantBuffer = std::make_unique<ConstantBuffer<CMaterialColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// スキニング
	iConstantBuffer = std::make_unique<ConstantBuffer<CSkin>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV情報
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// ディゾルブ
	iConstantBuffer = std::make_unique<ConstantBuffer<CDissolve>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 影
	iConstantBuffer = std::make_unique<ConstantBuffer<CShadowMap>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void Object3D::MaterialTransfer()
{
	// マトリックス
	CTransform3DShadow transform3DShadowData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetPerspectiveProjectionMat(),
		ShadowMap::GetLightCamera().GetViewLookToMat() * ShadowMap::GetLightCamera().GetOrthoGrphicProjectionMat(),
		mTransform.GetWorldMat(),
		mCamera->pos
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DShadowData);

	if (mModel == nullptr)
	{
		return;
	}

	// マテリアルカラー
	CMaterialColor materialColorData;
	if (isLighting == true && isAllLighting == true)
	{
		materialColorData =
		{
			Color::one - 0.4f,
			mModel->material.diffuse,
			mModel->material.specular,
		};
	}
	else
	{
		materialColorData = { Color::one, Color::zero, Color::zero };
	}
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), materialColorData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), colorData);

	// スキン情報
	if (mModel->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModel);
		fbxModel->PlayAnimetion();

		CSkin skinData{};
		for (uint32_t i = 0; i < fbxModel->bones.size(); i++)
		{
			skinData.bones[i] = fbxModel->bones[i].currentMat;
		}
		TransferDataToConstantBuffer(mMaterial.constantBuffers[3].get(), skinData);
	}

	// UVデータ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[4].get(), uvData);

	// ディゾルブ
	CDissolve dissolveData = { dissolve,colorPower,Vec2(0,0), dissolveColor.To01() };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[5].get(), dissolveData);

	// ディゾルブ
	CShadowMap shadowMapData = { mIsWriteShadow };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[6].get(), shadowMapData);
}
void Object3D::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mMaterial.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mMaterial.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, mMaterial.constantBuffers[2]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		3, mMaterial.constantBuffers[3]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, mMaterial.constantBuffers[4]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		6, mMaterial.constantBuffers[5]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		7, mMaterial.constantBuffers[6]->constantBuffer->GetGPUVirtualAddress());
}

// --- セッター -------------------------------------------------------- //

// モデル
void Object3D::SetModel(Model* model)
{
	mModel = model;
	mTexture = mModel->texture;

	// パイプライン変更
	if (mModel->format == ModelFormat::Obj)
	{
		mGraphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
	}
	if (mModel->format == ModelFormat::Fbx)
	{
		mGraphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

// テクスチャー
void Object3D::SetTexture(Texture* texture) { mTexture = texture; }

// グラフィックスパイプライン
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// アニメーション
void Object3D::SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay)
{
	// スキン情報
	if (mModel->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModel);

		fbxModel->animation.index = animationIndex;
		fbxModel->animation.timer.SetLimitTimer(maxFrame);
		fbxModel->animation.isPlay = isPlay;
	}
}

// カメラ
void Object3D::SetCamera(Camera* camera)
{
	mCamera = camera;
}

// 影
void Object3D::SetisShadow(const bool isWriteShadow, const bool isWriteDepth)
{
	mIsWriteShadow = isWriteShadow;
	mIsWriteDepth = isWriteDepth;

	if (mIsWriteDepth == true)
	{
		ShadowMap::Register();
	}
}

// 親
void Object3D::SetParent(Transform* parent)
{
	mParent = parent;
}

void Object3D::SetBillboardType(const BillboardType type)
{
	mTransform.SetBillboardType(type);
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 Object3D::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ワールドスケール
Vec3 Object3D::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// トランスフォーム
Transform Object3D::GetTransform()
{
	return mTransform;
}

// 親
Transform* Object3D::GetParent()
{
	return mParent;
}

// モデル
Model* Object3D::GetModel() { return mModel; }
