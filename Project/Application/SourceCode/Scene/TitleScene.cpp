#include "TitleScene.h"
#include "GameScene.h"
#include "SceneChanger.h"
#include "TransitionManager.h"
#include "MovieEvent.h"

void TitleScene::Load()
{
	TextureManager::LoadTexture("Title/GameTitle.png", "GameTitle");
	TextureManager::LoadTexture("Title/PressButton.png", "PressButton");
	TextureManager::LoadTexture("Title/PressButtonBack.png", "PressButtonBack");

	TextureManager::ExcuteComandList();
}

void TitleScene::UnLoad()
{
	TextureManager::UnLoadTexture("GameTitle");
	TextureManager::UnLoadTexture("PressButton");
	TextureManager::UnLoadTexture("PressButtonBack");
}

void TitleScene::CreateInstance()
{
	mTitleUI = std::make_unique<TitleUI>();
}

void TitleScene::Init()
{
	Camera::current.pos = { 0,0,-10 };
	Camera::current.rot = { 0,0,0 };

	mTitleUI->Init();

	SoundManager::Play("TitleBGM", true);
	SoundManager::SetVolume("TitleBGM", 0);
	mBgmVolume = 0;

	MovieEvent::SetisPlayOnce(false);
}

void TitleScene::Update()
{
	SoundManager::SetVolume("TitleBGM", mBgmVolume);
	mBgmVolume += 0.01f;
	mBgmVolume = Min<float>(mBgmVolume, 1.f);

	mTitleUI->Update();

	if (Key::GetKeyDown(DIK_S))
	{
		TransitionManager::GetInstance()->Start(TransitionType::Scene);
	}
	if (Key::GetKeyDown(DIK_E))
	{
		TransitionManager::GetInstance()->End();
	}

	if (Pad::GetAnyButtonDown())
	{
		if (mTitleUI->GetisAfterImage() == false)
		{
			SoundManager::Play("SelectSE");
			mTitleUI->SetisAfterImage(true);

			if (mIsPush == false)
			{
				mIsPush = true;
			}
		}
	}

	if (mTitleUI->GetisEnd() == true)
	{
		auto currentTransition = TransitionManager::GetInstance()->GetCurrentTransition();

		// 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ縺系ull縺九▽繝懊ち繝ｳ謚ｼ縺励※縺ｪ縺・ｴ蜷・
		if (currentTransition == nullptr)
		{
			if (mIsPush == true)
			{
				TransitionManager::GetInstance()->Start(TransitionType::Scene);
				mIsPush = false;
			}
		}
		else
		{
			if (currentTransition->GetType() == TransitionType::Scene &&
				currentTransition->GetStep() == TransitionStep::Progress)
			{
				SceneManager::ChangeScene<GameScene>();
				if (SceneManager::GetisChanged() == true)
				{
					TransitionManager::GetInstance()->End();
				}
			}
		}
	}
}

void TitleScene::RenderTextureSetting()
{
}

void TitleScene::Draw()
{
	mTitleUI->DrawFrontSprite();
}

void TitleScene::DrawDebugGui()
{
}

