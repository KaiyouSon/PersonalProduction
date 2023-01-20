#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include "Util.h"
#include <vector>

class Sprite
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	std::vector<VertexPosUv> vertices;
	ConstantBuffer<ConstantBufferDataTransform2D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	Transform transform;

public:
	Texture texture;
	Vec2 pos;
	Vec2 scale;
	Vec2 uvPos;
	Vec2 size;
	float rot;
	Color color;
	Vec2 anchorPoint;

private:
	void TransferTexturePos();

public:
	Sprite();
	~Sprite();
	void Update();
	void Draw();

	inline Vec2 GetTextureSize() { return texture.size; }
	void SetTextureRect(const Vec2& leftTopPos, const Vec2 rightDownPos);

	static void SetBlendMode(const BlendMode& blendMode);
};

