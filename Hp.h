#pragma once
#include "Engine/GameObject.h"
class Hp :
    public GameObject
{
	int hHp;//�̗̓n���h��
	int hHpFrame;//�̗̓t���[���n���h��
	int hpMaxVal_;//�Q�[�W�̍ő�l
	int hpCrrVal_;//�J�����g�̒l�i���݂̒l�j crrunt value
public:
	Hp(GameObject* parent);
	~Hp();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetHpVal(int crr, int max)
	{
		hpCrrVal_ = crr;
		hpMaxVal_ = max;
	} // �̗͂̒l��ݒ�

};

