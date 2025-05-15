#pragma once
#include "Engine/GameObject.h"
class Hp :
    public GameObject
{
	int hp;//体力　数値は5
	int hHp;//体力ハンドル
	int hHpFrame;//体力フレームハンドル
	XMFLOAT3 hpPos;//体力の位置
	XMFLOAT3 hpFramePos;//体力フレームの位置
	Transform hpFrameTransform;//体力フレームのTransform
	Transform hpTransform;//体力のTransform
public:
	Hp(GameObject* parent);
	~Hp();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

