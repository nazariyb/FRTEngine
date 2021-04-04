#include "Camera.h"
#include <algorithm>

namespace frt
{

using namespace DirectX;

Camera::Camera()
    : _initialPosition(0, 0, 0)
    , _position(_initialPosition)
    , _yaw(XM_PI)
    , _pitch(0.f)
    , _lookDirection(0, 0, -1)
    , _upDirection(0, 1, 0)
    , _moveSpeed(20.f)
    , _turnSpeed(XM_PIDIV4)
    , _keysPressed{}
{}

void Camera::Init(DirectX::XMFLOAT3 position)
{
    _initialPosition = position;
    Reset();
}

void Camera::Update(float elapsedSeconds)
{
    // Calculate the move vector in camera space.
    XMFLOAT3 move(0, 0, 0);

    if (_keysPressed.a)
        move.x -= 1.0f;
    if (_keysPressed.d)
        move.x += 1.0f;
    if (_keysPressed.w)
        move.z -= 1.0f;
    if (_keysPressed.s)
        move.z += 1.0f;

    if (fabs(move.x) > 0.1f && fabs(move.z) > 0.1f)
    {
        XMVECTOR vector = XMVector3Normalize(XMLoadFloat3(&move));
        move.x = XMVectorGetX(vector);
        move.z = XMVectorGetZ(vector);
    }

    float moveInterval = _moveSpeed * elapsedSeconds;
    float rotateInterval = _turnSpeed * elapsedSeconds;

    if (_keysPressed.left)
        _yaw += rotateInterval;
    if (_keysPressed.right)
        _yaw -= rotateInterval;
    if (_keysPressed.up)
        _pitch += rotateInterval;
    if (_keysPressed.down)
        _pitch -= rotateInterval;

    // Prevent looking too far up or down.
    _pitch = std::min(_pitch, XM_PIDIV4);
    _pitch = std::max(-XM_PIDIV4, _pitch);

    // Move the camera in model space.
    float x = move.x * -cosf(_yaw) - move.z * sinf(_yaw);
    float z = move.x * sinf(_yaw) - move.z * cosf(_yaw);
    _position.x += x * moveInterval;
    _position.z += z * moveInterval;

    // Determine the look direction.
    float r = cosf(_pitch);
    _lookDirection.x = r * sinf(_yaw);
    _lookDirection.y = sinf(_pitch);
    _lookDirection.z = r * cosf(_yaw);
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
    return XMMatrixLookToRH(XMLoadFloat3(&_position), XMLoadFloat3(&_lookDirection), XMLoadFloat3(&_upDirection));
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(float fov, float aspectRatio, float nearPlane /*= 1.0f*/, float farPlane /*= 1000.0f*/)
{
    return XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::SetMoveSpeed(float unitsPerSecond)
{
    _moveSpeed = unitsPerSecond;
}

void Camera::SetTurnSpeed(float radiansPerSecond)
{
    _turnSpeed = radiansPerSecond;
}

void Camera::OnKeyDown(WPARAM key)
{
    switch (key)
    {
    case 'W':
        _keysPressed.w = true;
        break;
    case 'A':
        _keysPressed.a = true;
        break;
    case 'S':
        _keysPressed.s = true;
        break;
    case 'D':
        _keysPressed.d = true;
        break;
    case VK_LEFT:
        _keysPressed.left = true;
        break;
    case VK_RIGHT:
        _keysPressed.right = true;
        break;
    case VK_UP:
        _keysPressed.up = true;
        break;
    case VK_DOWN:
        _keysPressed.down = true;
        break;
    case VK_ESCAPE:
        Reset();
        break;
    }
}

void Camera::OnKeyUp(WPARAM key)
{
    switch (key)
    {
    case 'W':
        _keysPressed.w = false;
        break;
    case 'A':
        _keysPressed.a = false;
        break;
    case 'S':
        _keysPressed.s = false;
        break;
    case 'D':
        _keysPressed.d = false;
        break;
    case VK_LEFT:
        _keysPressed.left = false;
        break;
    case VK_RIGHT:
        _keysPressed.right = false;
        break;
    case VK_UP:
        _keysPressed.up = false;
        break;
    case VK_DOWN:
        _keysPressed.down = false;
        break;
    }
}

void Camera::Reset()
{
    _position = _initialPosition;
    _yaw = XM_PI;
    _pitch = 0.f;
    _lookDirection = { 0, 0, -1 };
}

}
