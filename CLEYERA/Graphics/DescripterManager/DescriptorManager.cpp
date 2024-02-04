#include"DescriptorManager.h"

DescriptorManager *DescriptorManager::GetInstance()
{
	static DescriptorManager instance;
	return &instance;
}

void DescriptorManager::Initialize()
{
	ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();
	DescriptorManager::GetInstance()->descripterSize_.SRV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DescriptorManager::GetInstance()->descripterSize_.RTV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DescriptorManager::GetInstance()->descripterSize_.DSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	DescriptorManager::GetInstance()->index = 0;
}

void DescriptorManager::BeginFlame()
{
	if (DescriptorManager::GetInstance()->index >= DESCRIPTER_MAX)
	{
		//DescripterIndex�̐�������𒴂���
		//#define DescripterMax �̐��𑝂₷���e�N�X�`���EParticle�̗��p�������
		LogManager::Log("DescripterManager index Overflow");
		assert(0);
	}

}

void DescriptorManager::Clear()
{
	DescriptorManager::GetInstance()->index = 0;
}

uint32_t DescriptorManager::CreateInstancingSRV(uint32_t NumInstansing, ComPtr<ID3D12Resource>& resource, UINT size)
{
	DescriptorManager::GetInstance()->index++;
	D3D12_SHADER_RESOURCE_VIEW_DESC instansingSrvDesc;
	instansingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instansingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instansingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instansingSrvDesc.Buffer.FirstElement = 0;
	instansingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;;
	instansingSrvDesc.Buffer.NumElements = NumInstansing;
	instansingSrvDesc.Buffer.StructureByteStride = size;

	DescriptorManager::GetInstance()->SrvHandleCPU[DescriptorManager::GetInstance()->index] = 
		GetCPUDescriptorHandle(
			DirectXCommon::GetInstance()->GetSrvHeap(),
			DescriptorManager::GetInstance()->descripterSize_.SRV, 
			DescriptorManager::GetInstance()->index
		);

	DescriptorManager::GetInstance()->SrvHandleGPU[DescriptorManager::GetInstance()->index] =
		GetGPUDescriptorHandle(
		    DirectXCommon::GetInstance()->GetSrvHeap(), 
			DescriptorManager::GetInstance()->descripterSize_.SRV,
			DescriptorManager::GetInstance()->index
 	    );

	DescriptorManager::GetInstance()->SrvHandleCPU[DescriptorManager::GetInstance()->index].ptr +=
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	DescriptorManager::GetInstance()->SrvHandleGPU[DescriptorManager::GetInstance()->index].ptr +=
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
		resource.Get(),
		&instansingSrvDesc,
		DescriptorManager::GetInstance()->SrvHandleCPU[DescriptorManager::GetInstance()->index]
	);

	return DescriptorManager::GetInstance()->index;
}

void DescriptorManager::rootParamerterCommand(UINT rootPatramerterIndex, uint32_t texhandle)
{
	Commands command = DirectXCommon::GetInstance()->GetCommands();

	command.m_pList->SetGraphicsRootDescriptorTable(
		rootPatramerterIndex, 
		DescriptorManager::GetInstance()->SrvHandleGPU[texhandle]
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descripterHeap, uint32_t desiripterSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descripterHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (desiripterSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorManager::GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descripterHeap, uint32_t desiripterSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descripterHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (desiripterSize * index);
	return handleGPU;
}

void DescriptorManager::CGHandlePtr()
{
	DescriptorManager::GetInstance()->SrvHandleCPU[DescriptorManager::GetInstance()->index].ptr 
		+= DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	DescriptorManager::GetInstance()->SrvHandleGPU[DescriptorManager::GetInstance()->index].ptr
		+= DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DescriptorManager::CreateShaderResourceView(ID3D12Resource* resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index)
{
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
		resource,
		&srvDesc,
		DescriptorManager::GetInstance()->SrvHandleCPU[index]);
}
