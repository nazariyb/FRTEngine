#pragma once
#include "GameObject.h"
#include <vector>
#include "Render\Mesh.h"
#include "App.h"

class Tetromino : public frt::GameObject
{
public:
    Tetromino();
    //Tetromino(class frt::App* owner, frt::Mesh::SceneObjectConstantBuffer* buffer);

    inline void RotatePitch(float pitchDelta) { _pitch += pitchDelta; };
    inline void RotateYaw(float yawDelta) { _yaw += yawDelta; };
    inline void RotateRoll(float rollDelta) { _roll += rollDelta; };
    //void Rotate(float Pitch, float Yaw, float Roll);
    void UpdateConstantBuffers(frt::Mesh::SceneObjectConstantBuffer* buffer);

    virtual void InitializeGraphicsResources(class frt::Graphics* graphics) override;
    virtual void InitializeConstantBuffers(class frt::Graphics* graphics) override;
    virtual void PopulateCommandList() override;

private:
    std::vector<frt::Mesh*> _meshes;
    frt::App* _owner;
    float _pitch;
    float _yaw;
    float _roll;
};

