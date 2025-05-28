#pragma once
#include "Engine/GameObject.h"
class GameClearScene :
    public GameObject
{
	int hGameClear;
public:
	GameClearScene(GameObject* parent);
	~GameClearScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

