#include "TitleScene.h"

void TitleScene::Initialize()
{
	TextureManager::UnUsedFilePath();
	cleyeraTexHandle_ = TextureManager::LoadPngTexture("Resources/Default/CLEYERA.png");
	blackTexHandle_ = TextureManager::LoadPngTexture("blackTex.png");

	for (int i = 0; i < 2; i++)
	{
		sprite_[i] = make_unique<Sprite>();
		sprite_[i]->Initialize(new SpriteBoxState);

		worldTransform_[i].Initialize();
	}
	sprite_[0]->SetTexHandle(blackTexHandle_);

	Vector2 size = TextureManager::GetTextureSize(cleyeraTexHandle_);
	size.x *= -0.5f;
	size.y *= -0.5f;
	sprite_[1]->SetPos(size);
	sprite_[1]->SetTexHandle(cleyeraTexHandle_);
	worldTransform_[1].scale = { 0.5f,0.5f,0.5f };
	worldTransform_[1].translate = { 640.0f,360.0f };
	viewProjection_.Initialize();
}

void TitleScene::Update(GameManager* Scene)
{
	Scene;
	color_.w -= 0.01f;
	sprite_[1]->SetColor(color_);

	for (int i = 0; i < 2; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

	if (color_.w<=0.0f)
	{
		Scene->ChangeState(new DemoScene);
		return;

	}

	viewProjection_.UpdateMatrix();
}

void TitleScene::Back2dSpriteDraw()
{
}

void TitleScene::Object3dDraw()
{
}

void TitleScene::Flont2dSpriteDraw()
{
	for (int i = 0; i < 2; i++)
	{
		sprite_[i]->Draw(worldTransform_[i], viewProjection_);
	}
}
