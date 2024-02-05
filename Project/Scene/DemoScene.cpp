#include "DemoScene.h"

void DemoScene::Initialize()
{
	ModelManager::ModelLoadNormalMap();
    sphereModelHandle = ModelManager::LoadObjectFile("Sphere");

	ModelManager::ModelLoadNormalMap();
	monkeyModelHandle_ = ModelManager::LoadObjectFile("TestMonkey");

    modelDataSelect = sphereModelHandle;

	worldTransform_.Initialize();
	worldTransform_.translate.y = 1;
	gameObject_ = make_unique<Game3dObject>();
	gameObject_->Create();
	gameObject_->SetModel(NowModelDataSelect_);
	gameObject_->UseLight(true);

	ModelManager::ModelLoadNormalMap();
	uint32_t groundModelHandle = ModelManager::LoadObjectFile("TestGround");
	groundObject_ = make_unique<Game3dObject>();
	groundObject_->Create();
	groundObject_->SetModel(groundModelHandle);
	groundWorldTransform_.Initialize();

	ModelManager::ModelLoadNormalMap();
	uint32_t skyDomeModelHandle = ModelManager::LoadObjectFile("testSkyDome");
	skyDomeObject_ = make_unique<Game3dObject>();
	skyDomeObject_->Create();
	skyDomeObject_->SetModel(skyDomeModelHandle);
	skyDomeWorldTransform_.Initialize();
	skyDomeObject_->UseLight(true);
	skyDomeWorldTransform_.scale = { 16.0f,16.0f,16.0f };

	whiteLight_.position = { 0.0f,0.0f,0.0f };
	redLight_.position = { 2.0f,0.0f,0.0f };
	redLight_.color = { 1,0,0,1 };
	TextureManager::UnUsedFilePath();
	uvCheckerTexHandle_ = TextureManager::LoadPngTexture("Resources/Default/uvChecker.png");
	TextureManager::UnUsedFilePath();
	CLEYERATexHandle_ = TextureManager::LoadPngTexture("Resources/Default/CLEYERA.png");
	Vector2 texSize = TextureManager::GetTextureSize(uvCheckerTexHandle_);
	texSize.x *= -0.5;
	texSize.y *= -0.5f;

	for (int i = 0; i < 2; i++)
	{
		NowSelectTex[i] = uvCheckerTexHandle_;
		sprite_[i] = make_unique<Sprite>();
		sprite_[i]->Initialize(new SpriteBoxState,texSize);
		sprite_[i]->SetTexHandle(NowSelectTex[i]);
		spriteWorldTransform_[i].Initialize();
	}

	fireParticle_ = make_unique<FireParticle>();
	fireParticle_->Initialize({0.0f,2.0f,8.0f});

	hitSoundHandle_ = AudioManager::SoundLoadWave("Resources/sounds/shot.wav");

	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,2.0f,-16.0f };
}

void DemoScene::Update(GameManager* Scene)
{
	Scene;

#ifdef USE_IMGUI

	ImGuIUpdate();

#endif // USE_IMGUI

	GameObjectContorol();

	SelectShadingMode(gameObject_, gameObjectShadingModelSelect_);
	SelectShadingMode(groundObject_,groundObjectShadingModelSelect_);
	LightingManager::AddList(whiteLight_);
	LightingManager::AddList(redLight_);
	gameObject_->SetModel(modelDataSelect);
	gameObject_->SetColor(modelColor_);
	gameObject_->SetShininess(shininess_);
	gameObject_->SetMetalness(metalness);
	gameObject_->SetRoughness(roughness_);
	gameObject_->SetSpecular(specular_);

	for (int i = 0; i < 2; i++)
	{
		switch (SelectSprite_[i])
		{
		case UVCHECKER:
			NowSelectTex[i] = uvCheckerTexHandle_;
			break;
		case CLEYERA:
			NowSelectTex[i] = CLEYERATexHandle_;

		default:
			break;
		}

		Vector2 size = TextureManager::GetTextureSize(NowSelectTex[i]);
		size.x *= ankerPos_[i].x;
		size.y *= ankerPos_[i].y;
		sprite_[i]->SetSrc(spriteSrc[i].lt, spriteSrc[i].lb, spriteSrc[i].rt, spriteSrc[i].rb);
		sprite_[i]->SetPos(size);
		sprite_[i]->SetColor(spriteColor_[i]);
		sprite_[i]->SetTexHandle(NowSelectTex[i]);
		sprite_[i]->SetBlendMode(spriteBlendMode_[i]);
		spriteWorldTransform_[i].UpdateMatrix();
	}

	fireParticle_->Update({0.0f,2.0f,8.0f});
	worldTransform_.UpdateMatrix();
	groundWorldTransform_.UpdateMatrix();
	skyDomeWorldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
}

void DemoScene::Back2dSpriteDraw()
{
}

void DemoScene::Object3dDraw()
{
	
	gameObject_->Draw(worldTransform_, viewProjection_);
	

	if (isSkyDomeDraw_)
	{
		skyDomeObject_->Draw(skyDomeWorldTransform_, viewProjection_);
	}

	if (isGroundDraw_)
	{
		groundObject_->Draw(groundWorldTransform_, viewProjection_);
	}
}

void DemoScene::Flont2dSpriteDraw()
{
	if (isFireDraw_)
	{
		fireParticle_->Draw(viewProjection_);
	}
	for (int i = 0; i < 2; i++)
	{
		if (IsSpriteDraw[i])
		{
			sprite_[i]->Draw(spriteWorldTransform_[i], viewProjection_);
		}
	}
}

void DemoScene::SpriteImGui( WorldTransform& w,const char*name,bool &drawFlag,BlendMode &b,Vector4 &color,DemoSpriteChange &tc,Vector2 &anker,DemoSpriteSrc &src)
{
	auto SetBlend = [](BlendMode b)
	{
		switch (b)
		{
		case BlendNone:
			return "BlendNone";
		case BlendAdd:
			return "BlendAdd";
		case BlendSubtruct:
			return "BlendSubtruct";
		case BlendMultiply:
			return "BlendMultiply";
		case BlendScreen:
			return "BlendScreen";
		default:
			return "Unknown";
		}
	};

	auto TexChange = [](DemoSpriteChange s)
	{
		switch (s)
		{
		case UVCHECKER:
			return "uvChecker";
		case CLEYERA:
			return "CLEYERA";
		default:
			return "unKnown";
		}
	};

	if (ImGui::TreeNode(name))
	{
		ImGui::DragFloat2("translate", &w.translate.x,-10.0f,10.0f);
		ImGui::DragFloat3("rotate", &w.rotation.x, -0.1f, 0.1f);
		ImGui::DragFloat2("scale", &w.scale.x, -0.1f, 0.1f);
		ImGui::Checkbox("isDraw", &drawFlag);
		ImGui::ColorPicker4("Color", &color.x);
		ImGui::DragFloat2("anker", &anker.x, -0.1f, 0.1f);
		ImGui::DragFloat2("src::Lt",&src.lt.x,- 0.1f, 0.1f);
		ImGui::DragFloat2("src::Lb", &src.lb.x, -0.1f, 0.1f);
		ImGui::DragFloat2("src::Rt", &src.rt.x, -0.1f, 0.1f);
		ImGui::DragFloat2("src::Rb", &src.rb.x, -0.1f, 0.1f);

		if (ImGui::BeginCombo("BlendChange", SetBlend(b)))
		{
			for (int i = 0; i < 5; i++) {
				bool isSelected = (b == static_cast<BlendMode>(i));
				if (ImGui::Selectable(SetBlend(static_cast<BlendMode>(i)), isSelected))
					b = static_cast<BlendMode>(i);
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("TexChange", TexChange(tc)))
		{
			for (int i = 0; i < 2; i++) {
				bool isSelected = (tc == static_cast<DemoSpriteChange>(i));
				if (ImGui::Selectable(TexChange(static_cast<DemoSpriteChange>(i)), isSelected))
					tc = static_cast<DemoSpriteChange>(i);
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}


}

void DemoScene::ImGuIUpdate()
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.02f, 0.02f, 1.0f));

	ImGui::Begin("Demo");
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::DragFloat3("Rotate", &viewProjection_.rotation_.x, -0.01f, 0.01f);
		ImGui::DragFloat3("Translate", &viewProjection_.translation_.x, -0.1f, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Control"))
	{
		ImGui::Text("WS :ZMove");
		ImGui::Text("AD :XMove");
		ImGui::Text("RF  :YMove");
		ImGui::Text("LeftRight    :YRotate");
		ImGui::Separator();
		ImGui::Text("LStick : Move");
		ImGui::Text("A : acceleration");
		ImGui::Text("RStick : Rotate");
		ImGui::TreePop();
	}
	auto  SelectModelEnum = [](DemoSceneChangeShadingModel s)
	{
		switch (s)
		{
		case NormalModel:
			return "NormalModel";
		case PhongModel:
			return "PhongModel";
		case PhongModel_NormalMap:
			return "PhongModel_NormalMap";
		case UE4PRDF:
			return "UE4PRDF";
		default:
			return "Unknoen";
		}
	};

	auto SelectModel = [](DemoModelChange s)
	{
		switch (s)
		{
		case Monkey:
			return "MOnkey";
		case Sphere:
			return "Sphere";
		default:
			return "Unknown";
		}
	};

	if (ImGui::TreeNode("Models"))
	{
		if (ImGui::BeginCombo("ModelChange", SelectModel(NowModelDataSelect_)))
		{
			for (int i = 0; i < 2; i++) {
				bool isSelected = (NowModelDataSelect_== static_cast<DemoModelChange>(i));
				if (ImGui::Selectable(SelectModel(static_cast<DemoModelChange>(i)), isSelected))
					NowModelDataSelect_ = static_cast<DemoModelChange>(i);
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		switch (NowModelDataSelect_)
		{
		case Monkey:
			modelDataSelect = monkeyModelHandle_;
			break;
		case Sphere:
			modelDataSelect = sphereModelHandle;
			break;
		default:
			break;
		}

		ImGui::DragFloat3("scale", &worldTransform_.scale.x, -0.1f, 0.1f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation.x, -0.1f, 0.1f);
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, -0.1f, 0.1f);
		ImGui::ColorPicker4("color", &modelColor_.x);
		ImGui::DragFloat(" shininess", &shininess_);
		ImGui::DragFloat("metalness", &metalness,-0.1f,0.1f);
		ImGui::DragFloat("rougness", &roughness_, -0.1f, 0.1f);
		ImGui::DragFloat("specular", &specular_, -0.1f, 0.1f);
		if (ImGui::BeginCombo("ShadingChange", SelectModelEnum(gameObjectShadingModelSelect_)))
		{
			for (int i = 0; i < 4; i++) {
				bool isSelected = (gameObjectShadingModelSelect_ == static_cast<DemoSceneChangeShadingModel>(i));
				if (ImGui::Selectable(SelectModelEnum(static_cast<DemoSceneChangeShadingModel>(i)), isSelected))
					gameObjectShadingModelSelect_ = static_cast<DemoSceneChangeShadingModel>(i);
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("Ground"))
	{
		ImGui::DragFloat3("scale", &groundWorldTransform_.scale.x, -0.1f, 0.1f);
		ImGui::DragFloat3("rotate", &groundWorldTransform_.rotation.x, -0.01f, 0.01f);
		ImGui::DragFloat3("translate", &groundWorldTransform_.translate.x, -0.1f, 0.1f);
		ImGui::Checkbox("DrawFlag", &isGroundDraw_);
		if (ImGui::BeginCombo("ShadingChange", SelectModelEnum(groundObjectShadingModelSelect_)))
		{
			for (int i = 0; i < 4; i++) {
				bool isSelected = (groundObjectShadingModelSelect_ == static_cast<DemoSceneChangeShadingModel>(i));
				if (ImGui::Selectable(SelectModelEnum(static_cast<DemoSceneChangeShadingModel>(i)), isSelected))
					groundObjectShadingModelSelect_ = static_cast<DemoSceneChangeShadingModel>(i);
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SkyDome"))
	{
		ImGui::DragFloat3("scale", &skyDomeWorldTransform_.scale.x, -0.1f, 0.1f);
		ImGui::DragFloat3("rotate", &skyDomeWorldTransform_.rotation.x, -0.01f, 0.01f);
		ImGui::DragFloat3("translate", &skyDomeWorldTransform_.translate.x, -0.1f, 0.1f);
		ImGui::Checkbox("DrawFlag", &isSkyDomeDraw_);
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("whiteLight"))
	{
		ImGui::DragFloat3("translate", &whiteLight_.position.x, -0.1f, 0.1f);
		ImGui::DragFloat("decay", &whiteLight_.decay, -0.1f, 0.1f);
		ImGui::DragFloat("radious", &whiteLight_.radious, -0.1f, 0.1f);
		ImGui::DragFloat("intencity", &whiteLight_.intencity,-0.1f,0.1f);
		ImGui::ColorPicker4("color", &whiteLight_.color.x);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("redLight"))
	{
		ImGui::DragFloat3("translate", &redLight_.position.x, -0.1f, 0.1f);
		ImGui::DragFloat("decay", &redLight_.decay, -0.1f, 0.1f);
		ImGui::DragFloat("radious", &redLight_.radious, -0.1f, 0.1f);
		ImGui::DragFloat("intencity", &redLight_.intencity, -0.1f, 0.1f);
		ImGui::ColorPicker4("color", &redLight_.color.x);

		ImGui::TreePop();
	}
	ImGui::Separator();


	SpriteImGui(spriteWorldTransform_[0], "spriteA",IsSpriteDraw[0],spriteBlendMode_[0],spriteColor_[0],SelectSprite_[0],ankerPos_[0],spriteSrc[0]);
	SpriteImGui(spriteWorldTransform_[1], "spriteB",IsSpriteDraw[1], spriteBlendMode_[1],spriteColor_[1],SelectSprite_[1],ankerPos_[1],spriteSrc[1]);
	ImGui::Separator();
	if (ImGui::TreeNode("Particle"))
	{
		ImGui::Checkbox("DrawFlag", &isFireDraw_);
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::Button("SoundPlay"))
	{
		AudioManager::AudioPlayWave(hitSoundHandle_);
	}


	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void DemoScene::SelectShadingMode(unique_ptr<Game3dObject>& g, DemoSceneChangeShadingModel index)
{
	switch (index)
	{
	case NormalModel:
		g->UseLight(false);
		break;
	case PhongModel:
		g->UseLight(true);
		g->SetlectModelPipeline(PHONG_MODEL);
		break;
	case PhongModel_NormalMap:
		g->UseLight(true);
		g->SetlectModelPipeline(TestNormalModel);
		break;
	case UE4PRDF:
		g->UseLight(true);
		g->SetlectModelPipeline(UE4_BRDF);
		break;

	default:
		break;
	}
}

void DemoScene::GameObjectContorol()
{
	const float kSpeed = 0.05f;
	if (Input::PushKey(DIK_W))
	{
		worldTransform_.translate.z+=kSpeed;
	}
	if (Input::PushKey(DIK_S))
	{
		worldTransform_.translate.z -= kSpeed;
	}
	//x
	if (Input::PushKey(DIK_A))
	{
		worldTransform_.translate.x -= kSpeed;
	}
	if (Input::PushKey(DIK_D))
	{
		worldTransform_.translate.x += kSpeed;
	}
	//y
	if (Input::PushKey(DIK_F))
	{
		worldTransform_.translate.y -= kSpeed;
	}
	if (Input::PushKey(DIK_R))
	{
		worldTransform_.translate.y += kSpeed;
	}
	//Rotatey
	if (Input::PushKey(DIK_LEFT))
	{
		worldTransform_.rotation.y -= kSpeed;
	}
	if (Input::PushKey(DIK_RIGHT))
	{
		worldTransform_.rotation.y += kSpeed;
	}
	
	float joySpeed = 0.1f;

	if (Input::PushBotton(XINPUT_GAMEPAD_A))
	{
		joySpeed = 0.3f;
	}

	Vector2 joyL = Input::GetJoyLStickPos();
	Vector2 joyR = Input::GetJoyRStickPos();

	worldTransform_.translate.x += joyL.x*joySpeed;
	worldTransform_.translate.z += joyL.y*joySpeed;

	worldTransform_.rotation.y += joyR.x * joySpeed;
	worldTransform_.rotation.x += joyR.y * joySpeed;

}
