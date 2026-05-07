#pragma once

#include"../BaseEnemy/BaseEnemy.h"

class EnemyManager;

//弾を左にまっすぐ撃つだけの敵
class Enemy1 :public BaseEnemy
{
public:
	void Action(float deltaTime)override;
};

//プレイヤーに向かって、複数の弾を撃つ敵
class Enemy2:public BaseEnemy
{
public:
	void Action(float deltaTime)override;
	void InitOriginal(float shotNum, float shotArcRadian);
private:
	int m_shotNum;								//弾を打ち出す数
	float m_shotArcRadian;						//弾を打ち出す弧の角度（プレイヤーを中心に45度ずつ外側まで発射する）
};

//一定の位置まで行き、その場で弾ける
//弾けると敵を全方位に射出
class Enemy3 :public BaseEnemy
{
public:
	Enemy3() = delete;
	Enemy3(EnemyManager*enemyManager):
	m_pEnemyManager(enemyManager)
	{}
	void Action(float deltaTime)override;
	void InitOriginal(float explodePosX,int explodeEnemyNum,float explodeEnemySpeed);
private:
	EnemyManager* m_pEnemyManager;
	
	float m_explodePosX;
	int m_explodeEnemyNum;
	float m_explodeEnemySpeed;		//弾けた敵のスピード
};

//一定の位置まで行き、その場で敵を射出
//生存時間が過ぎると終わる
class Enemy4 :public BaseEnemy
{
public:
	Enemy4() = delete;
	Enemy4(EnemyManager*enemyManager):
	m_pEnemyManager(enemyManager)
	{}
	void Action(float deltaTime)override;
	void InitOriginal(float explodePosX, int explodeEnemyNum, float explodeEnemySpeed, float life, const bool bCanShoot = false);

private:
	EnemyManager* m_pEnemyManager;
	float m_explodePosX;
	int m_explodeEnemyNum;
	float m_explodeEnemySpeed;		//弾けた敵のスピード
	float m_life;
	bool m_bCanShoot;
};

class Enemy5 :public BaseEnemy
{
public:
	void Action(float deltaTime)override;
	void InitOriginal(float deltaRadian,float shotNum,float shotArcRadian);
private:
	
	float m_deltaRadian;		//一秒当たりの変化量
	float m_shotNum;
	float m_shotArcRadian;
};

class Enemy6 :public BaseEnemy
{
public:
	void Action(float deltaTime)override;
	void InitOriginal(int reflectNum);
private:
	
	int m_reflectNum;
};

//敵の動きを初期化時にコントロールする
class Enemy7 :public BaseEnemy
{
public:
	void Action(float deltaTime)override;
	void PushNextPos(Math::Vector2& pos);

	//Spawnを呼び出すときは
	//引数のmoveは値が変わるので不要

	//この関数を呼び出すときはSpawnの後で呼び出す
	void PostInit();

private:
	std::queue<Math::Vector2> m_nextPos;	//次に向かう座標
};

//敵を定位置で回転させる
class Enemy8 :public BaseEnemy
{
public:
	void Action(float deltaTime)override;

	void PostInit(Math::Vector2 rotationPos,float rotationRadius,float deltaRadian);

private:
	
	Math::Vector2 m_rotationPos;	//回転する中心（軸）
	float m_rotationRadius;			//回転する半径
	float m_radian;
	float m_deltaRadian;
	bool m_bRotationFlg;			//回転しているか？
};

