#pragma once
#include"Pch.h"
#include"MatrixTransform.h"
#include"CreateResource.h"
#include"ViewProjection.h"
#include"WinApp.h"
enum Projection
{
	PerspectiveFov = 0,

    OrthographicMatrix = 1
};

struct WorldTransform
{
	~WorldTransform() {};

	Vector3 scale = {1.0f,1.0f,1.0f};
	Vector3 rotation = {0.0f,0.0f,0.0f};
	Vector3 translate = {0.0f,0.0f,0.0f};

	Matrix4x4 matWorld = {};
	const WorldTransform* parent = {};


	ComPtr<ID3D12Resource> buffer_ = nullptr;

	/// <summary>
	/// GPUに送る用
	/// </summary>
	TransformationMatrix* BufferMatrix_ = nullptr;

	void Initialize();
	void SRTSetting(Vector3 s = { 1,1,1 }, Vector3 r = { 0,0,0 }, Vector3 t = {0,0,0});
	void UpdateMatrix();
  
	void CreateBuffer();
	void Map();
	void UnMap();
	void TransfarMatrix();
};

