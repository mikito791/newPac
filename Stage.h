#pragma once
#include "Engine/GameObject.h"
enum STG_TYPE
{
    FLOOR,
    WALL,
    MAX,
};
class Stage :
    public GameObject
{
    
    int hFloor;
    int hWall;
    XMFLOAT3 position;
    Transform stageTrans;
    STG_TYPE type;
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    int GetModelHandle() { return hFloor; }
    XMFLOAT3 GetPosition(XMFLOAT3 pos) { return position = pos; }
};

