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
    friend class TetrisBoard;
    
public:
    enum class Type
    {
        I, O, T, J, L, S, Z
    };

    struct Bounds
    {
        DirectX::XMFLOAT2 left;
        DirectX::XMFLOAT2 top;
        DirectX::XMFLOAT2 right;
        DirectX::XMFLOAT2 bottom;
    };

    struct BodyState
    {
        Bounds bounds;
        std::vector<DirectX::XMFLOAT3> meshPositions;
    };

public:
    Tetromino() = delete;
    Tetromino(Type type);
    //Tetromino(Type type, float radius);
    Tetromino(Type type, float radius, DirectX::XMFLOAT3 worldPosition, DirectX::XMFLOAT4 color, frt::MeshPool* meshPool = nullptr);
    //Tetromino(class frt::App* owner, frt::Mesh::SceneObjectConstantBuffer* buffer);

    inline void MoveX(float deltaX) { _worldPosition.x += deltaX; }
    inline void MoveY(float deltaY) { _worldPosition.y += deltaY; }
    inline void MoveZ(float deltaZ) { _worldPosition.z += deltaZ; }

    inline const DirectX::XMFLOAT3& GetWorldPosition() const { return _worldPosition; }

    void RotatePitch(float pitchDelta);
    void RotateYaw(float yawDelta);
    void RotateRoll(float rollDelta);

    inline float GetTopBound()    { return _topBound;    }
    inline float GetBottomBound() { return _bottomBound; }
    inline float GetLeftBound()   { return _leftBound;   }
    inline float GetRightBound()  { return _rightBound;  }

    BodyState GetBoundsAfterRotation(float deltaRadians);

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

    float _topBound;
    float _bottomBound;
    float _leftBound;
    float _rightBound;
};

