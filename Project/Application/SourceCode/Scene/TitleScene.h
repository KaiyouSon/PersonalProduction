#pragma once
#include "NewEngine.h"
#include "TitleUI.h"
#include "Emitter.h"

class TitleScene : public IScene
{
private:
	std::unique_ptr<TitleUI> mTitleUI;
	float mBgmVolume;
	bool mIsPush;

public:
	void Load() override;
	void UnLoad() override;
	void CreateInstance() override;
	void Init() override;
	void Update() override;
	void RenderTextureSetting() override;
	void Draw() override;
	void DrawDebugGui() override;
};

