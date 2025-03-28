#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "Camera.h"
#include "Renderer.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Sprite::Sprite() :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>())
{
	InitComponents();

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
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>())
{
	this->name = name;

	InitComponents();

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
	if (!CheckActive())
	{
		return;
	}

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
	if (!CheckActive())
	{
		return;
	}

	gCurrentScene->GetRenderer()->Register(layerTag,
		[this]()
		{
			DrawCommands();
		});
}
void Sprite::Draw(const std::string& _layerTag, const BlendMode _blendMode)
{
	if (isActive == false)
	{
		return;
	}

	blendMode = _blendMode;
	_layerTag;

	//Renderer::GetInstance()->Register(_layerTag,
	//	[this]()
	//	{
	//		DrawCommands();
	//	});
	DrawCommands();
}

void Sprite::Copy(GameObject* gameObj)
{
	isActive = gameObj->isActive;
	layerTag = gameObj->layerTag;
	color = gameObj->color;
	mComponentManager->Copy(gameObj->GetComponentManager());
}

// --- マテリアル関連 --------------------------------------------------- //
void Sprite::MaterialInit()
{
	if (mMaterial->constantBuffers.empty())
	{
		mMaterial->AddConstantBuffer<CTransform2D>();	// 2D行列
		mMaterial->AddConstantBuffer<CColor>();			// 色
	}
	else
	{
		mMaterial->SetConstantBuffer<CTransform2D>(0);	// 2D行列
		mMaterial->SetConstantBuffer<CColor>(1);		// 色
	}
	mMaterial->Init();
}
void Sprite::MaterialTransfer()
{
	if (!mMaterial)
	{
		return;
	}

	// マトリックス
	CTransform2D transform2DData =
	{
		mTransform->GetWorldMat() * Camera::current.GetOrthogrphicMat()
	};
	mMaterial->TransferDataToConstantBuffer(0, transform2DData);

	// 色データ
	CColor colorData = { color.To01() };
	mMaterial->TransferDataToConstantBuffer(1, colorData);
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

	ITexture* tex = mTextureComponent->GetTexture();

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

void Sprite::InitComponents()
{
	layerTag = "BackSprite";
	mType = GameObjectType::Sprite;

	mComponentManager->AddComponent<SpriteInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<TextureComponent>();
	mComponentManager->AddComponent<MaterialComponent>();
	mComponentManager->AddComponent<ScriptsComponent>();

	mInfo = mComponentManager->GetComponent<SpriteInfo>();
	mTransform = mComponentManager->GetComponent<Transform>();
	mTextureComponent = mComponentManager->GetComponent<TextureComponent>();
	mMaterialComponent = mComponentManager->GetComponent<MaterialComponent>();
	mScriptsComponent = mComponentManager->GetComponent<ScriptsComponent>();

	mMaterialComponent->SetMaterial("BasicSprite");
	mMaterial = mMaterialComponent->GetMaterial();
}

// --- 描画コマンド ----------------------------------------------------- //
void Sprite::DrawCommands()
{
	ITexture* tex = mTextureComponent->GetTexture();
	if (tex == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// マテリアルの描画コマンド
	mMaterial->DrawCommands(mTextureComponent);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());
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

	mTextureComponent->SetTexture(texture);
	if (isChangeSize)
	{
		SetSize(Vec2(tex->GetInitalSize().x, tex->GetInitalSize().y));
	}
}

// テクスチャー
void Sprite::SetTexture(const std::string& textureTag, const bool isChangeSize)
{
	ITexture* tex = gAssetsManager->GetTexture(textureTag);
	if (!tex)
	{
		return;
	}

	mTextureComponent->SetTexture(textureTag);
	if (isChangeSize)
	{
		Vec2 texSize = Vec2(tex->GetInitalSize().x, tex->GetInitalSize().y);

		SetSize(texSize);
		SetTextureRect(0, texSize);
	}
}

// 描画範囲
void Sprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	mInfo->rect = RectAngle(leftTopPos, rightDownPos);

	TransferUVCoord(leftTopPos, rightDownPos);
	mVertexBuffer->TransferToBuffer(mVertices);
}

void Sprite::SetTextureRect(RectAngle rect)
{
	SetTextureRect(rect.GetLeftTop(), rect.GetRightBottom());
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
	graphicsPipeline;
	//mGraphicsPipeline = graphicsPipeline;
}
