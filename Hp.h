#pragma once
#include "Engine/GameObject.h"
class Hp :
    public GameObject
{
	int hp;//�̗́@���l��5
	int hHp;//�̗̓n���h��
	int hHpFrame;//�̗̓t���[���n���h��
public:
	Hp(GameObject* parent);
	~Hp();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	int GetHp() { return hp; } //hp���擾����֐�
	void SetHp(int value) { hp = value; } //hp��ݒ肷��֐�
	void DecreaseHp(int value) { hp -= value; } //hp������������֐�
};

