#pragma once
#include "Engine/GameObject.h"
class RedWall :
    public GameObject
{
	int hRedWall;
	XMVECTOR front;
	XMVECTOR pos;
public:
	RedWall(GameObject* parent);
	~RedWall();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void setPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	
};

