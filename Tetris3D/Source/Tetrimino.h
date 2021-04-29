#pragma once
#include "GameObject.h"
#include <vector>
#include <DirectXMath.h>
#include "Render\Mesh.h"

namespace frt
{
class MeshPool;
class App;
class Graphics;
}

class Tetromino : public frt::GameObject
{
public:
    enum class Type
    {
        I, O, T, J, L, S, Z
    };

public:
    Tetromino() = delete;
    Tetromino(Type type);
    //Tetromino(Type type, float radius);
    Tetromino(Type type, float radius, DirectX::XMFLOAT3 worldPosition, frt::MeshPool* meshPool = nullptr);
    //Tetromino(class frt::App* owner, frt::Mesh::SceneObjectConstantBuffer* buffer);

    inline void MoveX(float deltaX) { _worldPosition.x += deltaX; }
    inline void MoveY(float deltaY) { _worldPosition.y += deltaY; }
    inline void MoveZ(float deltaZ) { _worldPosition.z += deltaZ; }

    inline void RotatePitch (float pitchDelta) { _rotation.x += pitchDelta; }
    inline void RotateYaw   (float yawDelta)   { _rotation.z += yawDelta; }
    inline void RotateRoll  (float rollDelta)  { _rotation.y += rollDelta; }
    //void Rotate(float Pitch, float Yaw, float Roll);
    void UpdateConstantBuffers();

    virtual void Update() override;
    virtual void PopulateCommandList() override;

    virtual void InitializeGraphicsResources(frt::Graphics* graphics) override;
    virtual void InitializeConstantBuffers(frt::Graphics* graphics) override;

    static frt::Mesh::SceneObjectConstantBuffer baseBuffer;

private:
    frt::App* _owner;
    
    std::vector<frt::Mesh*> _meshes;
    static const unsigned int MeshesNum;

    Type _type;
    float _radius;

    DirectX::XMFLOAT3 _rotation;
    DirectX::XMFLOAT3 _worldPosition;
};

