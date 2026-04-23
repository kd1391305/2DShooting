#pragma once

//当たり判定用関数のまとめファイル

class Player;
class Enemy;
class PlayerBullet;
class EnemyBullet;
class HUD;
class Fireworks;

//プレイヤー　と　敵
bool Collision(Player* player, std::vector<Enemy>& enemyList);

//プレイヤー　と　弾
bool Collision(Player* player, std::vector<EnemyBullet>& bulletList);

//弾　と　弾（いらなそう）
//bool Collision(Bullets* b1,Bullets*b2);

//弾　と　敵
bool Collision(std::vector<Fireworks>& fireworksList, std::vector<Enemy>& enemyList, HUD* HUD);

//円形当たり判定
bool IsCollision(Math::Vector2 pos1,float radius1, Math::Vector2 pos2,float radius2);

//矩形当たり判定
bool IsCollision_Box(Math::Vector2 pos1, Math::Vector2 radius1, Math::Vector2 pos2, Math::Vector2 radius2);