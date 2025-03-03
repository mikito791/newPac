#pragma once
#include "Engine/GameObject.h"
class RedEnemy :
    public GameObject
{
    int hRedEnemy;
	XMFLOAT3 pos;
public:
	RedEnemy(GameObject* parent);
	~RedEnemy();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

