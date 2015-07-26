#include "cocos2d.h"
USING_NS_CC;

#define LEVELNUMBER 1
#define ATTACKNUMBER 2


class player {
public:
	Sprite* sprite;
	float moveSpeed;
	float mass;
	float jumpForce;
	float impulse;
	int attack = 0;
	bool isAttacking = false;

	int beHitNumber = 0;

	int contactNumber = 0;
	
	player();
	void setAttack(int _attack);
	void clearAttack();
};