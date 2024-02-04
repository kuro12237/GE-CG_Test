#include "WorldTransform.h"

void WorldTransform::CreateBuffer()
{
	buffer_ = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
}

void WorldTransform::Map()
{
	buffer_->Map(0, nullptr, reinterpret_cast<void**>(&BufferMatrix_));
}

void WorldTransform::UnMap()
{
	buffer_->Unmap(0,nullptr);
}

void WorldTransform::Initialize()
{
	CreateBuffer();
	matWorld = MatrixTransform::Identity();
	TransfarMatrix();
}

void WorldTransform::SRTSetting(Vector3 s, Vector3 r, Vector3 t)
{
	scale = s;
	rotation = r;
	translate = t;
	UpdateMatrix();
}

void WorldTransform::UpdateMatrix()
{
	matWorld = MatrixTransform::AffineMatrix(scale, rotation, translate);

	if (parent) {
		matWorld = MatrixTransform::Multiply(matWorld, parent->matWorld);
	}
	
	TransfarMatrix();
}



void WorldTransform::TransfarMatrix()
{
	Map();
	BufferMatrix_->WVP = matWorld;
	BufferMatrix_->world = matWorld;//MatrixTransform::Identity();
	UnMap();
}
