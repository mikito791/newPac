#include "RedWall.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include"Engine/Input.h"

RedWall::RedWall(GameObject* parent): 
	GameObject(parent, "RedWall"),hRedWall(-1),front({ 0,0,1,0 })
{
	
}

RedWall::~RedWall()
{
}

void RedWall::Initialize()
{
	hRedWall = Model::Load("Model/PlayerWallred.fbx");
	XMFLOAT3 boxSize = { 0.2f,1.0f,1.0f };
	BoxCollider* collision = new BoxCollider(transform_.position_, boxSize);
	AddCollider(collision);
	transform_.position_ = XMFLOAT3(4, 0, 2);
}

void RedWall::Update()
{
	XMMATRIX rotY = XMMatrixIdentity();
	XMMATRIX rotX = XMMatrixIdentity();
	XMVECTOR rotVecY{ 0,0,0,0 };
	XMVECTOR rotVecX{ 0,0,0,0 };
	float moveSpeed = 0.01f;

	if (Input::IsKeyDown(DIK_UP))//前向く
	{
		//向き
		this->transform_.rotate_.y = 0;
		this->transform_.rotate_.x = 0;
		//位置
	}
	if (Input::IsKeyDown(DIK_DOWN))//後ろ向く
	{
		//向き
		this->transform_.rotate_.y = 180;
		this->transform_.rotate_.x = 0;
		//位置
	}
	if (Input::IsKeyDown(DIK_RIGHT))//右向く
	{
		//向き
		this->transform_.rotate_.y = 90;
		this->transform_.rotate_.x = 0;
		//位置
	}
	if (Input::IsKeyDown(DIK_LEFT))//左向く
	{
		//向き
		this->transform_.rotate_.y = 270;
		this->transform_.rotate_.x = 0;
		//位置
	}

	if (Input::IsKeyDown(DIK_W))//上向く
	{
		//向き
		this->transform_.rotate_.x = 270;
		this->transform_.rotate_.y = 0;
		//位置
	}

	if (Input::IsKeyDown(DIK_S))//下向く
	{
		//向き
		this->transform_.rotate_.x = 90;
		this->transform_.rotate_.y = 0;
		//位置
	}
	if(Input::IsKeyDown(DIK_SPACE))
	{
		transform_.rotate_.y = 90;//こうする予定
	}
	//残骸
	//↓ 
	
	//if (Input::IsKeyDown(DIK_UP))//前向く
	//{
	//	//向き
	//	this->transform_.rotate_.y = 0;
	//	this->transform_.rotate_.x = 0;
	//	//位置
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.z = XMVectorGetZ(rotVecY);
	//}
	//if (Input::IsKeyDown(DIK_DOWN))//後ろ向く
	//{
	//	//向き
	//	this->transform_.rotate_.y = 180;
	//	this->transform_.rotate_.x = 0;
	//	//位置
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.z = XMVectorGetZ(rotVecY);
	//}
	//if (Input::IsKeyDown(DIK_RIGHT))//右向く
	//{
	//	//向き
	//	this->transform_.rotate_.y = 90;
	//	this->transform_.rotate_.x = 0;
	//	//位置
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetX(rotVecY);
	//}
	//if (Input::IsKeyDown(DIK_LEFT))//左向く
	//{
	//	//向き
	//	this->transform_.rotate_.y = 270;
	//	this->transform_.rotate_.x = 0;
	//	//位置
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetX(rotVecY);
	//}
	//
	//if (Input::IsKeyDown(DIK_W))//上向く
	//{
	//	//向き
	//	this->transform_.rotate_.x = 270;
	//	this->transform_.rotate_.y = 0;
	//	//位置
	//	rotX = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.x));
	//	rotVecX = XMVector3TransformCoord(front, rotX);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetY(rotVecX);
	//}
	//
	//if (Input::IsKeyDown(DIK_S))//下向く
	//{
	//	//向き
	//	this->transform_.rotate_.x = 90;
	//	this->transform_.rotate_.y = 0;
	//	//位置
	//	rotX = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.x));
	//	rotVecX = XMVector3TransformCoord(front, rotX);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetY(rotVecX);
	//}


	//回転行列を求める
	rotX = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//ベクトルの回転結果を求める
	rotVecY = XMVector3TransformCoord(front, rotY);
	rotVecX = XMVector3TransformCoord(front, rotX);
	//move = speed * rotVec;
	XMFLOAT3 floPos = transform_.position_;
	XMVECTOR pos = XMLoadFloat3(&(floPos));
	XMStoreFloat3(&(floPos), pos);
	
}

void RedWall::Draw()
{
	Model::SetTransform(hRedWall, transform_);
	Model::Draw(hRedWall);
}

void RedWall::Release()
{
}
