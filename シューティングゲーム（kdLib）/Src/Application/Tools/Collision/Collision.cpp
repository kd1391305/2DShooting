#include "Collision.h"
#include"../../Chara/Player/Player.h"
#include"../../Bullet/EnemyBullet/EnemyBullet.h"
#include"../../Bullet/PlayerBullet/PlayerBullet.h"
#include"../../Timer/Timer.h"
#include"../../UI/Score/Score.h"
#include"../../Fireworks/Fireworks.h"
#include"../../Chara/Enemy/BaseEnemy/BaseEnemy.h"
#include"../RandEx/RandEx.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Chara/Enemy/Boss/Boss.h"

//プレイヤー　と　敵
bool CollisionPlayer_Enemy(std::shared_ptr<Player> player, std::vector<std::shared_ptr<BaseEnemy>>& enemyList)
{
	//プレイヤーが無敵中の場合
	if (player->IsInvincible())return false;

	for (auto& e : enemyList)
	{
		if (!e->IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, e->GetPos(), e->GetRadius().x))
		{
			player->Damage(10);
			player->Invincible(1.5f);			//1.5秒間無敵
			e->Damage(10);
			Timer::Instance().Stop(0.35f);		//0.35秒停止
			return true;
		}
	}
	return false;
}

//プレイヤー　と　弾
bool CollisionPlayer_EBullet(std::shared_ptr<Player> player, std::vector<std::shared_ptr<EnemyBullet>>& bulletList)
{
	//プレイヤーが無敵中の場合
	if (player->IsInvincible())return false;

	for (auto& b : bulletList)
	{
		if (!b->IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, b->GetPos(), b->GetRadius()))
		{
			player->Damage(10);
			player->Invincible(1.5f);			//1.5秒間無敵
			b->SetActive(false);
			Timer::Instance().Stop(0.35f);		//0.35秒停止
			return true;
		}
	}
	return false;
}

//プレイヤー　と　ボス
bool CollisionPlayer_Boss(std::shared_ptr<Player> player, std::shared_ptr<Boss> boss)
{
	if (!boss)return false;
	//プレイヤーが無敵中の場合
	if (player->IsInvincible())return false;

	if (IsCollision(player->GetPos(), player->GetRadius().x, boss->GetPos(), boss->GetRadius().x))
	{
		//プレイヤーにダメージ
		player->Damage(10);
		
		//無敵時間
		player->Invincible(1.5f);			//1.5秒間無敵
		
		//ボスにもダメージ
		boss->Damage(10);
		
		Timer::Instance().Stop(0.35f);		//0.35秒停止
		return true;
	}
	return false;
}

//チャージ弾　と　敵
bool CollisionPlayerBullet_Enemy(std::vector<std::shared_ptr<PlayerBullet>>& playerList, std::vector<std::shared_ptr<BaseEnemy>>& enemyList, std::vector<std::shared_ptr<BaseFireworks>>& fireworksList, std::shared_ptr<Score> score)
{
	for (auto& p : playerList)
	{
		if (p->IsActive() && p->GetPower() > 0)
		{
			for (auto& e : enemyList)
			{
				if (IsCollision(p->GetPos(), p->GetRadius(), e->GetPos(), e->GetRadius().x))
				{
					//ダメージ
					e->Damage(10 * p->GetPower());

					//敵に貫通する
					p->Pierce();

					//当たった時の処理
					e->OnHit();

					//スコアを加算する
					score->Add(100);

					//花火を弾けさせる
					{
						std::shared_ptr<BaseFireworks>temp;
						int type = rand() % FireworksManager::Type::Kind;
						switch (type)
						{
						case FireworksManager::Type::Circle:
							temp = std::make_shared<Fireworks1>();
							break;
						case FireworksManager::Type::Circle_Line:
							temp = std::make_shared<Fireworks2>();
							break;
						case FireworksManager::Type::NewCircle:
							temp = std::make_shared<Fireworks3>();
							break;
						case FireworksManager::Type::Petal:
							temp = std::make_shared<Fireworks4>();
							break;
						}
						temp->Init();
						float afterScale = { randRange(0.1f,0.2f) };
						temp->Shot(e->GetPos(), Math::Vector2{ NULL, NULL }, NULL, afterScale, e->GetColor());
						temp->Explode();
						fireworksList.push_back(temp);
					}
					
					
					//弾が消えるとき（この弾が敵を倒した数、花火を撃つ）
					if(p->GetPower() <= 0)
					{
						p->SetActive(false);
						std::shared_ptr<BaseFireworks>temp;
						Math::Vector2 shotPos;
						float r, g, b, a, afterScale;
						for (int i = 1; i < p->GetPierceNum(); i++)
						{
							//花火を弾けさせる
							int type = rand() % FireworksManager::Type::Kind;
							switch (type)
							{
							case FireworksManager::Type::Circle:
								temp = std::make_shared<Fireworks1>();
								break;
							case FireworksManager::Type::Circle_Line:
								temp = std::make_shared<Fireworks2>();
								break;
							case FireworksManager::Type::NewCircle:
								temp = std::make_shared<Fireworks3>();
								break;
							case FireworksManager::Type::Petal:
								temp = std::make_shared<Fireworks4>();
								break;
							}
							temp->Init();
							shotPos = e->GetPos();
							shotPos.x += randRange(-100, 100);
							shotPos.y += randRange(-100, 100);

							afterScale = { randRange(0.2f,0.3f) };

							r = randRange(0.0f, 0.6f);
							g = randRange(0.0f, 0.6f);
							b = randRange(0.0f, 0.6f);
							a = randRange(0.4f, 0.6f);
							temp->Shot(shotPos, Math::Vector2{ NULL,NULL } ,NULL, afterScale, Math::Color{ r,g,b,a });
							temp->Explode();
							temp->Draw();
							fireworksList.push_back(temp);
						}
						break;
					}
					
				}
			}
		}
	}
	return false;
}

//プレイヤーの弾　と　ボス
bool CollisionPlayerBullet_Boss(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::shared_ptr<Boss> boss, std::vector<std::shared_ptr<BaseFireworks>>& fireworksList)
{
	if (!boss)return false;

	Math::Vector2 bossPos = boss->GetPos();
	float bossRadius = boss->GetRadius().x;

	for (auto& bullet : playerBullet)
	{
		if (!bullet->IsActive())continue;
		if (IsCollision(bossPos, bossRadius, bullet->GetPos(), bullet->GetRadius()))
		{
			//敵に貫通する
			bullet->Pierce();

			//ダメージ
			boss->Damage(10);

			//当たった時の処理
			boss->OnHit();

			//花火を弾けさせる
			{
				std::shared_ptr<BaseFireworks>temp;
				int type = rand() % FireworksManager::Type::Kind;
				switch (type)
				{
				case FireworksManager::Type::Circle:
					temp = std::make_shared<Fireworks1>();
					break;
				case FireworksManager::Type::Circle_Line:
					temp = std::make_shared<Fireworks2>();
					break;
				case FireworksManager::Type::NewCircle:
					temp = std::make_shared<Fireworks3>();
					break;
				case FireworksManager::Type::Petal:
					temp = std::make_shared<Fireworks4>();
					break;
				}
				temp->Init();
				float afterScale = { randRange(0.1f,0.2f) };
				float r, g, b, a;
				r = randRange(0, 0.5f);
				g = randRange(0, 0.5f);
				b = randRange(0, 0.5f);
				a = randRange(0.4f, 0.6f);

				temp->Shot(bullet->GetPos(), Math::Vector2{ NULL, NULL }, NULL, afterScale, Math::Color{ r,g,b,a });
				temp->Explode();
				fireworksList.push_back(temp);
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

	if (c < radius1 + radius2)
	{
		return true;
	}
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
