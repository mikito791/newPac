#pragma once
#include "Engine/GameObject.h"
class Hp :
    public GameObject
{
	int hp;//‘Ì—Í@”’l‚Í5
public:
	Hp(GameObject* parent);
	~Hp();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	int GetHp() { return hp; } //hp‚ğæ“¾‚·‚éŠÖ”
	void SetHp(int value) { hp = value; } //hp‚ğİ’è‚·‚éŠÖ”
	void DecreaseHp(int value) { hp -= value; } //hp‚ğŒ¸­‚³‚¹‚éŠÖ”
};

