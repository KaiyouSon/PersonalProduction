#include "ColliderObject.h"
#include "RenderBase.h"
#include "ColliderDrawer.h"
#include "Collider.h"

using namespace ConstantBufferData;

ColliderObject::ColliderObject() :
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ColliderObject")),
	model(nullptr), mCamera(&Camera::current)
{
	// マテリアルの初期化
	MaterialInit();
}

void ColliderObject::Update()
{
	// カメラが設定してない場合
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	if (model == nullptr)
	{
		return;
	}

	transform.Update();

	// マテリアルの転送
	MaterialTransfer();
}
void ColliderObject::Draw()
{
	if (model == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)model->mesh.indices.size(), 1, 0, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void ColliderObject::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void ColliderObject::MaterialTransfer()
{
	// マトリックス
	CTransform3D transform3DData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetPerspectiveProjectionMat(),
		transform.GetWorldMat(),
		mCamera->pos,
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void ColliderObject::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->constantBuffer->GetGPUVirtualAddress());
	}
}

// --- セッター -------------------------------------------------------- //

// カメラ
void ColliderObject::SetCamera(Camera* camera)
{
	mCamera = camera;
}
