#pragma once

//当たり判定用関数のまとめファイル

class C_Player;
class C_Enemy;
class C_PlayerBullet;
class C_EnemyBullet;
class C_HUD;

//プレイヤー　と　敵
bool Collision(C_Player* player, std::vector<C_Enemy>& enemyList);

//プレイヤー　と　弾
bool Collision(C_Player* player, std::vector<C_EnemyBullet>& bulletList);

//弾　と　弾（いらなそう）
//bool Collision(C_Bullets* b1,C_Bullets*b2);

//弾　と　敵
bool Collision(std::vector<C_PlayerBullet>&bulletList, std::vector<C_Enemy>& enemyList,C_HUD *HUD);

//円形当たり判定
bool IsCollision(Math::Vector2 pos1,float radius1, Math::Vector2 pos2,float radius2);

//矩形当たり判定
bool IsCollision_Box(Math::Vector2 pos1, Math::Vector2 radius1, Math::Vector2 pos2, Math::Vector2 radius2);