#pragma once
#include "GameObject.h"

namespace frt
{
    class Plane;
}

class BoardBox : public frt::GameObject
{
public:
    BoardBox();
    virtual ~BoardBox();

    void Update() override;
    void PopulateCommandList() override;

    void InitializeGraphicsResources(frt::Graphics* graphics) override;
    void InitializeConstantBuffers(frt::Graphics* graphics) override;
    
private:
    void UpdateConstantBuffers();

    frt::Plane* _plane;
};
