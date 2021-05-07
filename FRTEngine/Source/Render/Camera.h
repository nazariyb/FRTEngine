#pragma once
#include "FRTEngine.h"
#include <DirectXMath.h>
#include "WindowsMinimal.h"

namespace frt
{
class FRTENGINE_API Camera
{
public:
    Camera();

    void Init(DirectX::XMFLOAT3 position);
    void Update(float elapsedSeconds);
    DirectX::XMMATRIX GetViewMatrix();
    DirectX::XMMATRIX GetProjectionMatrix(float fov, float aspectRatio, float nearPlane = 1.0f, float farPlane = 1000.0f);
    void SetMoveSpeed(float unitsPerSecond);
    void SetTurnSpeed(float radiansPerSecond);

    void OnKeyDown(WPARAM key);
    void OnKeyUp(WPARAM key);

private:
    void Reset();

    struct KeysPressed
    {
        bool w;
        bool a;
        bool s;
        bool d;

        bool left;
        bool right;
        bool up;
        bool down;
    };

    DirectX::XMFLOAT3 _initialPosition;
public:
    DirectX::XMFLOAT3 _position;
    DirectX::XMFLOAT3 _lookDirection;
private:
    float _yaw;                  // Relative to the +z axis.
    float _pitch;                // Relative to the xz plane.
    DirectX::XMFLOAT3 _upDirection;
    float _moveSpeed;            // Speed at which the camera moves, in units per second.
    float _turnSpeed;            // Speed at which the camera turns, in radians per second.

    KeysPressed _keysPressed;
};
}

