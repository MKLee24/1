#include "cocos2d.h"
USING_NS_CC;

#define LEVELNUMBER 1
#define ATTACKNUMBER 3
#define ACTIONNUMBER 5


class player {
public:
	Sprite* sprite;
	float moveSpeed;
	float mass;
	float jumpForce;
	float impulse;
	int attack = 0;
	bool isAttacking = false;
	bool jumped = false;

	int beHitNumber = 0;

	int contactNumber = 0;
	
	player();
	void setAttack(int _attack);
	void clearAttack();
	
	bool isRunning = false;
	void clearRunning();
	bool isStanding = false;
	void clearStanding();

	Point attackPoint[ATTACKNUMBER];

};