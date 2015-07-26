#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <Vector>
using namespace CocosDenshion;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
	return scene;
}

HelloWorld* HelloWorld::create(PhysicsWorld* world,int level)
{
	HelloWorld* pRet = new HelloWorld();
	if (pRet && pRet->init(world,level)){
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

bool HelloWorld::init(PhysicsWorld* world,int _level)
{
    if ( !Layer::init() )
    {
        return false;
    } 
	level = _level;
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(HelloWorld::setLevel), "selectedLevel", NULL);
	dispatcher = Director::getInstance()->getEventDispatcher();

	preLoadMusic();
	playBgMusic();

	m_world = world;
	//m_world->setAutoStep(false);
	m_world->setGravity(Vect(0,-grivity));
	this->setAnchorPoint(Point(0,0));
    winSize = Director::getInstance()->getWinSize();
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto map1path = String::createWithFormat("%s%d%s","images/level",level,"/bg.png");
	map1 = Sprite::create(map1path->getCString());
	map1->setPosition(map1->getContentSize().width/2+origin.x, map1->getContentSize().height/2+origin.y);
	this->addChild(map1,1,MAP1_TAG);
	auto map2path = String::createWithFormat("%s%d%s", "images/level", level, "/bg.png");
	map2 = Sprite::create(map2path->getCString());
	map2->setPosition(map1->getContentSize().width+map2->getContentSize().width/2+origin.x, map2->getContentSize().height/2+origin.y);
	this->addChild(map2,1,MAP2_TAG);

	Sprite* edge1 = Sprite::create();
	edge1->setContentSize(visibleSize);
	PhysicsBody* edgeBody1 = PhysicsBody::createEdgeBox(visibleSize);
	edgeBody1->setDynamic(false);
	edgeBody1->setGravityEnable(false);
	edgeBody1->setMass(100000.0f);
	edgeBody1->getShape(0)->setRestitution(0.0f);
	edgeBody1->setContactTestBitmask(0xFF);
	edgeBody1->setCategoryBitmask(0x04);
	edgeBody1->setCollisionBitmask(0xFF);
	edgeBody1->getShape(0)->setTag(EDGE_TAG);
	edge1->setPhysicsBody(edgeBody1);
	edge1->setPosition(visibleSize.width/2,visibleSize.height/2);
	addChild(edge1,1,EDGEONE_TAG);

	Sprite* edge = Sprite::create();
	edge->setContentSize(visibleSize);
	PhysicsBody* edgeBody = PhysicsBody::createEdgeBox(Size(visibleSize.width,visibleSize.height/6));
	edgeBody->setDynamic(false);
	edgeBody->setGravityEnable(false);
	edgeBody->setMass(100000.0f);
	edgeBody->getShape(0)->setRestitution(0.0f);
	edgeBody->setContactTestBitmask(0xFF);
	edgeBody->setCategoryBitmask(0x04);
	edgeBody->setCollisionBitmask(0xFF);
	edgeBody->getShape(0)->setTag(EDGE_TAG);
	edge->setPhysicsBody(edgeBody);
	edge->setPosition(visibleSize.width / 2, visibleSize.height / 12);
	addChild(edge, 1, EDGE_TAG);


	Sprite* floor = Sprite::create();
	PhysicsBody* floorBody = PhysicsBody::createEdgeSegment(Point(0, visibleSize.height / 10), Point(visibleSize.width, visibleSize.height / 10));
	floorBody->setDynamic(false);
	floorBody->setGravityEnable(false);
	floorBody->getShape(0)->setRestitution(0.0f);
	floorBody->setMass(100000.0f);
	floorBody->setCategoryBitmask(0x0003);
	floorBody->setContactTestBitmask(0x0003);
	floorBody->setCollisionBitmask(0x0001);
	floor->setPosition(0, visibleSize.height / 10);
	floor->setPhysicsBody(floorBody);
	//this->addChild(floor,2,FLOOR_TAG);

	setEnemyFrameNumber();
	setPlayerFrameNumber();
	initDamage();

	player1 = new player();
	auto playerPath = String::createWithFormat("%s%d%s", "images/level", level, "/player.png");
	player1->sprite = Sprite::create(playerPath->getCString());
	PhysicsBody* playerBody = PhysicsBody::createBox(player1->sprite->getContentSize());
	playerBody->setRotationEnable(false);
	playerBody->getShape(0)->setRestitution(0.0f);
	playerBody->getShape(0)->setFriction(0.8f);
	playerBody->setMass(player1->mass);
	playerBody->setCollisionBitmask(0x04);
	playerBody->setCategoryBitmask(0x01);
	playerBody->setContactTestBitmask(0xFF);
	playerBody->getShape(0)->setTag(PLAYER_TAG);
	player1->sprite->setPhysicsBody(playerBody);
	player1->sprite->setPosition(player1->sprite->getContentSize().width/2+5, floor->getPositionY()+player1->sprite->getContentSize().height*3);
	addChild(player1->sprite,2,PLAYER_TAG);

	preLoadAnimation();

	auto backItem = MenuItemImage::create("images/CloseNormal.png", "images/CloseSelected.png", CC_CALLBACK_0(HelloWorld::backToChoseLevel, this));
	//auto pauseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_0(HelloWorld::pauseScene, this));
	auto popupItem = MenuItemImage::create("images/CloseNormal.png", "images/CloseSelected.png", CC_CALLBACK_0(HelloWorld::toPopup, this));
	//pauseItem->setPosition(-backItem->getContentSize().width,0);
	popupItem->setPosition(0,0);
	backItem->setPosition(backItem->getContentSize().width,0);

	auto backmenu = Menu::create(popupItem,backItem,NULL);
	backmenu->setPosition(visibleSize.width - backItem->getContentSize().width*2, visibleSize.height - backItem->getContentSize().height / 2);
	addChild(backmenu, 10);

	hpLayer = HPLayer::create();
	hpLayer->setPosition(0, visibleSize.height / 15 * 14);
	addChild(hpLayer, 2, HPLAYER_TAG);

	Vector<MenuItem*> attackLogoItems;
	MenuItemSprite* attackLogoItem;
	for (int i = 0; i < ATTACKNUMBER; i++){
		auto logoPath = String::createWithFormat("%s%d%s%d%s", "images/level", level, "/attack", i, "/logo.png");
		auto normalSprite = Sprite::create(logoPath->getCString());
		normalSprite->setOpacity(200);
		auto selectedSprite = Sprite::create(logoPath->getCString());
		attackLogoItem = MenuItemSprite::create(normalSprite, selectedSprite, CC_CALLBACK_0(player::setAttack, player1, i));
		attackLogoItem->setPosition(Vec2(0, -i*attackLogoItem->getContentSize().height));
		attackLogoItems.pushBack(attackLogoItem);

	}
	auto attackLogoMenu = Menu::createWithArray(attackLogoItems);
	attackLogoMenu->setPosition(visibleSize.width - attackLogoItem->getContentSize().width, visibleSize.height / 2 + 2 * attackLogoItem->getContentSize().height);
	addChild(attackLogoMenu, 10);


	onTouchEvent();
	onKeyBoardEvent();
	physicsEvent();
	EventCustom e("custom");
	dispatcher->dispatchEvent(&e);

	enemyComing(1.0f);
	

	this->schedule(schedule_selector(HelloWorld::enemyComing), 5.0f);
	this->schedule(schedule_selector(HelloWorld::enemyAttack), 2.0f);
	this->scheduleUpdate();
	
	log("init");
    return true;
}

//void HelloWorld :: onEnter(){
//	log("Enter");
	//this->schedule(schedule_selector(HelloWorld::enemyComing), 5.0f);
	//this->schedule(schedule_selector(HelloWorld::enemyAttack), 2.0f);
	//this->scheduleUpdate();
//}

//void HelloWorld::onExit(){
	//this->unscheduleAllSelectors();
//}


void HelloWorld::preLoadMusic(){
	auto bgMusicPath = String::createWithFormat("%s%d%s", "sounds/bgMusic_", level, ".mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMusicPath->getCString());
}
void HelloWorld::playBgMusic(){
	auto bgMusicPath = String::createWithFormat("%s%d%s", "sounds/bgMusic_", level, ".mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusicPath->getCString(), true);
}
void HelloWorld::stopBgMusic(){
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void HelloWorld::setEnemyFrameNumber(){
	enemyFrameNumber[0][0] = 1;
	enemyFrameNumber[0][1] = 7;
}
void HelloWorld::setPlayerFrameNumber(){
	playerFrameNumber[0][0] = 7;
	playerFrameNumber[0][1] = 12;
}

void HelloWorld::initDamage(){
	playerDamage.push_back(34.0f);
	playerDamage.push_back(101.0f);
	enemyDamage.push_back(26);
	enemyDamage.push_back(26);
}

void HelloWorld::preLoadAnimation(){
	for (int i = 0; i < ATTACKNUMBER; i++){
		playerAttackFrame[i].reserve(playerFrameNumber[level][i]);
		for (int j = 0; j < playerFrameNumber[level][i]; j++){
			auto attackPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/attack",i,"/", j, ".png");
			auto tempPlayerSprite = Sprite::create(attackPath->getCString());
			playerAttackFrame[i].pushBack(SpriteFrame::create(attackPath->getCString(), Rect(0, 0, tempPlayerSprite->getContentSize().width, tempPlayerSprite->getContentSize().height)));
		}
	}
	//enemy
	for (int i = 0; i < ATTACKNUMBER; i++){
		enemyAttackFrame[i].reserve(enemyFrameNumber[level][i]);
		for (int j = 0; j < enemyFrameNumber[level][i]; j++){
			auto enemyPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/enemy", i, "/",j,".png");
			auto tempEnemySprite = Sprite::create(enemyPath->getCString());
			enemyAttackFrame[i].pushBack(SpriteFrame::create(enemyPath->getCString(), Rect(0, 0, tempEnemySprite->getContentSize().width, tempEnemySprite->getContentSize().height)));
		}
	}
}

void HelloWorld::enemyComing(float dt){
	int enemyType = (int)(CCRANDOM_0_1() * ATTACKNUMBER);
	//int enemyType = 1;
	enemy* enemy1 = new enemy();
	auto enemyPath = String::createWithFormat("%s%d%s%d%s", "images/level", level, "/enemy",enemyType,"/0.png");
	enemy1->sprite = Sprite::create(enemyPath->getCString());
	enemy1->attack = enemyType;
	if (enemy1->attack == 0) enemy1->moveSpeed *= 2;
	enemy1->distance = CCRANDOM_0_1() + 3;
	enemy1->sprite->setPosition(visibleSize.width-enemy1->sprite->getContentSize().width/2,visibleSize.height/6+enemy1->sprite->getContentSize().height/2+5);
	auto enemyBody = PhysicsBody::createBox(enemy1->sprite->getContentSize());
	enemyBody->setCategoryBitmask(0x01);
	enemyBody->setCollisionBitmask(0x04);
	enemyBody->setContactTestBitmask(0xFF);
	enemyBody->getShape(0)->setMass(10000);
	enemyBody->getShape(0)->setRestitution(0.0f);
	enemyBody->getShape(0)->setTag(ENEMY_TAG_ZERO+enemyType);
	enemy1->sprite->setPhysicsBody(enemyBody);
	addChild(enemy1->sprite,2,ENEMY_TAG_ZERO+enemyType);
	enemies.push_back(enemy1);
}

bullet* HelloWorld::throwBullet(Point startPosition, Vec2 force){
	auto bulletPath = String::createWithFormat("%s%d%s", "images/level", level, "/bullet.png");
	auto bullet1 = new bullet();
	bullet1->sprite = Sprite::create(bulletPath->getCString());
	bullet1->sprite->setPosition(startPosition);
	auto bulletBody = PhysicsBody::createBox(bullet1->sprite->getContentSize());
	bulletBody->getShape(0)->setMass(bullet1->mass);
	bulletBody->getShape(0)->setRestitution(0.0f);
	bulletBody->setCategoryBitmask(0x00);
	bulletBody->setCollisionBitmask(0x00);
	bulletBody->setContactTestBitmask(0xFF);
	bulletBody->getShape(0)->setTag(BULLET_TAG);
	bulletBody->applyImpulse(force);
	bullet1->sprite->setPhysicsBody(bulletBody);
	return bullet1;
}

void HelloWorld::updatePlayerHP(float dt){
	if (player1->attack == 1 && player1->isAttacking)  {
		player1->beHitNumber = 0;
		return;
	}
	auto HPProgess = (ProgressTimer*)(hpLayer->getChildByTag(HPTAG));
	auto nowHP = HPProgess->getPercentage();
	float decreaseHP = player1->contactNumber * enemyDamage.at(0)* dt + player1->beHitNumber* enemyDamage.at(1);
	HPProgess->setPercentage(nowHP-decreaseHP);
	player1->beHitNumber = 0;
}

void HelloWorld::playerMove(bool isPressedW, bool isPressedA, bool isPressedD, float time){
	if (isPressedD) {
		if (player1->sprite->getPositionX() >= visibleSize.width / 3){
			map1->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			map2->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			for (auto tempEnemy : enemies){
				tempEnemy->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			}
		}
		else{
			player1->sprite->runAction(MoveBy::create(time, Vec2(player1->moveSpeed * time, 0)));
		}
	}
	else if (isPressedA){
		if (player1->sprite->getPositionX() - player1->moveSpeed*time >= player1->sprite->getContentSize().width / 2 + 5) player1->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
	}
	if (isPressedW && jumped == false){
		auto _jumpForce = Vec2(0, player1->jumpForce);
		player1->sprite->getPhysicsBody()->applyImpulse(_jumpForce);
		jumped = true;
	}
}

void HelloWorld::enemyMove(float time){
	auto playerPos = player1->sprite->getPosition();
	auto playerSize = player1->sprite->getContentSize();
	std::vector<enemy*>::iterator enemyIt = enemies.begin();
	for (; enemyIt != enemies.end(); enemyIt++){
		(*enemyIt)->enemyMove(playerPos,visibleSize,playerSize,time);
	}
}

void HelloWorld::enemyAttack(float time){
	std::vector<enemy*>::iterator it = enemies.begin();
	for (; it != enemies.end(); it++){
		if ( (*it)->attack==1 && (*it)->isAttacking == false){
			(*it)->isAttacking = true;
			auto animation = Animation::createWithSpriteFrames(enemyAttackFrame[(*it)->attack], 0.1f);
			auto animate = Animate::create(animation);
			auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(enemy::clearAttack, (*it))), NULL);
			(*it)->sprite->runAction(actions);
			auto force = (*it)->impulse*(Vec2((player1->sprite->getPositionX() - (*it)->sprite->getPositionX()), visibleSize.height));
			auto tempBullet = throwBullet((*it)->sprite->getPosition(),force);
			this->addChild(tempBullet->sprite,3,ENEMY_BULLET_TAG);
			enemyBullets.push_back(tempBullet);
		}
	}
}

void HelloWorld::updateBullet(float time){
	int size = bullets.size();
	std::vector<bullet*>::iterator it;
	for (it = bullets.begin(); it != bullets.end();){
		Sprite* tempBullet = (*it)->sprite;
		bool needRemoveBullet = false;
		Vec2 tempPosition = tempBullet->getPosition();
		if (tempBullet->getPositionX() <= tempBullet->getContentSize().width / 2 + 1) needRemoveBullet = true;
		else if (tempBullet->getPositionX() >= visibleSize.width - tempBullet->getContentSize().width / 2 - 1) needRemoveBullet = true;
		else if (tempBullet->getPositionY() <= tempBullet->getContentSize().height / 2 + 1) needRemoveBullet = true;
		else if (tempBullet->getPositionY() >= visibleSize.height - tempBullet->getContentSize().height / 2 - 1) needRemoveBullet = true;

		std::vector<enemy*>::iterator enemyIt;
		for (enemyIt = enemies.begin(); enemyIt != enemies.end();){
			Sprite* tempEnemy = (*enemyIt)->sprite;
			if (abs(tempEnemy->getPositionX() - tempBullet->getPositionX()) <= tempEnemy->getContentSize().width / 2 + tempBullet->getContentSize().width / 2
				&& abs(tempEnemy->getPositionY() - tempBullet->getPositionY()) <= tempEnemy->getContentSize().height / 2 + tempBullet->getContentSize().height / 2){
				(*enemyIt)->HP -= playerDamage.at(0);
				needRemoveBullet = true;
				break;
			}
			else{
				enemyIt++;
			}
		}

		if (needRemoveBullet){
			tempBullet->removeFromParentAndCleanup(true);
			it = bullets.erase(it);/////////////////////////////////////////////remeber
		}
		else{
			it++;
		}
	}
}

void HelloWorld::updateEnemyBullet(float time){
	int size = enemyBullets.size();
	std::vector<bullet*>::iterator it;
	for (it = enemyBullets.begin(); it !=enemyBullets.end();){
		Sprite* tempBullet = (*it)->sprite;
		bool needRemoveBullet = false;
		Vec2 tempPosition = tempBullet->getPosition();
		if (tempPosition.x<= tempBullet->getContentSize().width / 2 + 1) needRemoveBullet = true;
		else if (tempPosition.x>= visibleSize.width - tempBullet->getContentSize().width / 2 - 1) needRemoveBullet = true;
		else if (tempPosition.y<= tempBullet->getContentSize().height / 2 + 1) needRemoveBullet = true;
		else if (tempPosition.y >= visibleSize.height - tempBullet->getContentSize().height / 2 - 1) needRemoveBullet = true;
		else if (abs(player1->sprite->getPositionX() - tempPosition.x) <= (player1->sprite->getContentSize().width / 2 + tempBullet->getContentSize().width / 2) &&
			abs(player1->sprite->getPositionY() - tempPosition.y) <= (player1->sprite->getContentSize().height / 2 + tempBullet->getContentSize().height / 2)){
			player1->beHitNumber++;
			needRemoveBullet = true;
		}

		if (needRemoveBullet){
			tempBullet->removeFromParentAndCleanup(true);
			it = enemyBullets.erase(it);/////////////////////////////////////////////remeber
		}
		else{
			it++;
		}
	}
}

void HelloWorld::updatePlayerAttackOne(float time){
	if (player1->attack == 1 && player1->isAttacking){
		auto tempPos = player1->sprite->getPosition();
		std::vector<enemy*>::iterator it = enemies.begin();
		for (; it != enemies.end();it++){
			if (abs((*it)->sprite->getPositionX() - tempPos.x) <= playerAttackFrame[player1->attack].at(0)->getRect().size.width/2+(*it)->sprite->getContentSize().width && 
				abs((*it)->sprite->getPositionY()-tempPos.y) <= playerAttackFrame[player1->attack].at(0)->getRect().size.height/2+(*it)->sprite->getContentSize().height){
				(*it)->HP -= playerDamage.at(1) * time;
			}
		}
	}
}

void HelloWorld::updateEnemyHP(float time){
	std::vector<enemy*>::iterator enemyIt = enemies.begin();
	for (; enemyIt != enemies.end();){
		if ((*enemyIt)->HP <= 0){
			(*enemyIt)->sprite->removeFromParentAndCleanup(true);
			enemyIt = enemies.erase(enemyIt);
		}
		else{
			enemyIt++;
		}
	}
}

void HelloWorld::update(float time){
	//循环地图
	Sprite* tempMap1 = (Sprite*)this->getChildByTag(MAP1_TAG);
	Sprite* tempMap2 = (Sprite*)this->getChildByTag(MAP2_TAG);
	if (tempMap1->getPositionX() + tempMap1->getContentSize().width / 2 <= origin.x){
		//tempMap1->setPositionX(tempMap2->getContentSize().width+tempMap1->getContentSize().width/2);
		float offset = tempMap1->getPositionX() + tempMap1->getContentSize().width / 2 - origin.x;
		tempMap1->setPosition(Point(tempMap1->getContentSize().width / 2 + origin.x + visibleSize.width + offset, visibleSize.height / 2 + origin.y));
	}
	if (tempMap2->getPositionX() + tempMap2->getContentSize().width / 2 <= origin.x) {
		//tempMap2->setPositionX(tempMap1->getContentSize().width+tempMap2->getContentSize().width/2);
		float offset = tempMap2->getPositionX() + tempMap2->getContentSize().width / 2 - origin.x;
		tempMap2->setPosition(Point(tempMap2->getContentSize().width / 2 + origin.x + visibleSize.width + offset, visibleSize.height / 2 + origin.y));
	}

	//playerMove
	playerMove(isPressedW, isPressedA, isPressedD, time);
	//enemy0Move
	enemyMove(time);

	//updateBullet
	updateBullet(time);

	//updateEnemyBullet
	updateEnemyBullet(time);

	updatePlayerAttackOne(time);

	updatePlayerHP(time);

	updateEnemyHP(time);
}




void HelloWorld::onTouchEvent(){
	//触屏事件
	auto toucherListener = EventListenerTouchOneByOne::create();
	toucherListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	toucherListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	toucherListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(toucherListener, this);
}
void HelloWorld::onKeyBoardEvent(){
	//按键事件
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

void HelloWorld::customEvent(){
	//自定义事件
	auto customEventListener = EventListenerCustom::create("custom", [](EventCustom* e){
		log("'custom' event");
	});
	dispatcher->addEventListenerWithSceneGraphPriority(customEventListener,this);
}

void HelloWorld::physicsEvent(){
	//碰撞事件
	auto physicsListener = EventListenerPhysicsContact::create();
	physicsListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin,this);
	physicsListener->onContactSeperate = CC_CALLBACK_1(HelloWorld::onContactSeperate,this);
	dispatcher->addEventListenerWithSceneGraphPriority(physicsListener,this);
}

bool HelloWorld::onContactBegin(PhysicsContact& contact){
	//接触开始
	auto a = contact.getShapeA()->getTag();
	auto b = contact.getShapeB()->getTag();
	if ((a*b) == PLAYER_TAG * EDGE_TAG) {
		isContact = true;
		isPressedW = false;
		jumped = false;
	}
	else if ((a*b) == PLAYER_TAG*ENEMY_TAG_ZERO){
		player1->contactNumber++;
	}
	auto temp = String::createWithFormat("%d", player1->contactNumber);
	log(temp->getCString());
	log("onContactBegin");
	return true;
}

void HelloWorld::onContactSeperate(PhysicsContact& contact){
	//接触结束
	auto a = contact.getShapeA()->getTag();
	auto b = contact.getShapeB()->getTag();
	if ((a * b) == PLAYER_TAG * EDGE_TAG){
		isContact = false;
	}
	else if ((a*b) == PLAYER_TAG * ENEMY_TAG_ZERO){
		player1->contactNumber--;
	}
	log("onContactSeperate");
}

void HelloWorld::particleTest(){
	//粒子效果
	auto particle = ParticleExplosion::create();
	this->addChild(particle);
}

void HelloWorld::playerAttack(Point touchPos){
	auto animation = Animation::createWithSpriteFrames(playerAttackFrame[player1->attack], 0.1f);
	auto animate = Animate::create(animation);
	//auto attack1 = Animate::create(Animation::createWithSpriteFrames(attack1Frame, 0.4f));
	auto animateSprite = Sprite::create();
	auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(player::clearAttack, player1)), CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, animateSprite, true)), NULL);
	animateSprite->setPosition(player1->sprite->getContentSize().width / 2, player1->sprite->getContentSize().height / 2);
	player1->sprite->addChild(animateSprite);
	animateSprite->runAction(actions);
	if (player1->attack == 0){
		auto force = player1->impulse*Vec2(touchPos.x - player1->sprite->getPositionX(), touchPos.y - player1->sprite->getPositionY());
		bullet* bullet1 = throwBullet(player1->sprite->getPosition(), force);
		this->addChild(bullet1->sprite,3,BULLET_TAG);
		bullets.push_back(bullet1);
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event){
	if (!player1->isAttacking){
		player1->isAttacking = true;
		playerAttack(touch->getLocation());
	}
	log("onTouchBegin");
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event){
	log("onTouchMoved");
}
void HelloWorld::onTouchEnded(Touch* touch, Event* event){
	log("onTouchEnded");
}
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event){
	if (code == EventKeyboard::KeyCode::KEY_D){
		if(!isPressedD) isPressedD = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_A){
		if (!isPressedA) isPressedA = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_W){
		if (!isPressedW) {
			isPressedW = true;
		}
	}
	log("onKeyPressed");
}
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event){
	if (code == EventKeyboard::KeyCode::KEY_D){
		if (isPressedD) isPressedD = false;
	}
	else if (code == EventKeyboard::KeyCode::KEY_A){
		if (isPressedA) isPressedA = false;
	}
	log("onKeyReleased");
}

//void HelloWorld::setLevel(Ref* pSender){
//	auto msg = (String*)pSender;
//	level = msg->intValue();
//	for (int i = 0; i < 100; i++){
//		log("okok");
//	}
//}

void HelloWorld::backToChoseLevel(){
	auto scene = ChoseLevel::createScene();
	Director::getInstance()->replaceScene(CCTransitionCrossFade::create(1.0f, scene));
	stopBgMusic();
}

void HelloWorld::toPopup(){
	/*
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width,visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	*/
	//auto popupScene = Popup::createScene(renderTexture,"images/popupBg.png");
	auto popup = Popup::create("images/popupBg.png");
	this->addChild(popup, 10);
	Action* popupAction = Sequence::create(ScaleTo::create(0.0f, 0.0f),
									ScaleTo::create(0.06f, 1.05f),
									ScaleTo::create(0.08f, 0.95f),
									ScaleTo::create(0.08f, 1.0f), NULL);
	popup->runAction(popupAction);
	 //Director::getInstance()->pause();ddd
	//director->pushScene(director->getRunningScene());
	//Director::getInstance()->replaceScene(popupScene);
}

void HelloWorld::pauseScene(){
	if (!isPaused) {
		Director::getInstance()->pause();
		isPaused = true;
	}
	else {
		Director::getInstance()->resume();
		isPaused = false;
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}



