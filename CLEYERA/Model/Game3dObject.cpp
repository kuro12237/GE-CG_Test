#include "Game3dObject.h"

void Game3dObject::Create()
{
	MaterialBuffer_ = CreateResources::CreateBufferResource(sizeof(Material));
}

void Game3dObject::SetModel(uint32_t index)
{
	if (prevModelIndex_ != index)
	{
	    model_ = ModelManager::GetModel(index);
		texHandle_ = ModelManager::GetObjData(index).material.handle;
		normalTexHandle_ = ModelManager::GetObjData(index).normalTexHandle;
	}
	prevModelIndex_ = index;
}

void Game3dObject::Draw(WorldTransform worldTransform ,ViewProjection view)
{
	if (model_ == nullptr)
	{
		return;
	}


	model_->UseLight(UseLight_);

	Material * materialData;
	MaterialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->shininess = shininess;
	materialData->color = color_;
	materialData->uvTransform = MatrixTransform::AffineMatrix(uvScale_, uvRotate, uvTranslate);
	materialData->specular_    = specular_;
	materialData->roughness_ = roughness_;
	materialData->metalness_  = metalness_;

	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	SPSOProperty PSO = GraphicsPipelineManager::GetInstance()->GetPso().Sprite3d.none;
	if (UseLight_)
	{
		if (CommpandPipeline(PSO))
		{
			assert(0);
		}
	}
	commands.m_pList->SetGraphicsRootSignature(PSO.rootSignature.Get());
	commands.m_pList->SetPipelineState(PSO.GraphicsPipelineState.Get());

	model_->CommandCallPipelineVertex();

	Commands command = DirectXCommon::GetInstance()->GetCommands();

	command.m_pList->SetGraphicsRootConstantBufferView(0, MaterialBuffer_->GetGPUVirtualAddress());
	command.m_pList->SetGraphicsRootConstantBufferView(1, worldTransform.buffer_->GetGPUVirtualAddress());
	command.m_pList->SetGraphicsRootConstantBufferView(2, view.buffer_->GetGPUVirtualAddress());
	DescriptorManager::rootParamerterCommand(4, LightingManager::dsvHandle());
	commands.m_pList->SetGraphicsRootConstantBufferView(5, LightingManager::GetBuffer()->GetGPUVirtualAddress());

	DescriptorManager::rootParamerterCommand(6, texHandle_);

	if (ModelShaderSelect_==TestNormalModel||ModelShaderSelect_==UE4_BRDF)
	{
		DescriptorManager::rootParamerterCommand(7, normalTexHandle_);
	}

	model_->Draw(view);
}

bool Game3dObject::CommpandPipeline(SPSOProperty &PSO)
{
	switch (ModelShaderSelect_)
	{
	case PHONG_MODEL:
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().PhongModel;
		break;

	case UE4_BRDF:
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().PBR_Model;
		break;
	case TestNormalModel:
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().TestNormalModel;
		break;

	default:

		break;
	}


	return false;
}
