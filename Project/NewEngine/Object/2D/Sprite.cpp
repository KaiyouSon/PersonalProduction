#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "Camera.h"
#include "Renderer.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Sprite::Sprite() :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mMaterial(std::make_unique<Material>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Sprite"))
{
	InitToSprite();
	mInfo = mComponentManager->GetComponent<SpriteInfo>();

	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

Sprite::Sprite(const std::string& name) :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mMaterial(std::make_unique<Material>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Sprite"))
{
	this->name = name;

	InitToSprite();
	mInfo = mComponentManager->GetComponent<SpriteInfo>();

	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

void Sprite::Update()
{
	BaseUpdate();

	// マテリアルの転送
	MaterialTransfer();

	// 頂点データの転送
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Sprite::ExecuteCS()
{
}
void Sprite::AppedToRenderer()
{
	Renderer::GetInstance()->Register("BackSprite",
		[this]()
		{
			DrawCommands();
		});
}
void Sprite::Draw(const std::string& _layerTag, const BlendMode _blendMode)
{
	blendMode = _blendMode;
	_layerTag;


	//Renderer::GetInstance()->Register(_layerTag,
	//	[this]()
	//	{
	//		DrawCommands();
	//	});
	DrawCommands();
}

// --- マテリアル関連 --------------------------------------------------- //
void Sprite::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 2D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void Sprite::MaterialTransfer()
{
	// マトリックス
	CTransform2D transform2DData =
	{
		mTransform->GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transform2DData);

	// 色データ
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);
}
void Sprite::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- 頂点データ関連 --------------------------------------------------- //
void Sprite::TransferVertexCoord()
{
	enum class Point { LD, LU, RD, RU };

	// 四辺
	float left = (0.f - mInfo->anchorPoint.x) * mInfo->spriteSize.x;
	float right = (1.f - mInfo->anchorPoint.x) * mInfo->spriteSize.x;
	float up = (0.f - mInfo->anchorPoint.y) * mInfo->spriteSize.y;
	float down = (1.f - mInfo->anchorPoint.y) * mInfo->spriteSize.y;

	switch (mInfo->flipType)
	{
	case FlipType::X:
		left = -left;
		right = -right;
		break;

	case FlipType::Y:
		up = -up;
		down = -down;
		break;

	case FlipType::XY:
		left = -left;
		right = -right;
		up = -up;
		down = -down;
		break;

	default:
		break;
	}

	// 頂点座標
	mVertices[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  //左下
	mVertices[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  //左上
	mVertices[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  //右下
	mVertices[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  //右上
}
void Sprite::TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	enum class Point { LD, LU, RD, RU };

	ITexture* tex = mTextureData->GetTexture();

	// 四辺
	float left = leftTopPos.x / tex->GetInitalSize().x;
	float right = rightDownPos.x / tex->GetInitalSize().x;
	float up = leftTopPos.y / tex->GetInitalSize().y;
	float down = rightDownPos.y / tex->GetInitalSize().y;

	// uv座標
	mVertices[(uint32_t)Point::LD].uv = Vec2(left, down);	 //左下
	mVertices[(uint32_t)Point::LU].uv = Vec2(left, up);		 //左上
	mVertices[(uint32_t)Point::RD].uv = Vec2(right, down);	 //右下
	mVertices[(uint32_t)Point::RU].uv = Vec2(right, up);	 //右上
}

// --- 描画コマンド ----------------------------------------------------- //
void Sprite::DrawCommands()
{
	ITexture* tex = mTextureData->GetCurrentTexture();
	if (tex == nullptr)
	{
		return;
	}


	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	uint32_t endIndex = mGraphicsPipeline->GetRootSignature()->GetUAVStartIndex();

	for (uint32_t i = startIndex; i < endIndex; i++)
	{
		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			startIndex, tex->GetBufferResource()->srvHandle.gpu);
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- セッター -------------------------------------------------------- //

void Sprite::SetTexture(Texture* texture, const bool isChangeSize)
{
	ITexture* tex = texture;
	if (!tex)
	{
		return;
	}

	mTextureData->SetCurrentTexture(texture);
	if (isChangeSize)
	{
		SetSize(Vec2(tex->GetInitalSize().x, tex->GetInitalSize().y));
	}
}

// テクスチャー
void Sprite::SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize)
{
	//ITexture* tex = TextureManager::GetTexture(textureTag);
	ITexture* tex = gAssetsManager->GetTexture(textureTag);
	if (!tex)
	{
		return;
	}

	mTextureData->SetCurrentTexture(textureTag);
	if (isChangeSize)
	{
		SetSize(Vec2(tex->GetInitalSize().x, tex->GetInitalSize().y));
	}
}

// 描画範囲
void Sprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	TransferUVCoord(leftTopPos, rightDownPos);
	mVertexBuffer->TransferToBuffer(mVertices);
}

// サイズ
void Sprite::SetSize(const Vec2 size)
{
	mInfo->spriteSize = size;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// アンカーポイント
void Sprite::SetAnchorPoint(const Vec2 anchorPoint)
{
	mInfo->anchorPoint = anchorPoint;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// 画像反転
void Sprite::SetFlipType(const FlipType flipType)
{
	mInfo->flipType = flipType;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// グラフィックスパイプライン
void Sprite::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
