
#include "cocos2d.h"
#include "bullet.h"
USING_NS_CC;

#define LEVELNUMBER 1
#define ATTACKNUMBER 2

class enemy{
public:
	Sprite* sprite;
	float moveSpeed;
	float mass;
	float jumpForce;
	float impulse;
	int attack;
	bool isAttacking;

	float beHitNumber = 0;

	float distance; ///enemy1

	float HP = 100.0f;

	Vector<SpriteFrame*> attackFrame;
	int frameNumber[LEVELNUMBER][ATTACKNUMBER];
	enemy();
	void setAttack(int _attack);
	void clearAttack();

	void enemyMove(Point playerPos, Size visibleSize, Size playerSize, float time);
};
