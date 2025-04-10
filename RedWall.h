#pragma once
#include "Engine/GameObject.h"
class RedWall :
    public GameObject
{
	int hRedWall;
public:
	RedWall(GameObject* parent);
	~RedWall();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void setPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	XMFLOAT3 GetPos() const { return transform_.position_; }
};

