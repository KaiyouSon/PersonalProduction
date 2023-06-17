#pragma once
#include "NewEngine.h"

class Vignette
{
private:
	std::unique_ptr<PostEffect> postEffect_;
	RenderTexture* tex_;

public:
	Vignette();
	void Update();
	void DrawPostEffect();
	void PrevSceneDraw();
	void PostSceneDraw();

};

