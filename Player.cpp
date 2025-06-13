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
#include"Stage.h"

namespace
{
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
	Direction Front = FRONT;
	float JumpHeight = 5.0f; // ジャンプの高さ
	float Gravity = 9.8f/60.0f; // 重力の強さ
	float MaxGravity = 6.0f; // 最大重力の強さ
	float gravityVelocity = 0.0f; // 落下加速度
	float gravityIncrease = 0.2f; // 毎フレーム増加する重力の量
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hModel = -1;
	jumpPower = 0.0f; // ジャンプの力

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
	
	//カメラ
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);

	//無敵処理
	Invincible();
	//
	Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
	int hGroundModel = pStage->GetModelHandle();    //モデル番号を取得

	RayCastData data;
	data.start = transform_.position_;   //レイの発射位置
	data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
	Model::RayCast(hGroundModel, &data); //レイを発射
	onGround = false;//空中がデフォルト
	//レイが当たったら
	if (data.hit)
	{
		//その分位置を下げる
		transform_.position_.y -= data.dist;
		onGround = true; // 地面にいる状態にする
		jumpPower = 0.0f; // ジャンプ力をリセット
		//Debug::Log("Player is on the ground", true);
	}
	//ジャンプ作る
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (prevSpaceKey == false &&onGround)
		{
			Jump();
		}
		prevSpaceKey = true; 
	}
	else
	{
		prevSpaceKey = false; // スペースキーが離されたらフラグをリセット
		
	}
	if (onGround)
	{
		gravityVelocity = 0.0f; // 地面にいる場合は重力をリセット
		jumpPower = 0.0f; // 地面にいる場合はジャンプ力をリセット
	}
	else
	{
		gravityVelocity += gravityIncrease; // 空中にいる場合は重力を増加
		if (gravityVelocity > MaxGravity)
		{
			gravityVelocity = MaxGravity; // 最大重力を超えないように制限
		}
		jumpPower += gravityVelocity; // 重力を適用
		transform_.position_.y += jumpPower; // ジャンプの高さを更新
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

void Player::Jump()
{
	jumpPower = -sqrt(2.0f * JumpHeight * Gravity); // ジャンプの初速を計算
	onGround = false; // ジャンプ中は地面にいないとする
}

void Player::Invincible()
{
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
}
