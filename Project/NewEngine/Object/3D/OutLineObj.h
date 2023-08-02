#pragma once
#include "NewEngine.h"

// 最近使ってないためリファクタリングは後回し
class OutLineObj
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> mConstantBufferTransform;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> mConstantBufferColor;
	Transform mTransform;
	Transform* mParent;

public:
	Object3D* obj;
	Color color;

public:
	OutLineObj();

	void Update(Transform* parent = nullptr);
	void Draw();
};

