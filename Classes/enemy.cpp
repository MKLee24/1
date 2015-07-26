#include "enemy.h"
void enemy::setAttack(int _attack){
	attack = _attack;
}

void enemy::clearAttack(){
	isAttacking = false;
	log("okokok");
}

enemy::enemy(){
	moveSpeed = 40;
	mass = 10000;
	jumpForce = 150 * 20000;
	impulse = 38;
	attack = 0;
	isAttacking = false;
}

void enemy::enemyMove(Point playerPos, Size visibleSize, Size playerSize, float time){
	auto enemyPos = sprite->getPosition();
	if (attack == 0){
		if (playerPos.x >= enemyPos.x && playerPos.x - enemyPos.x >= (playerSize.width / 2 + sprite->getContentSize().width / 2)){
			sprite->runAction(MoveBy::create(time, Vec2(moveSpeed * time, 0)));
			if (sprite->isFlippedX()) sprite->setFlippedX(false);
		}
		else if (playerPos.x < enemyPos.x && enemyPos.x - playerPos.x >= (playerSize.width / 2 + sprite->getContentSize().width / 2)){
			sprite->runAction(MoveBy::create(time, Vec2(-moveSpeed * time, 0)));
			if (!(sprite->isFlippedX())) sprite->setFlippedX(true);
		}
	}

	else if (attack == 1){
		if (playerPos.x - 5 <= enemyPos.x){
			if (enemyPos.x - playerPos.x >= 2 * visibleSize.width / distance){
				sprite->runAction(MoveBy::create(time, Vec2(-moveSpeed * time, 0)));
			}
			else{
				sprite->runAction(MoveBy::create(time, Vec2(moveSpeed * time, 0)));
			}
			if (!(sprite->isFlippedX())) sprite->setFlippedX(true);
		}
		else{
			if (playerPos.x - enemyPos.x >= visibleSize.width / distance){
				sprite->runAction(MoveBy::create(time, Vec2(moveSpeed * time, 0)));
			}
			else{
				sprite->runAction(MoveBy::create(time, Vec2(-moveSpeed * time, 0)));
			}
			if (sprite->isFlippedX()) sprite->setFlippedX(false);
		}
	}
}


