#pragma once
#include "NewEngine.h"

// 草のクラス
class Grass
{
private:
	std::vector<VertexBufferData::VGrass> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VGrass>> mVertexBuffer;
	std::vector<Timer> mTimers;
	Transform mTransform;
	Transform* mParent;
	Material mMaterial;
	std::unique_ptr<Material> mCSMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Billboard mBillboard;
	DepthTexture* mDepthTex;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Texture* texture;
	static ConstantBufferData::CPOM sPOMData;
	static ConstantBufferData::CMaterialColor sMaterialColor;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Grass();
	void GenerateGrassToSquare(const Vec2 size, const uint32_t density);
	void GenerateGrassToSphere(const float radius, const uint32_t density);
	void Update(Transform* parent = nullptr);
	void Draw();

public:
	void SetBillboardType(const BillboardType billboardType);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};

