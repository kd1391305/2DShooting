#include "Collision.h"
#include"../Game/Chara/Player/Player.h"
#include"../Game//Chara/Enemy/EnemyManager.h"
#include"../Game//Bullet/EnemyBullet.h"
#include"../Game/Bullet/PlayerBullet.h"
#include"../Game/GameTimer.h"
#include"../Game/FadeEffect/FadeEffectManager.h"
#include"../Game/HUD/HUD.h"

//プレイヤー　と　敵
bool Collision(C_Player* player, std::vector<C_Enemy>& enemyList)
{
	for (auto& e : enemyList)
	{
		if (!e.IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, e.GetPos(), e.GetRadius().x))
		{
			player->Damage(10);
			player->Invincible(60);				//6０フレーム無敵になる
			e.SetActive(false);
			FADE_EFFECT.Emit(e.GetPos(), e.GetRadius().x);
			GAME_TIMER.Stop(200);		//0.2秒停止
			return true;
		}
	}
	return false;
}

//プレイヤー　と　過去の敵
bool Collision(C_Player* player, std::vector<C_FormerEnemy>& fEnemyList)
{
	for (auto& e : fEnemyList)
	{
		if (!e.IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, e.GetPos(), e.GetRadius().x))
		{
			player->Damage(10);
			player->Invincible(60);				//6０フレーム無敵になる
			e.SetActive(false);
			FADE_EFFECT.Emit(e.GetPos(), e.GetRadius().x);
			GAME_TIMER.Stop(200);		//0.2秒停止
			return true;
		}
	}
	return false;
}

//プレイヤー　と　弾
bool Collision(C_Player* player, std::vector<C_EnemyBullet>& bulletList)
{
	for (auto &b : bulletList)
	{
		if (!b.IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, b.GetPos(),b.GetRadius()))
		{
			player->Damage(10);
			b.SetActive(false);
			FADE_EFFECT.Emit(b.GetPos(), b.GetRadius());
			return true;
		}
	}
	return false;
}

//弾　と　弾
//bool Collision(C_Bullets* b1, C_Bullets* b2)
//{
//	for (auto& B1 : b1->GetBullets())
//	{
//		if (!B1.IsActive())continue;
//		for (auto& B2 : b2->GetBullets())
//		{
//			if (!B2.IsActive())continue;
//			if (IsCollision(B1.GetPos(), C_Bullet::GetRadius(), B2.GetPos(), C_Bullet::GetRadius()))
//			{
//				B1.SetActive(false);
//				B2.SetActive(false);
//				return true;
//			}
//		}
//	}
//	return false;
//}

//自機の弾　と　敵
bool Collision(std::vector< C_PlayerBullet>& bulletList, std::vector<C_Enemy>& enemyList, C_HUD* HUD)
{
	for (auto& b : bulletList)
	{
		if (!b.IsActive())continue;
		for (auto& e : enemyList)
		{
			if (!e.IsActive())continue;
			if (IsCollision(b.GetPos(), b.GetRadius(), e.GetPos(), e.GetRadius().x))
			{
				b.SetActive(false);
				e.SetActive(false);
				HUD->AddScore(100);
				FADE_EFFECT.Emit(e.GetPos(), e.GetRadius().x);
			}
		}
	}

	return false;
}

//弾と過去の敵
bool Collision(std::vector<C_PlayerBullet>& bulletList, std::vector<C_FormerEnemy> fEnemyList, C_HUD* HUD)
{
	for (auto& b : bulletList)
	{
		if (!b.IsActive())continue;
		for (auto& e : fEnemyList)
		{
			if (!e.IsActive())continue;
			if (IsCollision(b.GetPos(), b.GetRadius(), e.GetPos(), e.GetRadius().x))
			{
				b.SetActive(false);
				e.SetActive(false);
				HUD->AddScore(100);
				FADE_EFFECT.Emit(e.GetPos(), e.GetRadius().x);
			}
		}
	}
	return true;
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
