#include "TitleScene.h"
#include "GameScene.h"
#include "SceneChanger.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,0,-10 };
	Camera::current.rot = { 0,0,0 };

	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Init();

	SoundManager::Play("TitleBGM", true);
	SoundManager::SetVolume("TitleBGM", 0);
	bgmVolume_ = 0;
}

void TitleScene::Update()
{
	SoundManager::SetVolume("TitleBGM", bgmVolume_);
	bgmVolume_ += 0.01f;
	bgmVolume_ = Min<float>(bgmVolume_, 1.f);

	titleUI_->Update();

	if (Pad::GetAnyButtonDown())
	{
		if (titleUI_->GetisAfterImage() == false)
		{
			SoundManager::Play("SelectSE");
			titleUI_->SetisAfterImage(true);
		}
	}

	if (titleUI_->GetisEnd() == true)
	{
		if (SceneChanger::GetInstance()->GetisSceneChanging() == false)
		{
			SceneChanger::GetInstance()->StartSceneChange();
			SceneChanger::GetInstance()->SetisEaseTitleBGM(true);
		}

		if (SceneChanger::GetInstance()->GetisChange() == true)
		{
			SceneManager::ChangeScene<GameScene>();
			SceneChanger::GetInstance()->SetisChange(false);

		}
	}
}

void TitleScene::RenderTextureSetting()
{
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::DrawModel()
{
}

void TitleScene::DrawFrontSprite()
{
	titleUI_->DrawFrontSprite();
}

void TitleScene::DrawDebugGui()
{
}
