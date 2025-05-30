#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Hp.h"
#include"Engine/Debug.h"
#include"AllyBall.h"

namespace
{
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
	Direction Front = FRONT;
}

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
	transform_.position_ = XMFLOAT3(4, 0, 2);
	transform_.rotate_ = XMFLOAT3(0, 0, 0);
	//コライダー
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
	transform_.rotate_.y = GetRotationFromDirection(Front); // 初期方向を前に設定
	prevSpaceKey = false;
	HP = 50;
	Point = 0;
}

void Player::Update()
{
	//入力処理
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection);
	/*if (Input::IsKeyDown(DIK_LEFT))
	{
		transform_.rotate_.y = 270;
	}
	if (Input::IsKeyDown(DIK_RIGHT))
	{
		transform_.rotate_.y = 90;
	}
	if (Input::IsKeyDown(DIK_UP))
	{
		transform_.rotate_.y = 0;
	}
	if (Input::IsKeyDown(DIK_DOWN))
	{
		transform_.rotate_.y = 180;
	}*/
	
	//カメラ
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);

	//無敵処理
	if (isInvincible)
	{
		invincibilityTimer -= GetDeltaTime();
		blinkTimer -= GetDeltaTime();

		// 点滅制御：0.1秒ごとに表示・非表示を切り替える
		if (blinkTimer <= 0.0f)
		{
			isVisible = !isVisible;
			blinkTimer = 0.1f; // 点滅間隔
		}

		if (invincibilityTimer <= 0.0f)
		{
			isInvincible = false;
			isVisible = true; // 最終的に表示に戻す
		}
	}
	else
	{
		isVisible = true; // 無敵でなければ常に表示
	}
	if (Input::IsKeyDown(DIK_X))
	{
		transform_.position_.y -= 0.001f;
		if (transform_.position_.y >= -1)
		{
			transform_.position_.y = -1;
		}
	}
	else if (Input::IsKeyUp(DIK_X))
	{
		transform_.position_.y += 0.001f;
		if (transform_.position_.y <= 0)
		{
			transform_.position_.y = 0;
		}
	}

}

void Player::Draw()
{
	if (!isVisible) return; // 非表示状態なら描画スキップ
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void Player::Release()
{
}

float Player::CalculateDistanceEnemy(const XMFLOAT3& PlayPos, const XMFLOAT3& EnemyPos)
{
	//2点間の距離を計算
	float dx = PlayPos.x - EnemyPos.x;
	float dy = PlayPos.y - EnemyPos.y;
	float dz = PlayPos.z - EnemyPos.z;

	//距離を返す(計算)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "RedEnemy")
	{
		HpDown(1);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
		if (HP <= 0)
		{
			this->KillMe();
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
		}
	}
	if (pTarget->GetObjectName() == "AllyBall")
	{
		PointUp(1);
		pTarget->KillMe();
		if (Point >= 5)
		{
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR);
		}
	}
}

Direction Player::GetDirectionFromInput()
{
	static Direction lastDirection = FRONT;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT)|| Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP)   || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;

	return lastDirection;
}

int Player::GetRotationFromDirection(Direction dir)
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

void Player::HpDown(int hp)
{
	HP -= hp;
}

float Player::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}

void Player::PointUp(int pt)
{
	Point += pt;
}
