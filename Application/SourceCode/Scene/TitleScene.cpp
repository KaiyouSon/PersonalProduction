#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj.model = *ModelManager::GetModel("AttackEnemy");
	obj.rot.y = Radian(180);

	skyDome.model = *ModelManager::GetModel("SkyDome");

	//sprite.texture = *TextureManager::GetTexture("White");
	//sprite.texture = *TextureManager::GetTexture("pic");
	sprite.texture = *TextureManager::GetRenderTexture("PostEffect");
	sprite.pos = GetWindowHalfSize();

	SoundManager::Play("BGM");
}

static bool isDraw = false;

void TitleScene::Update()
{
	//if (Key::GetKeyTrigger(DIK_SPACE))
	//{
	//	if (isDraw == false)
	//	{
	//		isDraw = true;
	//	}
	//	else
	//	{
	//		isDraw = false;
	//	}
	//}

	////obj.rot.y += Radian(1);
	//const float speed = 10;

	//sprite.pos.x += (Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT)) * 5.f;
	//sprite.pos.y += (Key::GetKey(DIK_DOWN) - Key::GetKey(DIK_UP)) * 5.f;
	//sprite.color.a += Key::GetKey(DIK_Q) - Key::GetKey(DIK_E);

	//obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A));
	//obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S));

	//if (Pad::GetButton(PadCodo::ButtonA))
	//{
	//	static int a = 0;
	//	a++;
	//}

	//obj.pos.x = +Pad::GetStick(PadCodo::LeftStick, 300).x / 1000;
	//obj.pos.y = -Pad::GetStick(PadCodo::LeftStick, 300).y / 1000;

	//sprite.pos.x += Pad::GetStick(PadCodo::LeftStick, 300, 1).x / 1000;
	//sprite.pos.y += Pad::GetStick(PadCodo::LeftStick, 300, 1).y / 1000;

	sprite.Update();

	sprite.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
	skyDome.Update();
	obj.Update();
}

void TitleScene::DrawRenderTexture()
{
	TextureManager::GetRenderTexture("PostEffect")->PreDrawScene();
	if (isDraw == true)
	{
		skyDome.Draw();
	}
	obj.Draw();
	TextureManager::GetRenderTexture("PostEffect")->PostDrawScene();
}

void TitleScene::DrawBackSprite()
{
	sprite.Draw();
}

void TitleScene::DrawModel()
{
	//obj.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
