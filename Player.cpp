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
	const float reversalDuration = 5.0f; // 反転の持続時間（秒）
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hPlayer = -1;
	prevSpaceKey = false;
	HP = 50; // 初期HP
	MaxHP = 50;
	hHealSound = -1; // ヒール音のハンドル
	hDamageSound = -1; // ダメージ音のハンドル
}

Player::~Player()
{
}

void Player::Initialize()
{
	hPlayer = Model::Load("Model/Player.fbx");
	assert(hPlayer >= 0);
	hConfusion = Model::Load("Model/Confusion.fbx"); // 混乱エフェクトのモデル読み込み
	assert(hConfusion >= 0);
	hHealSound = Audio::Load("Sound/Heal.wav"); // ヒール音の読み込み
	assert(hHealSound >= 0);
	hDamageSound = Audio::Load("Sound/Damage.wav"); // ダメージ音の読み込み
	assert(hDamageSound >= 0);
	//transform_.position_.y = 10;
	transform_.position_ = XMFLOAT3(4, 0, 2);
	transform_.rotate_ = XMFLOAT3(0, 0, 0);
	//コライダー
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
	csv.Load("CSV/Player.csv");
	onReversal = false;
	reversalTimer = 0.0f; // 反転タイマーの初期化
}

void Player::Update()
{
	float deltaTime = GetDeltaTime(); // デルタタイムを取得
	
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
		float correction = data.dist * correctionSpeed * deltaTime; // 補正値を計算
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

	//操作反転
	if (onReversal)
	{
		reversalTimer += deltaTime;
		if (reversalTimer >= reversalDuration)
		{
			onReversal = false;
			reversalTimer = 0.0f;
			IsConfusion = false; // 反転が終了したら混乱状態を解除
		}
	}

	//混乱エフェクト処理
	ConTrans.position_ = transform_.position_;
	ConTrans.rotate_.y += 3; // プレイヤーの向きに合わせる

}

void Player::Draw()
{	
	if (!isVisible) return; // 非表示状態なら描画スキップ
	Model::SetTransform(hPlayer, transform_);
	Model::Draw(hPlayer);

	if (!IsConfusion)return;
	Model::SetTransform(hConfusion, ConTrans);
	Model::Draw(hConfusion); // 混乱エフェクトの描画
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
		HpDown(0.5);
		Audio::Play(hDamageSound); // ダメージ音を再生
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
	}
	if (pTarget->GetObjectName() == "HealBall")
	{
		HpUp(5);
		pTarget->KillMe();
		Audio::Play(hHealSound); // ヒール音を再生
	}
	if (pTarget->GetObjectName() == "ReversalBall")
	{
		//1:ReversalBallに当たったら操作を反転させる
		pTarget->KillMe(); // ReversalBallに当たったら自分を削除
		ReversalBall* pReversalBall = dynamic_cast<ReversalBall*>(pTarget);
		Direction reversalDirection = pReversalBall->GetReveralDirectionFromInput();
		transform_.rotate_.y = pReversalBall->GetRotationFromReveralDirection(reversalDirection);
		//Shieldに反転を通知
		Shield* pShield = (Shield*)FindObject("Shield");
		if (pShield)
		{
			pShield->StartReversal();
		}
		onReversal = true;
		reversalTimer = 0.0f;
		IsConfusion = true; // 混乱状態にする
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
		Audio::Play(hDamageSound); // ダメージ音を再生
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // 初期化してすぐ点滅開始
	}
}

Direction Player::GetDirectionFromInput() const
{
	static Direction lastDirection;

	/*if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;*/
	if (!onReversal) 
	{
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	}
	else
	{
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = BACK;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT;
	}
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
