#include "VertexShader.h"

#include "Exception.h"
#include "Utils/ShaderReader.h"


namespace frt
{
    VertexShader::VertexShader(Graphics* owner, LPCWSTR filename, unsigned inputSlot): GraphicsResource(owner)
    {
        // UINT8* pVertexShaderData;
        // UINT vertexShaderDataLength;
        // THROW_IF_FAILED(
        //     ShaderReader::ReadDataFromFile(L"D:\\FRT\\FRTEngine\\Binaries\\x64\\Debug\\Tetris3D\\VertexShader.cso",
        //         &pVertexShaderData, &vertexShaderDataLength));
        //
        // D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        // {
        //     {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, inputSlot, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        //     {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, inputSlot, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        //     {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, inputSlot, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        // };
    }
}
