#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"Shield.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"Engine/Debug.h"
#include"HealBall.h"
#include"Stage.h"
#include"ReversalBall.h"
#include"Engine/Audio.h"



namespace
{
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
	Direction Front = FRONT;
	float JumpHeight = 50.0f; // ジャンプの高さ
	float Gravity = 3.0f / 60.0f; // 重力の強さ
	float MaxGravity = 6.0f; // 最大重力の強さ
	float correctionSpeed = 5.0f; // 補正の速さ（調整可能）

}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hModel = -1;
	hDmageSound = -1;
	hHealSound = -1;
	hBombSound = -1;
	prevSpaceKey = false;
	HP = 50; // 初期HP
	MaxHP = 50;
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
	csv.Load("CSV/variable.csv");
}

void Player::Update()
{
	float deltataTime = GetDeltaTime(); // デルタタイムを取得
	// ReversalBallに当たったら操作を反転させる

	Shield* pShield = (Shield*)FindObject("Shield");
	//入力処理
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection);

	//壁の向きと位置も反転させる
	/*if (pRedWall)
	{
		pRedWall->SetDirection(reversedDirection);
		pRedWall->SetPosition(pReversalBall->GetPositionFromReveralDirection(reversedDirection));
	}*/


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
		float correction = data.dist * correctionSpeed * deltataTime; // 補正値を計算
		//その分位置を下げる
		if (correction > data.dist)
		{
			correction = data.dist; // 補正値が距離を超えないように制限
		}
		transform_.position_.y -= correction;
		onGround = true; // 地面にいる状態にする
		jumpPower = 0.0f; // ジャンプ力をリセット

	}
	else
	{
		onGround = false; // 地面にいない状態にする
	}

	//ジャンプ作る
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (prevSpaceKey == false && onGround)
		{
			Jump();
		}
		prevSpaceKey = true;
	}
	else
	{
		prevSpaceKey = false; // スペースキーが離されたらフラグをリセット

	}

	if (!onGround)
	{
		// 空中にいる場合は重力を適用
		jumpPower += Gravity; // 重力を適用
		if (jumpPower > MaxGravity) // 最大重力を超えないように制限
		{
			jumpPower = MaxGravity;
		}
		transform_.position_.y += jumpPower; // ジャンプの高さを更新
	}
	else
	{
		jumpPower = 0.0f; // 地面にいる場合はジャンプの力をリセット
		onGround = true; // 地面にいる状態にする
	}

	//Debug::Log(transform_.position_.y,"\n"); 
	//if (Input::IsKey(DIK_K))
	//{
	//	transform_.position_.x += 0.1f; // Jキーで上昇
	//}
	//if (Input::IsKey(DIK_L))
	//{
	//	transform_.position_.x -= 0.1f; // Kキーで下降
	//}
	//if (Input::IsKey(DIK_I))
	//{
	//	transform_.position_.z += 0.1f; // Jキーで上昇
	//}
	//if (Input::IsKey(DIK_O))
	//{
	//	transform_.position_.z -= 0.1f; // Kキーで下降
	//}
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

float Player::CalculateDistance(const XMFLOAT3& PlayPos, const XMFLOAT3& Pos)
{
	//2点間の距離を計算
	float dx = PlayPos.x - Pos.x;
	float dy = PlayPos.y - Pos.y;
	float dz = PlayPos.z - Pos.z;

	//距離を返す(計算)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "NeedleBall")
	{
		HpDown(0.2);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
	}
	if (pTarget->GetObjectName() == "HealBall")
	{
		HpUp(5);
		pTarget->KillMe();
	}
	if (pTarget->GetObjectName() == "ReversalBall")
	{
		ReversalBall* pReversalBall = (ReversalBall*)FindObject("ReversalBall");
		Direction reversalDirection = pReversalBall->GetReveralDirectionFromInput();
		transform_.rotate_.y = pReversalBall->GetRotationFromReveralDirection(reversalDirection);
		//壁の向きと位置も反転させる
	}
	if (pTarget->GetObjectName() == "Bomb")
	{
		HpDown(10);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
	}
	if (pTarget->GetObjectName() == "Ghost")
	{
		HpDown(0.05);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
	}
}

Direction Player::GetDirectionFromInput()
{
	static Direction lastDirection;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	/*if (OnReversal == false) {
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_RIGHT)|| Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_UP)   || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	}*/
	//else // 操作を反転させる
	//{
	//	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT; // 左入力をすると右を向く
	//	if (Input::IsKeyDown(DIK_RIGHT)|| Input::IsKeyDown(DIK_D)) lastDirection = LEFT;  // 右入力をすると左を向く
	//	if (Input::IsKeyDown(DIK_UP)   || Input::IsKeyDown(DIK_W)) lastDirection = BACK;  // 上入力をすると後ろを向く
	//	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT; // 下入力をすると前を向く
	//}
	return lastDirection;
}

int Player::GetRotationFromDirection(Direction dir)
{
	int Left, Right, Front, Back;
	switch (dir)
	{
	case LEFT:
		return Left = csv.GetValue(1, 0);
		//return 270;
		break;
	case RIGHT:
		return Right = csv.GetValue(2, 0);
		//return 90;
		break;
	case FRONT:
		return Front = csv.GetValue(3, 0);
		//return 0;
		break;
	case BACK:
		return Back = csv.GetValue(4, 0);
		//return 180;
		break;
	default:
		break;
	}
}

void Player::HpDown(float hp)
{
	HP -= hp;
	if (HP <= 0)
	{
		HP = 0; // HPが0未満にならないように制限
		this->KillMe();
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
	}
	Hp* pHp = (Hp*)(FindObject("Hp"));
	pHp->SetHpVal(HP, MaxHP); // HPの更新
}

void Player::HpUp(int hp)
{
	HP += hp;
	if (HP > MaxHP)
	{
		HP = MaxHP; // HPが最大値を超えないように制限
	}
	Hp* pHp = (Hp*)(FindObject("Hp"));
	pHp->SetHpVal(HP, MaxHP); // HPの更新
}

float Player::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}

void Player::Jump()
{
	jumpPower = sqrt(2.0f * JumpHeight * Gravity); // ジャンプの初速を計算
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
