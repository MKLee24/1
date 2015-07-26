#include "player.h"

void player::setAttack(int _attack){
	attack = _attack;
}

void player::clearAttack(){
	isAttacking = false;
	log("okokok");
}

player::player(){
	moveSpeed = 200;
	mass = 10000;
	jumpForce = 200 * 20000;
	impulse = 50;
	attack = 0;
	isAttacking = false;
}

