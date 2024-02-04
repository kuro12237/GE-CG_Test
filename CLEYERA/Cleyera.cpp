#include "Cleyera.h"

Cleyera* Cleyera::GetInstance()
{
	static Cleyera instance;
	return &instance;
}

void Cleyera::Initialize()
{
	Cleyera::GetInstance();

	WinApp::Initialize();
	DirectXCommon::initialize();
	DescriptorManager::Initialize();
	ShaderManager::Initialize();
	GraphicsPipelineManager::Initialize();
	
	TextureManager::Initialize();
	ImGuiManager::Initialize();
	Input::Initialize();
	AudioManager::Initialize();
	LightingManager::Initialize();
}

void Cleyera::Finalize()
{
	ModelManager::Finalize();
	DebugTools::ClearCommand();
	AudioManager::Finalize();
	TextureManager::Finalize();
	DescriptorManager::Clear();
	ImGuiManager::Finalize();
	DirectXCommon::Finalize();
	WinApp::Finalize();
}

void Cleyera::BeginFlame()
{
	DirectXCommon::BeginFlame();
	ImGuiManager::BeginFlame();
	Input::BeginFlame();
	DescriptorManager::BeginFlame();
}

void Cleyera::EndFlame()
{
	ImGuiManager::EndFlame();
	DirectXCommon::EndFlame();
}
