#pragma once
#include "Engine/GameObject.h"
class Stage :
    public GameObject
{
    int hFloor;
    int hWall;
    int hStage;
    XMFLOAT3 position;
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    int GetModelHandle() { return hStage; }
    XMFLOAT3 GetPos() const { return transform_.position_; }
};

