#include "TetrisBoard.h"

const DirectX::XMFLOAT3 TetrisBoard::TopBound    {  0.f, 20.f, 0.f };
const DirectX::XMFLOAT3 TetrisBoard::BottomBound {  0.f,  0.f, 0.f };
const DirectX::XMFLOAT3 TetrisBoard::LeftBound   { -5.f, 10.f, 0.f };
const DirectX::XMFLOAT3 TetrisBoard::RightBound  {  5.f, 10.f, 0.f };

TetrisBoard::TetrisBoard()
{}

void TetrisBoard::Update()
{}
