#pragma once
#include "GameObject.h"
#include <DirectXMath.h>


class TetrisBoard : public frt::GameObject
{
public:
    TetrisBoard();

    virtual void Update() override;

public:
    static const DirectX::XMFLOAT3 TopBound;
    static const DirectX::XMFLOAT3 BottomBound;
    static const DirectX::XMFLOAT3 LeftBound;
    static const DirectX::XMFLOAT3 RightBound;
};

