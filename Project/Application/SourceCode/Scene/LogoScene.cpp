#include "LogoScene.h"
#include "TitleScene.h"

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
}

void LogoScene::Load()
{
	TextureManager::LoadTexture("LogoScene/NewEngineTitle.png", "NewEngineTitle");
	TextureManager::CreateTexture(Color::black, "Black");
}

void LogoScene::UnLoad()
{
	TextureManager::UnLoadTexture("NewEngineTitle");
	TextureManager::UnLoadTexture("Black");
}

void LogoScene::CreateInstance()
{
	mLogoSprite = std::move(std::make_unique<Sprite>());
	mBackSprite = std::move(std::make_unique<Sprite>());
}

void LogoScene::Init()
{
	mLogoSprite->SetTexture(TextureManager::GetTexture("NewEngineTitle"));
	mBackSprite->SetTexture(TextureManager::GetTexture("Black"));

	mLogoSprite->pos = GetWindowHalfSize();
	mBackSprite->pos = GetWindowHalfSize();

	mLogoSprite->color.a = 0;
	mBackSprite->SetSize(GetWindowSize());

	mStayTimer.SetLimitTimer(30);

	mAlphaEase.SetEaseTimer(60);
	mIsReverce = false;

	mIsEnd = false;
}

void LogoScene::Update()
{
	mStayTimer.Update(true);
	if (mStayTimer.GetisTimeOut() == true && mIsEnd == false)
	{
		mAlphaEase.Update();

		if (mIsReverce == false)
		{
			mLogoSprite->color.a = mAlphaEase.Lerp(0, 255);

			if (mAlphaEase.GetisEnd() == true)
			{
				mAlphaEase.Reset();
				mIsReverce = true;
			}
		}
		if (mIsReverce == true)
		{
			mLogoSprite->color.a = mAlphaEase.Lerp(255, 0);

			if (mAlphaEase.GetisEnd() == true)
			{
				mAlphaEase.Reset();
				mStayTimer.Reset();
				mIsReverce = false;
				mIsEnd = true;
			}
		}
	}

	mLogoSprite->Update();
	mBackSprite->Update();

	if (mStayTimer.GetisTimeOut() == true && mIsEnd == true)
	{
		SceneManager::ChangeScene<TitleScene>();

		if (SceneManager::GetisChanged() == true)
		{
			SceneManager::SetChangeStepToCreateInstance();
		}
	}
}

void LogoScene::RenderTextureSetting()
{
}

void LogoScene::Draw()
{
	mBackSprite->Draw();
	mLogoSprite->Draw();
}

void LogoScene::DrawDebugGui()
{
}

