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
#include"../../Animtion/HitEffect/HitEffectManager.h"
#include"../../Chara/Enemy/Boss/AttackArea/AttackAreaManager.h"
#include"../../Chara/Enemy/Boss/AttackArea/AttackArea.h"
#include"../../SoundCache/SoundCache.h"

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
		
		Timer::Instance().Stop(0.35f);		//0.35秒停止
		return true;
	}
	return false;
}

bool CollisionPlayer_AttackArea(std::shared_ptr<Player> player, std::shared_ptr<AttackAreaManager> attackAreaManager)
{
	if (player->IsInvincible())return false;

	for (auto& area : attackAreaManager->GetList())
	{
		if (!area->GetHitFlg())continue;
		if (IsCollision(player->GetPos(), player->GetRadius().x, area->GetPos(), area->GetRadius().x))
		{
			//プレイヤーにダメージ
			player->Damage(10);

			//無敵時間
			player->Invincible(1.5f);			//1.5秒間無敵

			Timer::Instance().Stop(0.35f);		//0.35秒停止

			return true;
		}

	}

	return false;
}

//プレイヤーの弾　と　敵
bool CollisionPlayerBullet_Enemy(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::vector<std::shared_ptr<BaseEnemy>>& enemyList, std::shared_ptr<FireworksManager> fireworksManager, std::shared_ptr<HitEffectManager> hitEffectManager, std::shared_ptr<Score> score)
{
	for (auto& p : playerBullet)
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

					

					//弾の力が０なら弾を消す　or　敵が生き残っていても消す
					if (p->GetPower() <= 0 || e->GetHp() > 0)
					{
						p->SetActive(false);
					}

					//ヒットエフェクト
					{
						Math::Vector2 emitPos;
						emitPos.y = p->GetPos().y;
						emitPos.x = e->GetPos().x - (e->GetRadius().y - fabs(e->GetPos().y - emitPos.y));

						hitEffectManager->Emit(emitPos, e->GetMove());
					}
					//敵が倒れたら、花火を弾けさせる
					if (!e->IsActive())
					{
						//総打ち上げ数をカウント
						score->AddExplodeNum(e->GetFireworksNum());

						Math::Vector2 shotPos;
						Math::Vector2 enemyPos = e->GetPos();
						float seVolume = 0.005f;
						for (int i = 0; i < e->GetFireworksNum(); i++)
						{
							if (i < 5)
							{
								int type = fireworksManager->GetRandomType_Quick();
								float afterScale = randRange(0.4f, 0.6f);
								float seVolume = 0.01f;
								//花火を弾けさせる
								shotPos = enemyPos;
								shotPos.x += randRange(-50, 50);
								shotPos.y += randRange(-50, 50);
								fireworksManager->Explode((FireworksManager::Type)type, shotPos, afterScale, e->GetColor(), seVolume);
							}
							else
							{
								float r, g, b, a;
								//5つ以上の花火を弾けさせる敵は、花火の色や大きさ、出現場所を少しランダムに変更する
								for (int i = 5; i < e->GetFireworksNum(); i++)
								{
									//花火を弾けさせる
									shotPos = enemyPos;
									shotPos.x += randRange(-100, 100);
									shotPos.y += randRange(-100, 100);

									float afterScale = { randRange(0.3f,0.5f) };

									r = randRange(0.0f, 0.9f);
									g = randRange(0.0f, 0.9f);
									b = randRange(0.0f, 0.9f);
									a = randRange(0.6f, 0.8f);

									int type = fireworksManager->GetRandomType_Quick();
									fireworksManager->Explode((FireworksManager::Type)type, shotPos, afterScale, Math::Color{ r,g,b,a }, seVolume);
								}
							}
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
bool CollisionPlayerBullet_Boss(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::shared_ptr<Boss> boss, std::shared_ptr<FireworksManager> fireworksManager, std::shared_ptr<HitEffectManager>hitEffectManager, std::shared_ptr<Score> score)
{
	if (!boss)return false;

	Math::Vector2 bossPos = boss->GetPos();
	float bossRadius = boss->GetRadius().x;

	for (auto& bullet : playerBullet)
	{
		if (!bullet->IsActive())continue;
		if (IsCollision(bossPos, bossRadius, bullet->GetPos(), bullet->GetRadius()))
		{
			//ダメージ
			boss->Damage(10 * bullet->GetPower());
			//チャージマックスで攻撃したら
			if (bullet->GetPower() == 10)
			{
				//追加ダメージ
				boss->Damage(100);
				//花火を発生
				boss->ExplodeFireworks(bullet->GetPower());

				//総打ち上げ数を増やす
				score->AddExplodeNum(5);
			}
			//チャージマックスではなかったら、ダメージ音発生
			else
			{
				std::shared_ptr<KdSoundInstance>se = SoundCache::Instance().Get("Sound/SE/Damage.wav");
				se->SetVolume(0.01f);
				se->Play(false);
			}

			bullet->SetActive(false);

			//ヒットエフェクト
			Math::Vector2 emitPos;
			emitPos.y = bullet->GetPos().y;
			emitPos.x = boss->GetPos().x - boss->GetRadius().x;
			hitEffectManager->Emit(emitPos, boss->GetMove());

			//当たった時の処理
			boss->OnHit();
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
