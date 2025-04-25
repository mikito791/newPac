#pragma once
#include "Engine/GameObject.h"
class GameOverScene :
    public GameObject
{
	int hGameOver;
public:
	GameOverScene(GameObject* parent);

	~GameOverScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};

