#pragma once
#include "FRTEngine.h"
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>


namespace frt
{

using Microsoft::WRL::ComPtr;

class FRTENGINE_API FrameResource
{
public:
    FrameResource(ID3D12Device* device, UINT rowCount, UINT columnCount, UINT materialCount, float spacingInterval);
    ~FrameResource();

    void InitBundle(ID3D12Device* device, ID3D12PipelineState* pipelineState,
                    UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* indexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* vertexBufferViewDesc,
                    ID3D12DescriptorHeap* cbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* samplerDescriptorHeap, ID3D12RootSignature* rootSignature);

    void PopulateCommandList(ID3D12GraphicsCommandList* commandList,
                             UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* indexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* vertexBufferViewDesc,
                             ID3D12DescriptorHeap* cbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* samplerDescriptorHeap, ID3D12RootSignature* rootSignature);

    void XM_CALLCONV UpdateConstantBuffers(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection);

    void RotateCube(UINT Index, float Roll, float Pitch, float Yaw);

public:
    struct SceneConstantBuffer
    {
        DirectX::XMFLOAT4X4 mvp;   // Model-view-projection
        FLOAT padding[48];
    };

    ComPtr<ID3D12CommandAllocator> _commandAllocator;
    ComPtr<ID3D12CommandAllocator> _bundleAllocator;
    ComPtr<ID3D12GraphicsCommandList> _bundle;
    ComPtr<ID3D12Resource> _cbvUploadHeap;
    SceneConstantBuffer* _constantBuffers;
    UINT64 _fenceValue;
    
    std::vector<DirectX::XMFLOAT4X4> _modelMatrices;
    UINT _rowCount;
    UINT _columnCount;
    UINT _materialCount;
    float _spacingInterval;

private:
    void SetPositions(FLOAT intervalX, FLOAT intervalZ);

};
}

