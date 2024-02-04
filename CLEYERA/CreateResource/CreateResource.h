#pragma once
#include"Pch.h"
#include"DirectXCommon.h"
#include"CreateResource.h"
struct  VertexData
{
	Vector4 position;
    Vector2 texcoord;
	Vector3 normal;
};
struct LightData
{
	Vector4 color;
	Vector3 direction;
	float intensity;
};

struct  TransformationViewMatrix
{
	Matrix4x4 view;
	Matrix4x4 viewProjection;
	Matrix4x4 orthographic;
	Vector3 position;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 world;
};

struct ParticleData{
	Matrix4x4 WVP;
	Matrix4x4 world;
	Vector4 color;
	Matrix4x4 uvTransform;
};
struct MaterialData
{
	uint32_t handle{};
	string textureFilePath{};
};

struct  SModelData
{
	vector<VertexData> vertices;
	MaterialData material;
	string texFilePath;
	uint32_t texHandle;
	string normalilePath;
	uint32_t normalTexHandle;
};




struct ResourcePeroperty
{
	D3D12_VERTEX_BUFFER_VIEW BufferView;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	ComPtr<ID3D12Resource> Index;
	ComPtr<ID3D12Resource> Vertex;
	ComPtr<ID3D12Resource> Material;
	ComPtr<ID3D12Resource> wvpResource;
	ComPtr<ID3D12Resource> Light;
	ComPtr<ID3D12Resource> instancingResource;
};

struct  Material
{
	Vector4 color;
	Matrix4x4 uvTransform;
	float shininess = 70.0f;
	float specular_ = 1.0f;
	float metalness_ = 1.0f;
	float roughness_ = 1.0f;

};

class CreateResources
{
public:

	/// <summary>
	/// Resource作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInbyte);

	static void CreateBufferResource(size_t sizeInbyte, ComPtr<ID3D12Resource>& Resource);


	/// <summary>
	/// BufferView��쐬
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW VertexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource, int size);
	/// <summary>
	/// index��BufferView
	/// </summary>
	/// <param name="NumVertex"></param>
	/// <returns></returns>
	static D3D12_INDEX_BUFFER_VIEW IndexCreateBufferView(size_t sizeInbyte, ComPtr<ID3D12Resource> Resource);

private:


};
