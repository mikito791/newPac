#pragma once
#include "Engine/GameObject.h"
class RedWall :
    public GameObject
{
	int hRedWall;
	int hBlueWall;
public:
	RedWall(GameObject* parent);
	~RedWall();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	
};

