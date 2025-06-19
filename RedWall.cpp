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
	AddCollider(collision);
	transform_.position_ = GetPositionFromDirection(FRONT);
	transform_.rotate_ = XMFLOAT3(0, 0, 0);
	transform_.rotate_.y = GetRotationFromDirection(FRONT); // ����������O�ɐݒ�
}

void RedWall::Update()
{
	//���͏���
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection); // �������X�V
	//�ʒu���X�V
	transform_.position_ = GetPositionFromDirection(currentDirection); // �ʒu���X�V
	
	//�W�����v���
	
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

Direction RedWall::GetDirectionFromInput()
{
	static Direction lastDirection = FRONT;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;

	return lastDirection;
}

int RedWall::GetRotationFromDirection(Direction dir)
{
	switch (dir)
	{
	case LEFT:
		return 270;
		break;
	case RIGHT:
		return 90;
		break;
	case FRONT:
		return 0;
		break;
	case BACK:
		return 180;
		break;
	default:
		break;
	}
}

XMFLOAT3 RedWall::GetPositionFromDirection(Direction dir)
{
	switch (dir)
	{
	case LEFT:
		return XMFLOAT3(3, 0, 2);
		break;
	case RIGHT:
		return XMFLOAT3(5, 0, 2);
		break;
	case FRONT:
		return XMFLOAT3(4, 0, 3);
		break;
	case BACK:
		return XMFLOAT3(4, 0, 1);
		break;
	default:
		break;
	}
}


