#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"RedWall.h"

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel(-1), front({ 0,0,1,0 })
{
	
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel = Model::Load("Model/Player.fbx");
	assert(hModel >= 0);
	front = XMVECTOR({ 0,0,1,0 });
	//transform_.position_.y = 10;
	SphereCollider* collision = new SphereCollider(transform_.position_, 1.2f);
	AddCollider(collision);
	transform_.position_=XMFLOAT3(4, 0, 2);
	RedWall* wall = Instantiate<RedWall>(this);
}

void Player::Update()
{
	XMMATRIX rotY = XMMatrixIdentity();
	XMMATRIX rotX = XMMatrixIdentity();
	XMVECTOR move{ 0,0,0,0 };
	XMVECTOR rotVecY{ 0,0,0,0 };
	XMVECTOR rotVecX{ 0,0,0,0 };
	
	if(Input::IsKey(DIK_SPACE))
	{
		Instantiate<RedWall>(this);
		
	}
	////キーを押して移動
	//if (Input::IsKey(DIK_UP))//前向く
	//{
	//	this->transform_.rotate_.y = 0;
	//	dir = 1.0;
	//}
	//if (Input::IsKey(DIK_DOWN))//後ろ向く
	//{
	//	this->transform_.rotate_.y = 180;
	//	dir = 1.0;
	//}
	//if (Input::IsKey(DIK_RIGHT))//右向く
	//{
	//	this->transform_.rotate_.y = 90;
	//	dir = 1.0;
	//}
	//if (Input::IsKey(DIK_LEFT))//左向く
	//{
	//	this->transform_.rotate_.y = 270;
	//	dir = 1.0;
	//}
	//
	////方向だけを決めて勝手に移動
	//dir = 1.0;
	if (Input::IsKeyDown(DIK_UP))//前向く
	{
		this->transform_.rotate_.y = 0;
		this->transform_.rotate_.x = 0;
	}
	if (Input::IsKeyDown(DIK_DOWN))//後ろ向く
	{
		this->transform_.rotate_.y = 180;
		this->transform_.rotate_.x = 0;
	}
	if (Input::IsKeyDown(DIK_RIGHT))//右向く
	{
		this->transform_.rotate_.y = 90;
		this->transform_.rotate_.x = 0;
	}
	if (Input::IsKeyDown(DIK_LEFT))//左向く
	{
		this->transform_.rotate_.y = 270;
		this->transform_.rotate_.x = 0;
	}
	
	if (Input::IsKeyDown(DIK_W))
	{
		this->transform_.rotate_.x = 270;
		this->transform_.rotate_.y = 0;
	}
	if (Input::IsKeyDown(DIK_S))
	{
		this->transform_.rotate_.x = 90;
		this->transform_.rotate_.y = 0;
	}
	rotX = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.x));
	//回転行列を求める
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//ベクトルの回転結果を求める
	rotVecY = XMVector3TransformCoord(front, rotY);
	rotVecX = XMVector3TransformCoord(front, rotX);
	//move = speed * rotVec;
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	XMStoreFloat3(&(transform_.position_), pos);
	

	//カメラのいろいろ
	
	//switch (camState)
	//{
	//case CAM_TYPE::TPS_TYPE:
	//{
	//	XMFLOAT3 camposplay = transform_.position_;
	//	//XMVECTOR camplay = XMLoadFloat3(&(camposplay));
	//	Camera::SetTarget(camposplay);
	//	//Camera::SetTarget(transform_.position_);
	//	XMVECTOR vEye{ 0,10,-10,0 };
	//	vEye = XMVector3TransformCoord(vEye, rotY);
	//	XMFLOAT3 camPos;
	//	XMStoreFloat3(&camPos, pos + vEye);
	//	//XMVECTOR Campos = XMLoadFloat3(&camPos);
	//	Camera::SetPosition(camPos);
	//	break;
	//}
	//case CAM_TYPE::FIXED_TYPE:
	//{
	//	XMFLOAT3 camPos = transform_.position_;
	//	camPos.y = transform_.position_.y + 10.0f;
	//	camPos.z = transform_.position_.z - 5.0f;
	//	Camera::SetPosition(camPos);
	//	Camera::SetTarget(transform_.position_);
	//	break;
	//}
	//case CAM_TYPE::FPS_TYPE:
	//{
	//	//XMFLOAT3 pos2 = XMFLOAT3(transform_.position_.x+1,transform_.position_.y+1,transform_.position_.z+1);
	//	Camera::SetPosition(transform_.position_);
	//	//Camera::SetPosition(pos2);
	//	XMFLOAT3 camTarget;
	//	XMStoreFloat3(&camTarget, pos + move);
	//	XMVECTOR camTarget3 = XMLoadFloat3(&camTarget);
	//	Camera::SetTarget(camTarget);
	//	break;
	//}
	//default:
	//	break;
	//}

	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 10.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);	
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


