#include "AllyBall.h"
#include "Engine/Model.h"

AllyBall::AllyBall(GameObject* parent)
{
}

void AllyBall::Initialize()
{
	hAllyBall = Model::Load("Model/blueEnemy.fbx");
}

void AllyBall::Update()
{
}

void AllyBall::Draw()
{
}

void AllyBall::Release()
{
}
