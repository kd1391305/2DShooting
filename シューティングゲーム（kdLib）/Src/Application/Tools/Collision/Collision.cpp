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

//ÉvÉåÉCÉÑÅ[Å@Ç∆Å@ìG
bool CollisionPlayer_Enemy(std::shared_ptr<Player> player, std::vector<std::shared_ptr<BaseEnemy>>& enemyList)
{
	//ÉvÉåÉCÉÑÅ[Ç™ñ≥ìGíÜÇÃèÍçá
	if (player->IsInvincible())return false;

	for (auto& e : enemyList)
	{
		if (!e->IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, e->GetPos(), e->GetRadius().x))
		{
			player->Damage(10);
			player->Invincible(1.5f);			//1.5ïbä‘ñ≥ìG
			e->Damage(10);
			Timer::Instance().Stop(0.35f);		//0.35ïbí‚é~
			return true;
		}
	}
	return false;
}

//ÉvÉåÉCÉÑÅ[Å@Ç∆Å@íe
bool CollisionPlayer_EBullet(std::shared_ptr<Player> player, std::vector<std::shared_ptr<EnemyBullet>>& bulletList)
{
	//ÉvÉåÉCÉÑÅ[Ç™ñ≥ìGíÜÇÃèÍçá
	if (player->IsInvincible())return false;

	for (auto& b : bulletList)
	{
		if (!b->IsActive())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, b->GetPos(), b->GetRadius()))
		{
			player->Damage(10);
			player->Invincible(1.5f);			//1.5ïbä‘ñ≥ìG
			b->SetActive(false);
			Timer::Instance().Stop(0.35f);		//0.35ïbí‚é~
			return true;
		}
	}
	return false;
}

//íeÅ@Ç∆Å@íe
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

//É`ÉÉÅ[ÉWíeÅ@Ç∆Å@ìG
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
					//ìGÇ…ä—í Ç∑ÇÈ
					p->Pierce();

					//É_ÉÅÅ[ÉW
					e->Damage(10);

					//ÉXÉRÉAÇâ¡éZÇ∑ÇÈ
					score->Add(100);

					//â‘âŒÇíeÇØÇ≥ÇπÇÈ
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
						float afterScale = { randRange(0.5f,0.8f) };
						temp->Shot(e->GetPos(), {NULL,NULL}, NULL, { NULL,NULL }, { afterScale,afterScale }, e->GetColor());
						temp->Explode();
						fireworksList.push_back(temp);
					}
					
					
					//íeÇ™è¡Ç¶ÇÈÇ∆Ç´ÅiÇ±ÇÃíeÇ™ìGÇì|ÇµÇΩêîÅAâ‘âŒÇåÇÇ¬Åj
					if(p->GetPower() <= 0)
					{
						p->SetActive(false);
						std::shared_ptr<BaseFireworks>temp;
						Math::Vector2 shotPos;
						float r, g, b, a, afterScale;
						for (int i = 1; i < p->GetPierceNum(); i++)
						{
							//â‘âŒÇíeÇØÇ≥ÇπÇÈ
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

							afterScale = { randRange(0.5f,0.8f) };

							r = randRange(0.0f, 0.6f);
							g = randRange(0.0f, 0.6f);
							b = randRange(0.0f, 0.6f);
							a = randRange(0.4f, 0.6f);
							temp->Shot(shotPos, { NULL,NULL }, NULL, { NULL,NULL }, { afterScale,afterScale }, { r,g,b,a });
							temp->Explode();
							temp->Draw();
							fireworksList.push_back(temp);
						}
					}
					
				}
			}
		}
	}
	return false;
}

//â~å`ìñÇΩÇËîªíË
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

//ãÈå`ìñÇΩÇËîªíË
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
	//ç∂âEìñÇΩÇËîªíË
	if ((left1 <= left2 && right1 >= left2)||
		(right1 >= right2 && left1 <= right2))
	{
		//è„â∫ìñÇΩÇËîªíË
		if ((top1 >= bottom2 && bottom1 <= top2) || 
			(top1 >= bottom2 && bottom1 <= bottom2))
			return true;
	}

	return false;
}
