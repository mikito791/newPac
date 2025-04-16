#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"RedWall.h"

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel(-1)
{
	
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel = Model::Load("Model/Player.fbx");
	assert(hModel >= 0);
	
	//transform_.position_.y = 10;
	
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.4f);
	AddCollider(collision);
	transform_.position_=XMFLOAT3(4, 0, 2);
}

void Player::Update()
{
	if (Input::IsKeyDown(DIK_LEFT))
	{
		transform_.rotate_ = XMFLOAT3(0, 270, 0);
	}
	if (Input::IsKeyDown(DIK_RIGHT))
	{
		transform_.rotate_ = XMFLOAT3(0, 90, 0);
	}
	if (Input::IsKeyDown(DIK_UP))
	{
		transform_.rotate_ = XMFLOAT3(0, 0, 0);
	}
	if (Input::IsKeyDown(DIK_DOWN))
	{
		transform_.rotate_ = XMFLOAT3(0, 180, 0);
	}
	//ƒJƒƒ‰
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
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
	if (pTarget->GetObjectName() == "RedEnemy")
	{
		this->KillMe();
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}
}


