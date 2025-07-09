#pragma once
#include "Engine/GameObject.h"
class TitleScene :
    public GameObject
{
	int hTitle;
	int alpha;
	int hBackground;
	bool isFadeOut; // フェードアウト状態を管理するフラグ
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	TitleScene(GameObject* parent);
	//デストラクタ
	~TitleScene();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

