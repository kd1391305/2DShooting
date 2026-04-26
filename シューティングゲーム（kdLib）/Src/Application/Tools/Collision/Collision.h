#pragma once

//当たり判定用関数のまとめファイル

class Player;
class BaseEnemy;
class PlayerBullet;
class EnemyBullet;
class Score;
class BaseFireworks;

//プレイヤー　と　敵
bool CollisionPlayer_Enemy(std::shared_ptr<Player> player, std::vector<std::shared_ptr<BaseEnemy>>& enemyList);

//プレイヤー　と　弾
bool CollisionPlayer_EBullet(std::shared_ptr<Player> player, std::vector<EnemyBullet>& bulletList);

//弾　と　弾（いらなそう）
//bool Collision(Bullets* b1,Bullets*b2);

//弾　と　敵
bool CollisionFireworks_Enemy(std::vector<std::shared_ptr<BaseFireworks>>& playerList, std::vector<std::shared_ptr<BaseEnemy>> &enemyList, std::shared_ptr<Score>& score);



//円形当たり判定
bool IsCollision(Math::Vector2 pos1,float radius1, Math::Vector2 pos2,float radius2);

//矩形当たり判定
bool IsCollision_Box(Math::Vector2 pos1, Math::Vector2 radius1, Math::Vector2 pos2, Math::Vector2 radius2);