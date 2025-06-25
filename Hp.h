#pragma once
#include "Engine/GameObject.h"
class Hp :
    public GameObject
{
	int hHp;//体力ハンドル
	int hHpFrame;//体力フレームハンドル
	int hpMaxVal_;//ゲージの最大値
	int hpCrrVal_;//カレントの値（現在の値） crrunt value
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
	} // 体力の値を設定

};

