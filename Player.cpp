#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"

enum CAM_TYPE
{
	TEST_TYPE,
	FIXED_TYPE,
	FPS_TYPE,
	//TPS_TYPE,
	MAX_TYPE,
};

Player::Player(GameObject* parent)
{
}

void Player::Initialize()
{
	hModel = Model::Load("Assets/Model/Player.fbx");
	assert(hModel >= 0);
	/*x = 0.5;
	y = 10;
	z = -5;*/
	speed = 0.05;
	front = XMVECTOR({ 0,0,1,0 });
}

void Player::Update()
{
	XMMATRIX rotY = XMMatrixIdentity();
	XMVECTOR move{ 0,0,0,0 };
	XMVECTOR rotVec{ 0,0,0,0 };
	float dir = 0;
	//if (Input::IsKey(DIK_UP))//前に進む
	//{
	//	transform_.position_.x += 1;
	//}
	//if (Input::IsKey(DIK_DOWN))//バック
	//{
	//	transform_.position_.x -= 1;
	//}
	//if (Input::IsKey(DIK_LEFT))//左向く
	//{
	//	transform_.rotate_.z -= 1;
	//}
	//if (Input::IsKey(DIK_RIGHT))//右向く
	//{
	//	transform_.rotate_.z += 1;
	//}
	if (Input::IsKey(DIK_UP))//前に進む
	{
		dir = 1.0;
	}
	//if (Input::IsKey(DIK_DOWN))//バック
	//{
	//	dir = -1.0;
	//}
	if (Input::IsKey(DIK_LEFT))//左向く
	{
		this->transform_.rotate_.y -= 3;
	}
	if (Input::IsKey(DIK_RIGHT))//右向く
	{
		this->transform_.rotate_.y += 3;
	}
	/*if (Input::IsKey(DIK_A))
	{
		x += 1;
		z += 1;
	}
	if (Input::IsKey(DIK_D))
	{
		x -= 1;
		z -= 1;
	}*/
	//回転行列を求める
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//ベクトルの回転結果を求める
	rotVec = XMVector3TransformCoord(front, rotY);
	move = speed * rotVec;
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	pos = pos + dir * move;
	XMStoreFloat3(&(transform_.position_), pos);

	//カメラのいろいろ
	if (Input::IsKeyDown(DIK_Z))
	{
		camState++;
		if (camState == CAM_TYPE::MAX_TYPE)
			camState = CAM_TYPE::TEST_TYPE;
	}

	switch (camState)
	{
	case CAM_TYPE::TEST_TYPE:
	{
		XMFLOAT3 camposplay = transform_.position_;
		XMVECTOR camplay = XMLoadFloat3(&(camposplay));
		Camera::SetTarget(camplay);
		//Camera::SetTarget(transform_.position_);
		XMVECTOR vEye{ 0,10,-10,0 };
		vEye = XMVector3TransformCoord(vEye, rotY);
		XMFLOAT3 camPos;
		XMStoreFloat3(&camPos, pos + vEye);
		XMVECTOR Campos = XMLoadFloat3(&camPos);
		Camera::SetPosition(Campos);
		break;
	}
	case CAM_TYPE::FIXED_TYPE:
	{
		Camera::SetPosition(XMVectorSet(0.5, 10, -5, 0));
		Camera::SetTarget(XMVectorSet(0.5, 0, 5, 0));
		break;
	}
	case CAM_TYPE::FPS_TYPE:
	{
		XMVECTOR pos2 = XMLoadFloat3(&(transform_.position_));
		Camera::SetPosition(pos);
		XMFLOAT3 camTarget;
		XMStoreFloat3(&camTarget, pos + move);
		XMVECTOR camTarget3 = XMLoadFloat3(&camTarget);
		Camera::SetTarget(camTarget3);
		break;
	}
	default:
		break;
	}
	
	
}

void Player::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
}
