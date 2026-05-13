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
class AttackAreaManager;

//プレイヤー　と　敵
bool CollisionPlayer_Enemy(std::shared_ptr<Player> player, std::vector<std::shared_ptr<BaseEnemy>>& enemyList);

//プレイヤー　と　弾
bool CollisionPlayer_EBullet(std::shared_ptr<Player> player, std::vector<std::shared_ptr<EnemyBullet>>& bulletList);

//プレイヤー　と　ボス
bool CollisionPlayer_Boss(std::shared_ptr<Player>player, std::shared_ptr<Boss>boss);

//プレイヤー　と　攻撃範囲
bool CollisionPlayer_AttackArea(std::shared_ptr<Player>player, std::shared_ptr<AttackAreaManager>attackAreaManager);

//弾　と　弾（いらなそう）
//bool Collision(Bullets* b1,Bullets*b2);

//プレイヤーの弾　と　敵
bool CollisionPlayerBullet_Enemy(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::vector<std::shared_ptr<BaseEnemy>>& enemyList,std::shared_ptr<FireworksManager> fireworksManager,std::shared_ptr<HitEffectManager>hitEffectManager,std::shared_ptr<Score> score);

//プレイヤーの弾　と　ボス
bool CollisionPlayerBullet_Boss(std::vector<std::shared_ptr<PlayerBullet>>& playerBullet, std::shared_ptr<Boss>boss, std::shared_ptr<FireworksManager> fireworksManager,std::shared_ptr<HitEffectManager>hitEffectManager,std::shared_ptr<Score> score);



//円形当たり判定
bool IsCollision(Math::Vector2 pos1,float radius1, Math::Vector2 pos2,float radius2);

//矩形当たり判定
bool IsCollision_Box(Math::Vector2 pos1, Math::Vector2 radius1, Math::Vector2 pos2, Math::Vector2 radius2);

//ボス用の当たり判定(長方形と平行四辺形)
bool IsCollision_Boss(Math::Vector2 pos, Math::Vector2 radius, std::shared_ptr<Boss>boss);

//ボス用の当たり判定（円と平行四辺形）
bool IsCollision_Boss(Math::Vector2 pos, float radius, std::shared_ptr<Boss>boss);

//線の位置と点の位置を調べる（ -1 = 左側、 0 = 重なっている, 1 = 右側）
float IsCross(Math::Vector2 linePos1, Math::Vector2 linePos2, Math::Vector2 pos);