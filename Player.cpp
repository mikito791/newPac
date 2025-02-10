#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"Stage.h"

float JUMP_HEIGHT = 48.0f * 4.0f;//ジャンプの高さ
const float GRAVITY = 9.8f / 60.0f;//重力加速度

enum CAM_TYPE
{
	FIXED_TYPE,
	TPS_TYPE,
	FPS_TYPE,
	MAX_TYPE,
};

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	
}

void Player::Initialize()
{
	hModel = Model::Load("Model/Player.fbx");
	assert(hModel >= 0);
	/*x = 0.5;
	y = 10;
	z = -5;*/
	speed = 0.05;
	front = XMVECTOR({ 0,0,1,0 });
	//transform_.position_.y = 10;
	SphereCollider* collision = new SphereCollider(transform_.position_, 1.2f);
	AddCollider(collision);
	transform_.position_=XMFLOAT3(14, 0, 1.5);
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
	//if (Input::IsKey(DIK_UP))//前に進む
	//{
	//	dir = 1.0;
	//}
	//if (Input::IsKey(DIK_DOWN))//バック
	//{
	//	dir = -1.0;
	//}

	dir = 1.0;
	if (Input::IsKeyDown(DIK_UP))//前向く
	{
		this->transform_.rotate_.y = 0;
	}
	if (Input::IsKeyDown(DIK_DOWN))//後ろ向く
	{
		this->transform_.rotate_.y = 180;
	}
	if (Input::IsKeyDown(DIK_RIGHT))//右向く
	{
		this->transform_.rotate_.y = 90;
	}
	if (Input::IsKeyDown(DIK_LEFT))//左向く
	{
		this->transform_.rotate_.y = 270;
	}
	/*if (Input::IsKey(DIK_W))
	{
		dir = 1.0;
	}*/

	//回転行列を求める
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//ベクトルの回転結果を求める
	rotVec = XMVector3TransformCoord(front, rotY);
	move = speed * rotVec;
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	pos = pos + dir * move;
	XMStoreFloat3(&(transform_.position_), pos);
	
	
	Stage* pStage = (Stage*)FindObject("Stage");
	int hStageModel = pStage->GetModelHandle();

	RayCastData data;
	data.start=transform_.position_;
	data.start.y = 0;
	data.dir = XMFLOAT3(0, -1, 0);
	Model::RayCast(hStageModel, &data);
	if(data.hit==true)
	{
		transform_.position_.y -= data.dist;
	}

	//カメラのいろいろ
	if (Input::IsKeyDown(DIK_Z))
	{
		camState++;
		if (camState == CAM_TYPE::MAX_TYPE)
			camState = CAM_TYPE::FIXED_TYPE;
	}

	switch (camState)
	{
	case CAM_TYPE::TPS_TYPE:
	{
		XMFLOAT3 camposplay = transform_.position_;
		//XMVECTOR camplay = XMLoadFloat3(&(camposplay));
		Camera::SetTarget(camposplay);
		//Camera::SetTarget(transform_.position_);
		XMVECTOR vEye{ 0,10,-10,0 };
		vEye = XMVector3TransformCoord(vEye, rotY);
		XMFLOAT3 camPos;
		XMStoreFloat3(&camPos, pos + vEye);
		//XMVECTOR Campos = XMLoadFloat3(&camPos);
		Camera::SetPosition(camPos);
		break;
	}
	case CAM_TYPE::FIXED_TYPE:
	{
		XMFLOAT3 camPos = transform_.position_;
		camPos.y = transform_.position_.y + 5.0f;
		camPos.z = transform_.position_.z - 10.0f;
		Camera::SetPosition(camPos);
		Camera::SetTarget(transform_.position_);
		break;
	}
	case CAM_TYPE::FPS_TYPE:
	{
		//XMFLOAT3 pos2 = XMFLOAT3(transform_.position_.x+1,transform_.position_.y+1,transform_.position_.z+1);
		Camera::SetPosition(transform_.position_);
		//Camera::SetPosition(pos2);
		XMFLOAT3 camTarget;
		XMStoreFloat3(&camTarget, pos + move);
		XMVECTOR camTarget3 = XMLoadFloat3(&camTarget);
		Camera::SetTarget(camTarget);
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
	if (pTarget->GetObjectName() == "Stage")
	{
		transform_.position_.y = 0;
	}
}
