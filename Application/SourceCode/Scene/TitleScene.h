#pragma once
#include "NewEngine.h"

class TitleScene : public IScene
{
private:
	Object3D obj;
	Object3D fbx;
	Object3D skyDome;
	OutLineObj olobj;
	Sprite sprite;
	SphereCollider sphereCollider;
	//RenderTexture renderTexture;

public:
	void Init() override;
	void Update() override;
	void DrawRenderTexture() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

