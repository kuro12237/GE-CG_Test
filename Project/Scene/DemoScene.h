#pragma once
#include"Game3dObject.h"
#include"IScene.h"
#include"GameManager.h"
#include"Light/LightingManager.h"
#include"GameObject/FireParticles/FireParticle.h"

enum DemoSceneChangeShadingModel
{
	NormalModel,
    PhongModel,
	PhongModel_NormalMap,
	UE4PRDF,
};

enum DemoModelChange
{
	Monkey,
	Sphere
};

enum DemoSpriteChange
{
	UVCHECKER,
	CLEYERA
};

struct  SelectModels
{
	uint32_t Sphere;
	uint32_t Box_;
	uint32_t Monkey_;
};

class DemoScene :public IScene
{
public:
	DemoScene() {};
	~DemoScene() {};

	void Initialize()override;

	void Update(GameManager* Scene)override;

	void Back2dSpriteDraw()override;

	void Object3dDraw()override;

	void Flont2dSpriteDraw()override;

private:

	void SpriteImGui(WorldTransform& w, const char* name, bool &drawFlag, BlendMode& b, Vector4& color, DemoSpriteChange& tc, Vector2& anker);

	void ImGuIUpdate();

	void SelectShadingMode(unique_ptr<Game3dObject> &g,DemoSceneChangeShadingModel index);

	void GameObjectContorol();

	ViewProjection viewProjection_ = {};
	DemoSceneChangeShadingModel gameObjectShadingModelSelect_ = PhongModel;

	Vector4 modelColor_ = { 1.0f,1.0f,1.0f,1.0f };
	unique_ptr<Game3dObject>gameObject_ = nullptr;
	WorldTransform worldTransform_ = {};
	DemoModelChange NowModelDataSelect_ = Sphere;
	uint32_t modelDataSelect = 0;
	float metalness = 0.0f;
	float roughness_ = 0.0f;
	float specular_ = 0.0f;
	float  shininess_ = 70.0f;

	uint32_t monkeyModelHandle_ = 0;
	uint32_t sphereModelHandle = 0;

	unique_ptr<Game3dObject>groundObject_ = nullptr;
	WorldTransform groundWorldTransform_ = {};
	DemoSceneChangeShadingModel groundObjectShadingModelSelect_ = PhongModel_NormalMap;
	bool isGroundDraw_ = true;

	unique_ptr<Game3dObject>skyDomeObject_ = nullptr;
	WorldTransform skyDomeWorldTransform_{};
	bool isSkyDomeDraw_ = true;

	PointLight_param whiteLight_ = {};
	unique_ptr<Game3dObject>WhiteLightTestGameObject_ = nullptr;
	WorldTransform whiteWorldTransform_ = {};
	bool isWhiteLightTestDrawGameObject_ = false;

	PointLight_param redLight_ = {};
	unique_ptr<Game3dObject>redLightTestGameObject_ = nullptr;
	WorldTransform redWorldTransform_ = {};
	bool isRedLightTestDrawGameObject_ = false;

	unique_ptr<Sprite>sprite_[2] = {};
	WorldTransform spriteWorldTransform_[2] = {};
	bool IsSpriteDraw[2] = {};
	uint32_t NowSelectTex[2] = {};
	DemoSpriteChange SelectSprite_[2] = {};
	BlendMode spriteBlendMode_[2] = { BlendNone,BlendNone };
	Vector4 spriteColor_[2] = { {1,1,1,1},{1,1,1,1} };
	Vector2 ankerPos_[2] = { 0.5f,0.5f };
	uint32_t uvCheckerTexHandle_ = 0;
	uint32_t CLEYERATexHandle_ = 0;

	unique_ptr<FireParticle>fireParticle_ = nullptr;
	bool isFireDraw_ = true;
};