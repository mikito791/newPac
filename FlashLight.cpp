#include "FlashLight.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"
#include"Engine/Debug.h"

namespace
{
	// ���]�̎������ԁi�b�j
	const float reversalDuration = 5.0f;
}

FlashLight::FlashLight(GameObject* parent) 
	:GameObject(parent, "FlashLight")
{
	hFlashLight = -1;
}

FlashLight::~FlashLight()
{
}

void FlashLight::Initialize()
{
	hFlashLight = Model::Load("Model/FlashLight.fbx");
	assert(hFlashLight >= 0);

	transform_.position_ = XMFLOAT3(4, 1.5, 2);
	csv.Load("CSV/Player.csv");
}

void FlashLight::Update()
{
	float deltaTime = GetDeltaTime();
	//���͏���
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection); // �������X�V
	transform_.position_ = GetPositionFromDirection(currentDirection); // �ʒu���X�V
	if (onReversal)
	{
		reversalTimer += deltaTime;
		if (reversalTimer >= reversalDuration)
		{
			onReversal = false;
			reversalTimer = 0.0f;
		}
	}
	if (Input::IsKeyDown(DIK_C))
	{
		//�����d����������o���i��ł��j
	}
}

void FlashLight::Draw()
{
	Model::SetTransform(hFlashLight, transform_);
	Model::Draw(hFlashLight);
}

void FlashLight::Release()
{
}



Direction FlashLight::GetDirectionFromInput() const
{
	static Direction lastDirection;

	if (!onReversal)
	{
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	}
	else
	{
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = BACK;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT;
	}

	return lastDirection;
}

int FlashLight::GetRotationFromDirection(Direction dir)
{
	int Left, Right, Front, Back;
	switch (dir)
	{
	case LEFT:
		return Left = csv.GetValue(1, 0);
		//return 270; // ������
		break;
	case RIGHT:
		return Right = csv.GetValue(2, 0);
		//return 90; // �E����
		break;
	case FRONT:
		return Front = csv.GetValue(3, 0);
		//return 0; // �O����
		break;
	case BACK:
		return Back = csv.GetValue(4, 0);
		//return 180;  //������
		break;
	default:
		break;
	}
}

XMFLOAT3 FlashLight::GetPositionFromDirection(Direction dir)
{
	//csv�ňʒu���擾�ł���悤�ɂ���
	float x, y, z;
	switch (dir)
	{
	case LEFT:
		x = csv.GetValue(1, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case RIGHT:
		x = csv.GetValue(4, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case FRONT:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(3, 2);
		return XMFLOAT3(x, y, z);
		break;
	case BACK:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(4, 2);
		return XMFLOAT3(x, y, z);
		break;
	default:
		break;
	}
}

float FlashLight::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}