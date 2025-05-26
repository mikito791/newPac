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

namespace
{
	int HP = 50;
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
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
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0), 0.3f);
	AddCollider(collider);
	Direction Front = FRONT;
	prevSpaceKey = false;
}

void Player::Update()
{
	//Direction currentDirection = GetDirectionFromInput();
	//transform_.rotate_.y = GetRotationFromDirection(currentDirection);
	if (Input::IsKeyDown(DIK_LEFT))
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
	}
	
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
	//プレイヤーのジャンプ//仮　盾で防げない敵用
	if (Input::IsKey(DIK_SPACE))//ジャンプで上に飛ぶ//上限必要
	{
		if (prevSpaceKey == false)
		{
			//ジャンプ処理
			transform_.position_.y += 10.0f;
		}
		if (transform_.position_.y > 0)
		{
			prevSpaceKey = true;
		}
	}
	else//定位置に止めるtransform_.position_.y = 0にする
	{
		
		prevSpaceKey = false;
	}
	transform_.position_.y -= 0.1f;
	if (transform_.position_.y < 0)
	{
		transform_.position_.y = 0;
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
		Debug::Log(HP);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
		if (HP == 0)
		{
			this->KillMe();
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
		}
	}
}

Direction Player::GetDirectionFromInput()
{
	if (Input::IsKey(DIK_LEFT))  return LEFT;
	if (Input::IsKey(DIK_RIGHT)) return RIGHT;
	if (Input::IsKey(DIK_UP))    return FRONT;
	if (Input::IsKey(DIK_DOWN))  return BACK;
}

int Player::GetRotationFromDirection(Direction dir)
{
	switch (dir)
	{
	case LEFT:  return 270;
	case RIGHT: return 90;
	case FRONT: return 0;
	case BACK:  return 180;
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
