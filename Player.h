#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
	int hModel;
	XMVECTOR front;
	float speed;
	int camState;
	int x, y, z;
	/*bool prevSpacekey;
	bool OnGround;
	float jumpSpeed;*/
public:
	Player(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void OnCollision(GameObject* pTarget) override;

};

