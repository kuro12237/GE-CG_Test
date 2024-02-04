#pragma once
#include"Game3dObject.h"
#include"IScene.h"
#include"GameManager.h"
#include"Light/LightingManager.h"
#include"GameObject/FireParticles/FireParticle.h"
#include"DemoScene.h"

class TitleScene :public IScene
{
public:
	TitleScene() {};
	~TitleScene() {};

	void Initialize()override;

	void Update(GameManager* Scene)override;

	void Back2dSpriteDraw()override;

	void Object3dDraw()override;

	void Flont2dSpriteDraw()override;

private:

	unique_ptr<Sprite>sprite_[2] = {};
	WorldTransform worldTransform_[2] = {};
	ViewProjection viewProjection_ = {};
	Vector4 color_ = { 1,1,1,1 };
	uint32_t blackTexHandle_ = 0;
	uint32_t cleyeraTexHandle_ = 0;
};
