#include "Collision.h"
#include"../Game/Chara/Player/Player.h"
#include"../Game//Chara/Enemy/EnemyManager.h"
#include"../Game//Bullet/EnemyBullet.h"
#include"../Timer/Timer.h"
#include"../Game/HUD/HUD.h"
#include"../Fireworks/FireworksManager.h"

//プレイヤー　と　敵
bool Collision(Player* player, std::vector<Enemy>& enemyList)
{
	//プレイヤーが無敵中の場合
	if (player->IsInvincible())return true;

	for (auto& e : enemyList)
	{
		if (!e.IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, e.GetPos(), e.GetRadius().x))
		{
			player->Damage(10);
			player->Invincible(1.0f);				//一秒間
			e.SetActive(false);
			Timer::Instance().Stop(0.2f);		//0.2秒停止
			return true;
		}
	}
	return false;
}

//プレイヤー　と　弾
bool Collision(Player* player, std::vector<EnemyBullet>& bulletList)
{
	//プレイヤーが無敵中の場合
	if (player->IsInvincible())return true;

	for (auto &b : bulletList)
	{
		if (!b.IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, b.GetPos(),b.GetRadius()))
		{
			player->Damage(10);
			Timer::Instance().Stop(0.2f);		//0.2秒停止
			b.SetActive(false);
			return true;
		}
	}
	return false;
}

//弾　と　弾
//bool Collision(Bullets* b1, Bullets* b2)
//{
//	for (auto& B1 : b1->GetBullets())
//	{
//		if (!B1.IsActive())continue;
//		for (auto& B2 : b2->GetBullets())
//		{
//			if (!B2.IsActive())continue;
//			if (IsCollision(B1.GetPos(), Bullet::GetRadius(), B2.GetPos(), Bullet::GetRadius()))
//			{
//				B1.SetActive(false);
//				B2.SetActive(false);
//				return true;
//			}
//		}
//	}
//	return false;
//}

//自機の弾（花火）　と　敵
bool Collision(std::vector<Fireworks>& fireworksList, std::vector<Enemy>& enemyList, HUD* HUD)
{
	for (auto& f : fireworksList)
	{
		if (!f.IsActive())continue;
		for (auto& e : enemyList)
		{
			if (!e.IsActive())continue;
			if (IsCollision(f.GetPos(), f.GetRadius(), e.GetPos(), e.GetRadius().x))
			{
				//花火を弾けさせる
				f.Explode();

				//弾は非活性状態に
				e.SetActive(false);

				//スコアを加算する
				HUD->AddScore(100);
			}
		}
	}

	return false;
}


//円形当たり判定
bool IsCollision(Math::Vector2 pos1,float radius1, Math::Vector2 pos2, float radius2)
{
	float a = pos1.x - pos2.x;
	float b = pos1.y - pos2.y;
	float c = sqrt(a * a + b * b);

	if (c < radius1 + radius2)return true;

	return false;
}

//矩形当たり判定
bool IsCollision_Box(Math::Vector2 pos1, Math::Vector2 radius1, Math::Vector2 pos2, Math::Vector2 radius2)
{
	float left1 = pos1.x - radius1.x;
	float right1 = pos1.x + radius1.x;
	float top1 = pos1.y + radius1.y;
	float bottom1 = pos1.y - radius1.y;

	float left2 = pos2.x - radius2.x;
	float right2 = pos2.x + radius2.x;
	float top2 = pos2.y + radius2.y;
	float bottom2 = pos2.y - radius2.y;
	//左右当たり判定
	if ((left1 <= left2 && right1 >= left2)||
		(right1 >= right2 && left1 <= right2))
	{
		//上下当たり判定
		if ((top1 >= bottom2 && bottom1 <= top2) || 
			(top1 >= bottom2 && bottom1 <= bottom2))
			return true;
	}

	return false;
}
