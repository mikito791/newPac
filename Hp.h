#pragma once
#include "Engine/GameObject.h"
class Hp :
    public GameObject
{
	int hp;//�̗́@���l��5
	int hHp;//�̗̓n���h��
	int hHpFrame;//�̗̓t���[���n���h��
	XMFLOAT3 hpPos;//�̗͂̈ʒu
	XMFLOAT3 hpFramePos;//�̗̓t���[���̈ʒu
	Transform hpFrameTransform;//�̗̓t���[����Transform
	Transform hpTransform;//�̗͂�Transform
public:
	Hp(GameObject* parent);
	~Hp();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

