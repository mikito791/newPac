#include "RedWall.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/Input.h"

RedWall::RedWall(GameObject* parent): 
	GameObject(parent, "RedWall"),hRedWall(-1)
{
	
}

RedWall::~RedWall()
{
}

void RedWall::Initialize()
{
	hRedWall = Model::Load("Model/PlayerWallred0.fbx");
	assert(hRedWall >= 0);
	hBlueWall = Model::Load("Model/PlayerWallblue0.fbx");
	assert(hBlueWall >= 0);
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.01f);
	//BoxCollider* collision2 = new BoxCollider(transform_.rotate_, boxSize);
	AddCollider(collision);
	//AddCollider(collision2);
	transform_.position_ = XMFLOAT3(4, 0, 3);
	transform_.rotate_= XMFLOAT3(0, 0, 0);
	keyPush = false;
}

void RedWall::Update()
{
	
	if(Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A))
	{
		transform_.rotate_.y = 270;
		transform_.position_ = XMFLOAT3(3, 0, 2);
	}
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D))
	{
		transform_.rotate_.y = 90;
		transform_.position_ = XMFLOAT3(5, 0, 2);
	}
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W))
	{
		transform_.rotate_.y = 0;
		transform_.position_ = XMFLOAT3(4, 0, 3);
	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S))
	{
		transform_.rotate_.y = 180;
		transform_.position_ = XMFLOAT3(4, 0, 1);
	}

	if (Input::IsKeyDown(DIK_SPACE))
	{
		keyPush = true;
	}
	else if (Input::IsKeyUp(DIK_SPACE))
	{
		keyPush = false;
	}
	//ジャンプ作る
	
	/*if (Input::IsKeyDown(DIK_W))
	{
		transform_.rotate_= XMFLOAT3(270, 0, 0);
		transform_.position_ = XMFLOAT3(4, 1, 1.5);
	}
	if (Input::IsKeyDown(DIK_S))
	{
		transform_.rotate_ = XMFLOAT3(90, 0, 0);
		transform_.position_ = XMFLOAT3(4, -1, 1.5);
	}*/
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
	////回転行列を求める
	//rotX = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.x));
	//rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	////ベクトルの回転結果を求める
	//rotVecY = XMVector3TransformCoord(front, rotY);
	//rotVecX = XMVector3TransformCoord(front, rotX);
	////move = speed * rotVec;
	//XMFLOAT3 floPos = transform_.position_;
	//XMVECTOR pos = XMLoadFloat3(&(floPos));
	//XMStoreFloat3(&(floPos), pos);
	//
}

void RedWall::Draw()
{
	if (keyPush)
	{
		Model::SetTransform(hBlueWall, transform_);
		Model::Draw(hBlueWall);
	}
	else
	{
		Model::SetTransform(hRedWall, transform_);
		Model::Draw(hRedWall);
	}
}

void RedWall::Release()
{
}
