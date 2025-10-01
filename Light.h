#pragma once
#include "Engine/GameObject.h"
class Light :
    public GameObject
{
	int hLight;
public:
	Light(GameObject* parent);
	~Light();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

