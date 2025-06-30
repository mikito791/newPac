#pragma once
#include "Engine/GameObject.h"
#include "Engine/Global.h"
#include"Engine/CsvReader.h"
class RedWall :
    public GameObject
{
	int hRedWall;
	CsvReader csv;
public:
	RedWall(GameObject* parent);
	~RedWall();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void SetPosition(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetDirection(Direction dir) { transform_.rotate_.y = GetRotationFromDirection(dir); }
	XMFLOAT3 GetPos() const { return transform_.position_; }
private:
	Direction GetDirectionFromInput();
	int GetRotationFromDirection(Direction dir);
	XMFLOAT3 GetPositionFromDirection(Direction dir);
};

