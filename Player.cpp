#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"Engine/Audio.h"
#include"Engine/Debug.h"

#include"NeedleBall.h"
#include"Hp.h"
#include"HealBall.h"
#include"Stage.h"
#include"ReversalBall.h"



namespace
{
	bool isBlinking = false;         // ダメージ演出中（点滅中）かどうか
	float blinkDuration = 1.0f;      // 点滅全体の長さ（1秒など）
	float blinkTimer = 0.0f;         // 残り点滅時間
	float blinkInterval = 0.1f;      // 点滅のON/OFFの間隔
	float blinkToggleTimer = 0.0f;   // 次に切り替えるまでの時間

	bool isVisible = true;           // 現在表示されているか
	Direction Front = FRONT;
	const float reversalDuration = 5.0f; // 反転の持続時間（秒）
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hPlayer = -1;
	prevSpaceKey = true;
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
	//
	pFlashLight = new FlashLight(this);
	pShield = new Shield(this);	
	pFlashLight->Initialize();
	pShield->Initialize();
}

void Player::Update()
{
	float deltaTime = GetDeltaTime(); // デルタタイムを取得
	
	//入力処理
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection);
	//盾の位置と向きを更新
	ShieldTrans.position_ = GetPositionFromDirection(currentDirection);
	ShieldPos = ShieldTrans.position_;
	ShieldTrans.rotate_.y = GetRotationFromDirection(currentDirection);
	ShieldRotY = ShieldTrans.rotate_.y;
	//懐中電灯の位置と向きを更新
	FlashLightTrans.position_ = GetPositionFromDirection(currentDirection);
	FlashLightPos = FlashLightTrans.position_;
	FlashLightTrans.rotate_.y = GetRotationFromDirection(currentDirection);
	FlashLightRotY = FlashLightTrans.rotate_.y;

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
	
	//レイが当たったら
	if (data.hit)
	{
		transform_.position_.y -= data.dist;
	}
	
	if (Input::IsKeyDown(DIK_SPACE))
	{
		//盾と懐中電灯を切り替える
		prevSpaceKey = !prevSpaceKey;
	}
	if (prevSpaceKey)
	{
		//pShield->SetTransform(ShieldPos, ShieldRotY);
		pShield->Update();
	}
	else
	{
		//pFlashLight->SetTransform(FlashLightPos, FlashLightRotY);
		pFlashLight->Update();
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
	if (prevSpaceKey)
	{
		pShield->SetTransform(ShieldPos, ShieldRotY);
		pShield->Draw();
	}
	else
	{
		pFlashLight->SetTransform(FlashLightPos, FlashLightRotY);
		pFlashLight->Draw();
	}
	if (!isVisible) return; // 非表示状態なら描画スキップ
	Model::SetTransform(hPlayer, transform_);
	Model::Draw(hPlayer);

	if (!IsConfusion)return;
	Model::SetTransform(hConfusion, ConTrans);
	Model::Draw(hConfusion); // 混乱エフェクトの描画
}

void Player::Release()
{
	if (hPlayer >= 0)
	{
		Model::Release(hPlayer);
		hPlayer = -1;
	}
	if (hConfusion >= 0)
	{
		Model::Release(hConfusion); // 混乱エフェクトのモデルを解放
		hConfusion = -1;
	}
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "NeedleBall")
	{
		HpDown(0.5);
		// 点滅開始
		isBlinking = true;
		blinkTimer = blinkDuration;
		blinkToggleTimer = 0.0f;
		isVisible = false;               // 最初は非表示から始める
	}
	if (pTarget->GetObjectName() == "HealBall")
	{
		HpUp(5);
		pTarget->KillMe();
		Audio::Play(hHealSound); // ヒール音を再生
	}
	if (pTarget->GetObjectName() == "ReversalBall")
	{
		pTarget->KillMe(); // ReversalBallに当たったら自分を削除
		//playerの操作を反転
		ReversalBall* pReversalBall = dynamic_cast<ReversalBall*>(pTarget);
		Direction reversalDirection = pReversalBall->GetReveralDirectionFromInput();
		transform_.rotate_.y = pReversalBall->GetRotationFromReveralDirection(reversalDirection);
		//Shieldに反転を通知
		Shield* pShield = (Shield*)FindObject("Shield");
		if (pShield)
		{
			pShield->StartReversalShield();
		}
		//FlashLightに反転を通知
		FlashLight* pFlashLight = (FlashLight*)FindObject("FlashLight");
		if (pFlashLight)
		{
			pFlashLight->StartReversalLight();
		}
		//反転状態にする
		onReversal = true;
		reversalTimer = 0.0f;
		IsConfusion = true; // 混乱エフェクトON
	}
	if (pTarget->GetObjectName() == "Bomb")
	{
		HpDown(10);
		// 点滅開始
		isBlinking = true;
		blinkTimer = blinkDuration;
		blinkToggleTimer = 0.0f;
		isVisible = false;               // 最初は非表示から始める
	}
	if (pTarget->GetObjectName() == "Ghost")
	{
		HpDown(0.05);
		Audio::Play(hDamageSound); // ダメージ音を再生
		// 点滅開始
		isBlinking = true;
		blinkTimer = blinkDuration;
		blinkToggleTimer = 0.0f;
		isVisible = false;               // 最初は非表示から始める
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

XMFLOAT3 Player::GetPositionFromDirection(Direction dir)
{
	//csvで位置を取得できるようにする
	float x, y, z;
	switch (dir)
	{
	case LEFT:
		x = csv.GetValue(1, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case RIGHT:
		x = csv.GetValue(4, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case FRONT:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(3, 2);
		return XMFLOAT3(x, y, z);
		break;
	case BACK:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(4, 2);
		return XMFLOAT3(x, y, z);
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

void Player::Invincible()
{
	if (isBlinking)
	{
		blinkTimer -= GetDeltaTime();
		blinkToggleTimer -= GetDeltaTime();

		// 点滅切り替え
		if (blinkToggleTimer <= 0.0f)
		{
			isVisible = !isVisible;
			blinkToggleTimer = blinkInterval;
		}

		// 点滅終了
		if (blinkTimer <= 0.0f)
		{
			isBlinking = false;
			isVisible = true;  // 最後に表示で終了
		}
	}
}
