#pragma once
#include "Engine/GameObject.h"
class FlashLight :
    public GameObject
{
    int hFlashLight;
public:
	FlashLight(GameObject* parent);
	~FlashLight();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

