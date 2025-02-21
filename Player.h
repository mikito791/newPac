#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
	int hModel;
	XMVECTOR front;
	float speed;
	int camState;
	XMMATRIX rotY = XMMatrixIdentity();
	XMVECTOR move{ 0,0,0,0 };
	XMVECTOR rotVec{ 0,0,0,0 };
	XMVECTOR pos;
	float dir = 0;
public:
	Player(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	//void OnCollision(GameObject* pTarget) override;

	
};

