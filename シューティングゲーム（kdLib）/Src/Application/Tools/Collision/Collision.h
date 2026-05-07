#pragma once

//当たり判定用関数のまとめファイル

class Player;
class BaseEnemy;
class PlayerBullet;
class EnemyBullet;
class Score;
class PlayerBullet;
class Boss;
class FireworksManager;
class HitEffectManager;

//プレイヤー　と　敵
bool CollisionPlayer_Enemy(std::shared_ptr<Player> player, std::vector<std::shared_ptr<BaseEnemy>>& enemyList);

//プレイヤー　と　弾
bool CollisionPlayer_EBullet(std::shared_ptr<Player> player, std::vector<std::shared_ptr<EnemyBullet>>& bulletList);

//プレイヤー　と　ボス
bool CollisionPlayer_Boss(std::shared_ptr<Player>player, std::shared_ptr<Boss>boss);

//弾　と　弾（いらなそう）
//bool Collision(Bullets* b1,Bullets*b2);

//プレイヤーの弾　と　敵
bool CollisionPlayerBullet_Enemy(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::vector<std::shared_ptr<BaseEnemy>>& enemyList,std::shared_ptr<FireworksManager> fireworksManager,std::shared_ptr<HitEffectManager>hitEffectManager,std::shared_ptr<Score> score);

//プレイヤーの弾　と　ボス
bool CollisionPlayerBullet_Boss(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::shared_ptr<Boss>boss, std::shared_ptr<FireworksManager> fireworksManager,std::shared_ptr<HitEffectManager>hitEffectManager);


//円形当たり判定
bool IsCollision(Math::Vector2 pos1,float radius1, Math::Vector2 pos2,float radius2);

//矩形当たり判定
bool IsCollision_Box(Math::Vector2 pos1, Math::Vector2 radius1, Math::Vector2 pos2, Math::Vector2 radius2);