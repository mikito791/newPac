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
	XMFLOAT3 boxSize = { 0.2f,1.0f,1.0f };
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.01f);
	//BoxCollider* collision2 = new BoxCollider(transform_.rotate_, boxSize);
	AddCollider(collision);
	//AddCollider(collision2);
	transform_.position_ = XMFLOAT3(4, 0, 3);
	transform_.rotate_= XMFLOAT3(0, 0, 0);
}

void RedWall::Update()
{
	
	if(Input::IsKeyDown(DIK_LEFT))
	{
		transform_.rotate_= XMFLOAT3(0, 270, 0);
		transform_.position_ = XMFLOAT3(3, 0, 2);
	}
	if (Input::IsKeyDown(DIK_RIGHT))
	{
		transform_.rotate_= XMFLOAT3(0, 90, 0);
		transform_.position_ = XMFLOAT3(5, 0, 2);
	}
	if (Input::IsKeyDown(DIK_UP))
	{
		transform_.rotate_= XMFLOAT3(0, 0, 0);
		transform_.position_ = XMFLOAT3(4, 0, 3);
	}
	if (Input::IsKeyDown(DIK_DOWN))
	{
		transform_.rotate_= XMFLOAT3(0, 180, 0);
		transform_.position_ = XMFLOAT3(4, 0, 1);
	}
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
	//�c�[
	//�� 
	
	//if (Input::IsKeyDown(DIK_UP))//�O����
	//{
	//	//����
	//	this->transform_.rotate_.y = 0;
	//	this->transform_.rotate_.x = 0;
	//	//�ʒu
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.z = XMVectorGetZ(rotVecY);
	//}
	//if (Input::IsKeyDown(DIK_DOWN))//������
	//{
	//	//����
	//	this->transform_.rotate_.y = 180;
	//	this->transform_.rotate_.x = 0;
	//	//�ʒu
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.z = XMVectorGetZ(rotVecY);
	//}
	//if (Input::IsKeyDown(DIK_RIGHT))//�E����
	//{
	//	//����
	//	this->transform_.rotate_.y = 90;
	//	this->transform_.rotate_.x = 0;
	//	//�ʒu
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetX(rotVecY);
	//}
	//if (Input::IsKeyDown(DIK_LEFT))//������
	//{
	//	//����
	//	this->transform_.rotate_.y = 270;
	//	this->transform_.rotate_.x = 0;
	//	//�ʒu
	//	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//	rotVecY = XMVector3TransformCoord(front, rotY);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetX(rotVecY);
	//}
	//
	//if (Input::IsKeyDown(DIK_W))//�����
	//{
	//	//����
	//	this->transform_.rotate_.x = 270;
	//	this->transform_.rotate_.y = 0;
	//	//�ʒu
	//	rotX = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.x));
	//	rotVecX = XMVector3TransformCoord(front, rotX);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetY(rotVecX);
	//}
	//
	//if (Input::IsKeyDown(DIK_S))//������
	//{
	//	//����
	//	this->transform_.rotate_.x = 90;
	//	this->transform_.rotate_.y = 0;
	//	//�ʒu
	//	rotX = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.x));
	//	rotVecX = XMVector3TransformCoord(front, rotX);
	//	//XMVECTOR move = rotVecY * moveSpeed;
	//	this->transform_.position_.x = XMVectorGetY(rotVecX);
	//}
	////��]�s������߂�
	//rotX = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.x));
	//rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	////�x�N�g���̉�]���ʂ����߂�
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
	Model::SetTransform(hRedWall, transform_);
	Model::Draw(hRedWall);
}

void RedWall::Release()
{
}
